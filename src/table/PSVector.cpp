//
//  PSVector.cpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include "PSVector.hpp"
#include "PSCost.hpp"


namespace pse {


PSV::PSV(const Algo& a, const TonIndex& i, const PSEnum& e):
_index(i),
_algo(a),
_enum(e.clone()),
_psb_partial(),
_psb_total(),
_local(), // empty
_tiebfail(0)
{
    _psb_partial.assign(_index.size(), nullptr);
    _psb_total.assign(_index.size(), nullptr);
    _local.assign(_index.size(), TonIndex::UNDEF);
    init();
}


PSV::PSV(const Algo& a, const TonIndex& i,
         const PSEnum& e, size_t i0, size_t i1):
_index(i),
_algo(a),
_enum(e.clone(i0, i1)),
_psb_partial(),
_psb_total(),
_local(), // empty
_tiebfail(0)
{
    // give the vector their definitive size (to use as arrays)
    _psb_partial.assign(_index.size(), nullptr);
    _psb_total.assign(_index.size(), nullptr);
    _local.assign(_index.size(), TonIndex::UNDEF);
    init();
}


PSV::PSV(const Algo& a, const TonIndex& i, const PSEnum& e, size_t i0):
_index(i),
_algo(a),
_enum(e.clone(i0)),
_psb_partial(),
_psb_total(),
_local(), // empty
_tiebfail(0)
{
    // give the vector their definitive size (to use as arrays)
    _psb_partial.assign(_index.size(), nullptr);
    _psb_total.assign(_index.size(), nullptr);
    _local.assign(_index.size(), TonIndex::UNDEF);
    init();
}


PSV::~PSV()
{
    TRACE("delete PS Vector {}-{}", psenum().first(), psenum().stop());
    // implicit destructor for array _tons
    _psb_partial.clear(); // deallocate smart pointers
    _psb_total.clear();
}


PSEnum& PSV::psenum() const
{
    assert(_enum);
    return *_enum;
}


// compute _psb_partial
void PSV::init()
{
    for (size_t i = 0; i < _index.size(); ++i)
    {
        // PS Bag is empty if first() = last()
        TRACE("PSV {}-{} ton {}", psenum().first(), psenum().stop(), ton(i));
        const Ton& toni = ton(i);
        if (_algo == Algo::PSE)
        {
            // arg local ton is ignored
            _psb_partial[i] =
                std::make_shared<const PSB>(Algo::PSE0, psenum(), toni, toni);
        }
        else if (_algo == Algo::PS14)
        {
            // arg local ton is ignored
            _psb_partial[i] =
                std::make_shared<const PSB>(Algo::PS14, psenum(), toni, toni);
        }
        else
        {
            ERROR("PSV best: unexpected algo {}", _algo);
        }
        _psb_total[i] = nullptr;
        TRACE("compute the best spelling for notes {}-{}, ton = {}: {} accid",
              psenum().first(), psenum().stop(), ton(i),
              _psb_partial[i]->cost());
    }
}


const Ton& PSV::ton(size_t i) const
{
    assert(i < _index.size());
    return _index.ton(i);
}


//size_t PSV::nbtons() const
//{
//    return _tons.size();
//}


const PSB& PSV::best(size_t step, size_t i)
{
    assert(step == 0 || step == 1);
    assert(i < _index.size());

    if (step == 0)
    {
        assert(_psb_partial[i]);   // is a std::unique_ptr<const PSB>
        return *(_psb_partial[i]);
    }
    else if (step == 1)
    {
        assert(i < _local.size());

        if (_local[i] == TonIndex::UNDEF)
        {
            ERROR("PSV: best1 {} local ton for {} must be estimated", i, i);
            return best1ERROR(i);
        }
        else if (_local[i] == TonIndex::FAILED)
        {
            ERROR("PSV: best1 {}: estimation of local ton for {} failed", i, i);
            return best1ERROR(i);
        }

        if (_psb_total[i] == nullptr) // not tabulated yet
        {
            assert(i < _index.size());
            const Ton& gton = ton(i);
            assert(_local[i] < _index.size());
            const Ton& lton = ton(_local[i]);
            // PS Bag is empty if first() = last()
            /// @todo pass algo
            if (_algo == Algo::PSE)
            {
                _psb_total[i] =
                 std::make_shared<const PSB>(Algo::PSE1, psenum(), gton, lton);
            }
            else if (_algo == Algo::PS14)
            {
                _psb_total[i] =
                 std::make_shared<const PSB>(Algo::PS14, psenum(), gton, lton);
            }
            else
            {
                ERROR("PSV best: unexpected algo {}", _algo);
            }
        }
            
        assert(_psb_total[i]);
        return *(_psb_total[i]);
    }
    else // should not happen
    {
        ERROR("PSV best : unexpected step number {}", step);
        return best1ERROR(i);
    }
}


const PSB& PSV::best1ERROR(size_t i) const
{
    assert(_psb_partial[i]);  // is a std::unique_ptr<const PSB>
    return *(_psb_partial[i]);
}


size_t PSV::local(size_t i) const
{
    assert(i < _index.size());
    assert(i < _local.size());
    if (_local[i] == TonIndex::UNDEF)
    {
        ERROR("PSV: local ton must be estimated before accessed");
    }
    else if (_local[i] == TonIndex::FAILED)
    {
        ERROR("PSV: estimation of local ton failed");
    }
    else
    {
        assert(_local[i] < _index.size());
    }
    return _local[i];
}


// compute the table _local for first column
bool PSV::estimateLocal(size_t ig)
{
    assert(ig < _local.size());
    if (_local[ig] != TonIndex::UNDEF)
    {
        ERROR("PSV: re-estimation of local tons, ignore.");
        return false;
    }

    return estimateLocal(ig, ig);
}


// compute the table _local given previous column
bool PSV::estimateLocal(size_t ig, const PSV& prev)
{
    assert(ig < _local.size());
    if (_local[ig] != TonIndex::UNDEF)
    {
        ERROR("PSV: re-estimation of local tons, ignore.");
        return false;
    }

    size_t iprev = prev._local[ig];
    assert(iprev != TonIndex::UNDEF);
    // propagate failure of prev.
    if (iprev == TonIndex::FAILED)
    {
        _local[ig] = TonIndex::FAILED;
        return false;
    }
    // estimate local from iprev
    else
    {
        assert(iprev < _index.size());
        return estimateLocal(ig, iprev);
    }
}


// compute _local[ig] where ig is a global ton. index
bool PSV::estimateLocal(size_t ig, size_t iprev)
{
    assert(ig < _local.size());
    // already estimated (and failed): ignore
    if (_local[ig] == TonIndex::FAILED)
    {
        WARN("PSV: local tonality for {} alreadu estimated, and failed. ignored.", ig);
        return false;
    }
    // already estimated (and not failed): ignore
    else if (_local[ig] != TonIndex::UNDEF)
    {
        WARN("PSV: local tonality for {} alreadu estimated. ignored.", ig);
        return false;
    }
    
    assert(iprev < _index.size());
    // empty input.
    if (psenum().empty())
    {
        TRACE("PSV: estimate local for {}: empty vector {}-{}, keeping previous {}",
              ig, psenum().first(), psenum().stop(), iprev);
        // we stay in the previous local tonality
        _local[ig] = iprev;
        return true;
    }
    
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());
    
    const Ton& pton = ton(iprev);
    const Ton& gton = ton(ig);

    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.

    // cost for the current best local tonality.
    PSCost cbest;            // WARNING: initialized to 0.
    
    // current best distance to previous local ton. for ig
    unsigned int dbest = -1; // initialized to avoid warning

    // current best distance to previous global ton. ig
    unsigned int dgbest = -1; // initialized to avoid warning

    for (size_t j = 0; j < _index.size(); ++j)
    {
        const Ton& jton = ton(j);
        assert(_psb_partial[j]);
        const PSB& psb = *(_psb_partial[j]);
        assert(! psb.empty());
        // occurs iff first() == last(), and in this case all the bags are empty.
        PSCost cost = psb.cost();
        
        // new best cost
        if ((j == 0) || (cost < cbest)) // cbest = 0 when j = 0
        {
            ibest = j;
            cbest = cost;
            dbest = pton.distDiatonic(jton);
            dgbest = gton.distDiatonic(jton);
        }
        // tie break
        else if (cost == cbest)
        {
            // criteria 1:
            // best distance (of current tonality j) to the previous local tonality for ig
            unsigned int dist = pton.distDiatonic(jton);
            if (dist < dbest)
            {
                ibest = j;
                dbest = dist;
                continue;
            }
            // criteria 2:
            // best distance (of current tonality j) to the global tonality ig
            else if (dist == dbest)
            {
                // const Ton& toni = ton(i);
                // const Ton& tonbest = ton(ibest);
                unsigned int distg = gton.distDiatonic(jton);
                if (distg < dgbest)
                {
                    ibest = j;
                    dgbest = distg;
                }
                // criteria 3:
                // smallest of key signature
                // ALT: best distance between the previous local tonality and
                // a config in bag for the current tonality j
                else if (distg == dgbest)
                {
                    const Ton& bton = ton(ibest);
                    if (std::abs(jton.fifths()) < std::abs(bton.fifths()))
                    {
                        ibest = j;
                    }
                    else if (std::abs(jton.fifths()) == std::abs(bton.fifths()))
                    {
                        WARN("PSV {}-{}, estimation locals, tie break fail {} vs {}, cost=[{}],   prevton={}, (dist prev={}, dist global({})={})",
                             psenum().first(), psenum().stop(),
                             ton(j), ton(ibest), cost, pton, dist, gton, distg);
                        _tiebfail++;
                    }
                    // otherwise keep the current best
                    else
                    {
                        assert(std::abs(jton.fifths()) > std::abs(bton.fifths()));
                    }
                }
                // otherwise keep the current best
                else
                {
                    assert(distg > dgbest); // we did not forget a case
                }
            }
            // otherwisse keep the current best
            else
            {
                assert(dist > dbest); // we did not forget a case
            }
        }
        // otherwisse keep the current best
        else
        {
            assert(cost > cbest); // we did not forget a case
        }
    }

    assert(ibest != TonIndex::UNDEF);
    assert(ibest != TonIndex::FAILED);
    assert(ibest < _index.size());
    _local[ig] = ibest;
    // _estimated = true;
    return true;
}


//const PSP* PSV::path(int ton) const
//{
//    assert(-7 <= ton);
//    assert(ton <= 7);
//    return _tons[ton+7];
//}


//void PSV::init()
//{
//    if (first() > last()) // empty sequence of notes
//    {
//        _tons.fill(nullptr);
//        return;
//    }
//
//    for (int ton = -7; ton <= 7; ++ton)
//    {
//        TRACE("compute the best spelling for notes {}-{}, ton={}",
//              first(), last(), ton);
//        PSP* ps = new PSP(ton, _enum, first(), last());
//        TRACE("cost={} the best spelling notes {}-{} ton={}",
//              ps->cost(), first(), last(), ton);
//        _tons[ton+7] = ps;
//    }
//}


bool PSV::rename(size_t i)
{
    // bool status = true;
    const PSB& psb = best(1, i);
    if (psb.empty() && (! psenum().empty()))
    {
        ERROR("PST rename {}-{}: no best path found",
              psenum().first(), psenum().stop());
        return false;
    }
    // warning: ambiguity (tie break fail)
    else if (psb.size() > 1)
    {
        std::stringstream sms;
        sms << '[';
        psb.cost().print(sms);
        sms << ']' << ' ';
        for (auto it = psb.cbegin(); it != psb.cend(); ++it)
        {
            assert(*it); // std::shared_ptr<const PSC0>
            (*it)->cost().print(sms);
            sms << ' ';
        }
        _tiebfail += 1;
        WARN("PSV {}-{}: tie break fail ({} bests): {}",
             psenum().first(), psenum().stop(), psb.size(), sms.str());
        // DEBUGU("PSV {}-{}: tie break fail {}", sms.str());
    }
    
    // rename notes in this vector (if any)
    if (! _enum->empty())
    {
        const PSC0& psc = psb.top();
        PSP psp(psc, psenum());
        //std::cout << "PSV: rename path" << std::endl;
        psp.rename();
    }
    return true;
}

} // end namespace pse

/// @}

