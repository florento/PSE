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


PSV::PSV(const Algo& a, const Cost& seed, const TonIndex& i,
         const PSEnum& e, size_t bar):
_index(i),
_algo(a),
_enum(e.clone()),
_bar(bar),
_psbs(i.size(), nullptr),
//_psb_total(), // TBR
//_locals(i.size(), TonIndex::UNDEF),
//_local_cands(), // emptyset
_tiebfail(0)
{
    //_psbs.assign(_index.size(), nullptr);
    //_psb_total.assign(_index.size(), nullptr);
    //_local.assign(_index.size(), TonIndex::UNDEF);
    init_psbs(seed);
}


PSV::PSV(const Algo& a, const Cost& seed, const TonIndex& i,
         const PSEnum& e, size_t i0, size_t i1, size_t bar):
_index(i),
_algo(a),
_enum(e.clone(i0, i1)),
_bar(bar),
_psbs(i.size(), nullptr),
//_psb_total(), // TBR
//_locals(i.size(), TonIndex::UNDEF),
//_local_cands(), // emptyset
_tiebfail(0)
{
    // give the vector their definitive size (to use as arrays)
    //_psbs.assign(_index.size(), nullptr);
    //_psb_total.assign(_index.size(), nullptr);
    //_local.assign(_index.size(), TonIndex::UNDEF);
    init_psbs(seed);
}


PSV::PSV(const Algo& a, const Cost& seed, const TonIndex& i,
         const PSEnum& e, size_t i0, size_t bar):
_index(i),
_algo(a),
_enum(e.clone(i0)),
_bar(bar),
_psbs(i.size(), nullptr),
//_psb_total(), // TBR
//_locals(i.size(), TonIndex::UNDEF),
//_local_cands(), // emptyset
_tiebfail(0)
{
    // give the vector their definitive size (to use as arrays)
    //_psbs.assign(_index.size(), nullptr);
    //_psb_total.assign(_index.size(), nullptr);
    //_local.assign(_index.size(), TonIndex::UNDEF);
    init_psbs(seed);
}


PSV::PSV(const PSV& col, const Cost& seed,
         const PSO& globals, const std::vector<size_t>& locals):
_index(col._index),
_algo(col._algo),
_enum(col._enum->clone()),
_bar(col.bar()),
_psbs(_index.size(), nullptr),
_tiebfail(0)
{
    assert(col.size() == locals.size());
    init_psbs(seed, globals, locals);
}


PSV::~PSV()
{
    TRACE("delete PS Vector {}-{}", enumerator().first(), enumerator().stop());
    // implicit destructor for array _tons
    _psbs.clear(); // deallocate smart pointers
    //_psb_total.clear();
}


PSEnum& PSV::enumerator() const
{
    assert(_enum);
    return *_enum;
}


size_t PSV::bar() const
{
    return _bar;
}


const Ton& PSV::ton(size_t i) const
{
    assert(i < _index.size());
    return _index.ton(i);
}


size_t PSV::size() const
{
    return _psbs.size();
}


const PSB& PSV::bag(size_t i) const
{
    assert(i < _psbs.size());
    assert(_psbs.at(i) != nullptr);
    return *(_psbs.at(i));
}


// TBR
//const PSB& PSV::best(size_t step, size_t i)
//{
//    assert(step == 0 || step == 1);
//    assert(i < _index.size());
//
//    if (step == 0)
//    {
//        assert(_psbs[i]);   // is a std::shared_ptr<const PSB>
//        return *(_psbs[i]);
//    }
//    else if (step == 1)
//    {
//        assert(i < _locals.size());
//
//        if (_locals[i] == TonIndex::UNDEF)
//        {
//            ERROR("PSV: best1 {} local ton for {} must be estimated", i, i);
//            return best1ERROR(i);
//        }
//        else if (_locals[i] == TonIndex::FAILED)
//        {
//            ERROR("PSV: best1 {}: estimation of local ton for {} failed", i, i);
//            return best1ERROR(i);
//        }
//
//        if (_psb_total[i] == nullptr) // not tabulated yet
//        {
//            assert(i < _index.size());
//            const Ton& gton = ton(i);
//            assert(_locals[i] < _index.size());
//            const Ton& lton = ton(_locals[i]);
//            // PS Bag is empty if first() = last()
//            /// @todo pass algo
//            if (_algo == Algo::PSE)
//            {
//                _psb_total[i] =
//                 std::make_shared<const PSB>(Algo::PSE1, enumerator(),
//                                             gton, lton);
//            }
//            else if (_algo == Algo::PS14)
//            {
//                _psb_total[i] =
//                 std::make_shared<const PSB>(Algo::PS14, enumerator(),
//                                             gton, lton);
//            }
//            else
//            {
//                ERROR("PSV best: unexpected algo {}", _algo);
//            }
//        }
//
//        assert(_psb_total[i]);
//        return *(_psb_total[i]);
//    }
//    else // should not happen
//    {
//        ERROR("PSV best : unexpected step number {}", step);
//        return best1ERROR(i);
//    }
//}


// compute _psbs
void PSV::init_psbs(const Cost& seed)
{
    for (size_t i = 0; i < _index.size(); ++i)
    {
        // PS Bag is empty if first() = last()
        TRACE("PSV {}-{} ton {}",
              enumerator().first(), enumerator().stop(), ton(i));
        assert(_psbs[i] == nullptr);
        const Ton& toni = ton(i);
        if (_algo == Algo::PSE)
        {
            // arg local ton is ignored
            _psbs[i] =
             std::make_shared<const PSB>(Algo::PSE, seed, enumerator(), toni);
        }
        else if (_algo == Algo::PS14)
        {
            // arg local ton is ignored
            _psbs[i] =
             std::make_shared<const PSB>(Algo::PS14, seed, enumerator(), toni);
        }
        else
        {
            ERROR("PSV best: unexpected algo {}", _algo);
        }
        assert(_psbs[i]);
        // _psb_total[i] = nullptr;
        TRACE("compute the best spelling for notes {}-{}, ton = {}",
              enumerator().first(), enumerator().stop(), ton(i));
        // "{} accid", _psbs[i]->cost()
    }
}

// compute _psbs with given local tons
void PSV::init_psbs(const Cost& seed,
                    const PSO& globals, const std::vector<size_t>& locals)
{
    assert(locals.size() == _index.size());
    
    for (auto it = globals.cbegin(); it != globals.cend(); ++it)
    {
        size_t i = *it;
        // PS Bag is empty if first() = last()
        TRACE("PSV {}-{} ton {}",
              enumerator().first(), enumerator().stop(), ton(i));
        assert(i < _psbs.size());
        assert(_psbs[i] == nullptr);
        assert(i < _index.size());
        const Ton& toni = ton(i);
        assert(toni.defined());
        assert(locals.at(i) != TonIndex::FAILED);
        assert(locals.at(i) != TonIndex::UNDEF);
        assert(locals.at(i) < _index.size());
        const Ton& ltoni = ton(locals.at(i));
        assert(ltoni.defined());
        if (_algo == Algo::PSE)
        {
            // arg local ton is ignored
            _psbs[i] =
                std::make_shared<const PSB>(Algo::PSE, seed, enumerator(),
                                            toni, ltoni);
        }
        else if (_algo == Algo::PS14)
        {
            // arg local ton is ignored
            _psbs[i] =
                std::make_shared<const PSB>(Algo::PS14, seed, enumerator(),
                                            toni, ltoni);
        }
        else
        {
            ERROR("PSV best: unexpected algo {}", _algo);
        }
        assert(_psbs[i]);
        TRACE("compute the best spelling for notes {}-{}, ton = {}",
              enumerator().first(), enumerator().stop(), ton(i));
        // "{} accid", _psbs[i]->cost()
    }
}


//size_t PSV::ilocal(size_t ig) const
//{
//    assert(ig < _index.size());
//    assert(ig < _locals.size());
//    if (_locals[ig] == TonIndex::UNDEF)
//    {
//        ERROR("PSV: local ton for {} must be estimated before accessed", ig);
//    }
//    else if (_locals[ig] == TonIndex::FAILED)
//    {
//        WARN("PSV: estimation of local ton failed");
//    }
//    else
//    {
//        assert(_locals[ig] < _index.size());
//    }
//    return _locals[ig];
//}


//bool PSV::estimateLocals()
//{
//    assert(_locals.size() == _index.size());
//    bool status = true;
//    for (size_t i = 0; i < _locals.size(); ++i)
//        status = status && estimateLocal(i);
//    return status;
//}


// compute the table _local for first column
//bool PSV::estimateLocal(size_t ig)
//{
//    assert(ig < _locals.size());
//    if (_locals[ig] != TonIndex::UNDEF)
//    {
//        ERROR("PSV: re-estimation of local tons, ignore.");
//        return false;
//    }
//
//    // first measure: the previous "local" ton is the assumed global ton ig.
//    return estimateLocal(ig, ig);
//}


//bool PSV::estimateLocals(const PSV& prev)
//{
//    assert(_locals.size() == _index.size());
//    bool status = true;
//    for (size_t i = 0; i < _locals.size(); ++i)
//        status = status && estimateLocal(i, prev);
//    return status;
//}


// compute the table _local given previous column
//bool PSV::estimateLocal(size_t ig, const PSV& prev)
//{
//    assert(ig < _locals.size());
//    if (_locals[ig] != TonIndex::UNDEF)
//    {
//        ERROR("PSV: re-estimation of local tons, ignore.");
//        return false;
//    }
//
//    size_t iprev = prev._locals[ig];
//    assert(iprev != TonIndex::UNDEF);
//
//    // propagate failure of prev.
//    if (iprev == TonIndex::FAILED)
//    {
//        _locals[ig] = TonIndex::FAILED;
//        return false;
//    }
//    // estimate local from iprev
//    else
//    {
//        assert(iprev < _index.size());
//        return estimateLocal(ig, iprev);
//    }
//}


// compute _local[ig] where ig is a global ton. index
//bool PSV::estimateLocal(size_t ig, size_t iprev)
//{
//    assert(ig < _locals.size());
//    // already estimated (and failed): ignore
//    if (_locals[ig] == TonIndex::FAILED)
//    {
//        WARN("PSV: local tonality for {} alreadu estimated, and failed. ignored.", ig);
//        return false;
//    }
//    // already estimated (and not failed): ignore
//    else if (_locals[ig] != TonIndex::UNDEF)
//    {
//        WARN("PSV: local tonality for {} alreadu estimated. ignored.", ig);
//        return false;
//    }
//
//    assert(iprev < _index.size());
//    // empty input.
//    if (enumerator().empty())
//    {
//        TRACE("PSV: estimate local for {}: empty vector {}-{}, keeping previous {}",
//              ig, enumerator().first(), enumerator().stop(), iprev);
//        // we stay in the previous local tonality
//        _locals[ig] = iprev;
//        return true;
//    }
//
//    // estimation of best candidate locals (tie)
//    if (_local_cands.empty())
//        init_local_cands();
//    assert(! _local_cands.empty());
//
//    assert(iprev != TonIndex::UNDEF);
//    assert(iprev != TonIndex::FAILED);
//    assert(iprev < _index.size());
//
//    const Ton& pton = ton(iprev);
//    const Ton& gton = ton(ig);
//
//    // index of the current best local tonality.
//    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.
//
//    // cost for the current best local tonality.
//    PSCost cbest;  // WARNING: initialized to 0.
//
//    // current best distance to previous local ton. for ig
//    unsigned int dbest = -1; // initialized to avoid warning
//
//    // current best distance to previous global ton. ig
//    unsigned int dgbest = -1; // initialized to avoid warning
//
//    for (size_t j : _local_cands)
//    {
//        const Ton& jton = ton(j);
//        assert(_psbs[j]);
//        const PSB& psb = *(_psbs[j]);
//        assert(! psb.empty());
//        // occurs iff first() == last(), and in this case all the bags are empty.
//        PSCost cost = psb.cost();
//
//        // tie break criteria 1:
//        // best distance (of current tonality j) to the previous local tonality for ig
//        unsigned int dist = pton.distDiatonic(jton);
//        if (dist < dbest)
//        {
//            ibest = j;
//            dbest = dist;
//            continue;
//        }
//        // tie break criteria 2:
//        // best distance (of current tonality j) to the global tonality ig
//        else if (dist == dbest)
//        {
//            unsigned int distg = gton.distDiatonic(jton);
//            if (distg < dgbest)
//            {
//                ibest = j;
//                dgbest = distg;
//            }
//            // tie break criteria 3:
//            // smallest key signature
//            // ALT: best distance between the previous local tonality and
//            // a config in bag for the current tonality j
//            else if (distg == dgbest)
//            {
//                const Ton& bton = ton(ibest);
//                if (std::abs(jton.fifths()) < std::abs(bton.fifths()))
//                {
//                    ibest = j;
//                }
//                else if (std::abs(jton.fifths()) == std::abs(bton.fifths()))
//                {
//                    WARN("PSV {}-{}, estimation locals, tie break fail {} vs {}, cost=[{}],   prevton={}, (dist prev={}, dist global({})={})",
//                         enumerator().first(), enumerator().stop(),
//                         ton(j), ton(ibest), cost, pton, dist, gton, distg);
//                    _tiebfail++;
//                }
//                // otherwise keep the current best
//                else
//                {
//                    assert(std::abs(jton.fifths()) > std::abs(bton.fifths()));
//                }
//            }
//            // otherwise keep the current best
//            else
//            {
//                assert(distg > dgbest); // we did not forget a case
//            }
//        }
//        // otherwisse keep the current best
//        else
//        {
//            assert(dist > dbest); // we did not forget a case
//        }
//    }
//
//    assert(ibest != TonIndex::UNDEF);
//    assert(ibest != TonIndex::FAILED);
//    assert(ibest < _index.size());
//    _locals[ig] = ibest;
//    return true;
//}


// compute _local_cands
//void PSV::init_local_cands()
//{
//    assert(_local_cands.empty());
//
//    // index of the current best local tonality.
//    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.
//
//    // cost for the current best local tonality.
//    PSCost cbest;            // WARNING: initialized to 0.
//
//    for (size_t j = 0; j < _index.size(); ++j)
//    {
//        // const Ton& jton = ton(j);
//        assert(_psbs[j]);
//        const PSB& psb = *(_psbs[j]);
//        assert(! psb.empty());
//
//        // occurs iff first() == last(), and in this case all the bags are empty.
//        PSCost cost = psb.cost();
//
//        // new best cost
//        if ((j == 0) || (cost < cbest)) // cbest = 0 when j = 0
//        {
//            ibest = j;
//            cbest = cost;
//            _local_cands.clear();
//            auto ret = _local_cands.insert(j);
//            assert(ret.second == true); // j was inserted
//        }
//        // tie break
//        else if (cost == cbest)
//        {
//            auto ret = _local_cands.insert(j);
//            assert(ret.second == true); // j was inserted
//        }
//        // otherwisse keep the current best
//        else
//        {
//            assert(cost > cbest); // we did not forget a case
//        }
//    }
//    assert(! _local_cands.empty());
//}


const PSB& PSV::best1ERROR(size_t i) const
{
    assert(_psbs[i]);  // is a std::unique_ptr<const PSB>
    return *(_psbs[i]);
}



//// compute _local[ig] where ig is a global ton. index
//bool PSV::estimateLocal(size_t ig, size_t iprev)
//{
//    assert(ig < _locals.size());
//    // already estimated (and failed): ignore
//    if (_locals[ig] == TonIndex::FAILED)
//    {
//        WARN("PSV: local tonality for {} alreadu estimated, and failed. ignored.", ig);
//        return false;
//    }
//    // already estimated (and not failed): ignore
//    else if (_locals[ig] != TonIndex::UNDEF)
//    {
//        WARN("PSV: local tonality for {} alreadu estimated. ignored.", ig);
//        return false;
//    }
//
//    assert(iprev < _index.size());
//    // empty input.
//    if (enumerator().empty())
//    {
//        TRACE("PSV: estimate local for {}: empty vector {}-{}, keeping previous {}",
//              ig, enumerator().first(), enumerator().stop(), iprev);
//        // we stay in the previous local tonality
//        _locals[ig] = iprev;
//        return true;
//    }
//
//    assert(iprev != TonIndex::UNDEF);
//    assert(iprev != TonIndex::FAILED);
//    assert(iprev < _index.size());
//
//    const Ton& pton = ton(iprev);
//    const Ton& gton = ton(ig);
//
//    // index of the current best local tonality.
//    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.
//
//    // cost for the current best local tonality.
//    PSCost cbest;            // WARNING: initialized to 0.
//
//    // current best distance to previous local ton. for ig
//    unsigned int dbest = -1; // initialized to avoid warning
//
//    // current best distance to previous global ton. ig
//    unsigned int dgbest = -1; // initialized to avoid warning
//
//    for (size_t j = 0; j < _index.size(); ++j)
//    {
//        const Ton& jton = ton(j);
//        assert(_psbs[j]);
//        const PSB& psb = *(_psbs[j]);
//        assert(! psb.empty());
//        // occurs iff first() == last(), and in this case all the bags are empty.
//        PSCost cost = psb.cost();
//
//        // new best cost
//        if ((j == 0) || (cost < cbest)) // cbest = 0 when j = 0
//        {
//            ibest = j;
//            cbest = cost;
//            dbest = pton.distDiatonic(jton);
//            dgbest = gton.distDiatonic(jton);
//        }
//        // tie break
//        else if (cost == cbest)
//        {
//            // criteria 1:
//            // best distance (of current tonality j) to the previous local tonality for ig
//            unsigned int dist = pton.distDiatonic(jton);
//            if (dist < dbest)
//            {
//                ibest = j;
//                dbest = dist;
//                continue;
//            }
//            // criteria 2:
//            // best distance (of current tonality j) to the global tonality ig
//            else if (dist == dbest)
//            {
//                // const Ton& toni = ton(i);
//                // const Ton& tonbest = ton(ibest);
//                unsigned int distg = gton.distDiatonic(jton);
//                if (distg < dgbest)
//                {
//                    ibest = j;
//                    dgbest = distg;
//                }
//                // criteria 3:
//                // smallest key signature
//                // ALT: best distance between the previous local tonality and
//                // a config in bag for the current tonality j
//                else if (distg == dgbest)
//                {
//                    const Ton& bton = ton(ibest);
//                    if (std::abs(jton.fifths()) < std::abs(bton.fifths()))
//                    {
//                        ibest = j;
//                    }
//                    else if (std::abs(jton.fifths()) == std::abs(bton.fifths()))
//                    {
//                        WARN("PSV {}-{}, estimation locals, tie break fail {} vs {}, cost=[{}],   prevton={}, (dist prev={}, dist global({})={})",
//                             enumerator().first(), enumerator().stop(),
//                             ton(j), ton(ibest), cost, pton, dist, gton, distg);
//                        _tiebfail++;
//                    }
//                    // otherwise keep the current best
//                    else
//                    {
//                        assert(std::abs(jton.fifths()) > std::abs(bton.fifths()));
//                    }
//                }
//                // otherwise keep the current best
//                else
//                {
//                    assert(distg > dgbest); // we did not forget a case
//                }
//            }
//            // otherwisse keep the current best
//            else
//            {
//                assert(dist > dbest); // we did not forget a case
//            }
//        }
//        // otherwisse keep the current best
//        else
//        {
//            assert(cost > cbest); // we did not forget a case
//        }
//    }
//
//    assert(ibest != TonIndex::UNDEF);
//    assert(ibest != TonIndex::FAILED);
//    assert(ibest < _index.size());
//    _locals[ig] = ibest;
//    // _estimated = true;
//    return true;
//}


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

// mv to PSB
bool PSV::rename(size_t i)
{
    // bool status = true;
    const PSB& psb = bag(i);
    if (psb.empty() && (! enumerator().empty()))
    {
        ERROR("PST rename {}-{}: no best path found",
              enumerator().first(), enumerator().stop());
        return false;
    }
    // warning: ambiguity (tie break fail)
    else if (psb.size() > 1)
    {
        // std::stringstream sms;
        // sms << '[';
        // psb.cost().print(sms);
        // sms << ']' << ' ';
        // for (auto it = psb.cbegin(); it != psb.cend(); ++it)
        // {
        //    assert(*it); // std::shared_ptr<const PSC0>
        //    (*it)->cost().print(sms);
        //    sms << ' ';
        // }
        
        
        _tiebfail += 1;

        WARN("PSV rename bar:{}({}-{}): tie break fail ({} bests) cost[{}]",
             _bar, enumerator().first(), enumerator().stop(),
             psb.size(), psb.cost());

        size_t s = 0;
        for (auto it = psb.cbegin(); it != psb.cend(); ++it)
        {
            assert(*it); // std::shared_ptr<const PSC0>
            assert(_enum);
            const PSP p(**it, *_enum);
            WARN("spelling {}: {}", s, p);
            ++s;
        }
            
        // DEBUGU("PSV {}-{}: tie break fail {}", sms.str());
    }
    
    // rename notes in this vector (if any)
    if (! _enum->empty())
    {
        const PSC0& psc = psb.top();
        PSP psp(psc, enumerator());
        //std::cout << "PSV: rename path" << std::endl;
        psp.rename();
    }
    return true;
}

} // end namespace pse

/// @}

