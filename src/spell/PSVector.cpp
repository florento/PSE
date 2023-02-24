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


PSV::PSV(const TonIndex& i, const PSEnum& e):
index(i),
_enum(e.clone()),
_psb0(),
_psb1(),
_local(TonIndex::UNDEF), // undef (value out of range)
//_estimated(false),
_tiebfail(0)
{
    // _psb0.fill(nullptr);
    // _psb1.fill(nullptr);
    init();
}


PSV::PSV(const TonIndex& i, const PSEnum& e, size_t i0, size_t i1):
index(i),
_enum(e.clone(i0, i1)),
_psb0(),
_psb1(),
_local(TonIndex::UNDEF), // undef (value out of range)
//_estimated(false),
_tiebfail(0)
{
    // _psb0.fill(nullptr);
    // _psb1.fill(nullptr);
    init();
}


PSV::PSV(const TonIndex& i, const PSEnum& e, size_t i0):
index(i),
_enum(e.clone(i0)),
_psb0(),
_psb1(),
_local(TonIndex::UNDEF), // undef (value out of range)
//_estimated(false),
_tiebfail(0)
{
    // _psb0.fill(nullptr);
    // _psb1.fill(nullptr);
    init();
}


PSV::~PSV()
{
    TRACE("delete PS Vector {}-{}", psenum().first(), psenum().stop());
    // implicit destructor for array _tons
}


PSEnum& PSV::psenum() const
{
    assert(_enum);
    return *_enum;
}


// compute _psb0
void PSV::init()
{
    for (size_t i = 0; i < index.size(); ++i)
    {
        // PS Bag is empty if first() = last()
        TRACE("PSV {}-{} ton {}", psenum().first(), psenum().stop(), ton(i));
        _psb0[i] = std::make_unique<const PSB>(ton(i), psenum());
        _psb1[i] = nullptr; // std::make_shared<const PSB>(ton(i), psenum());
        TRACE("compute the best spelling for notes {}-{}, ton = {}: {} accid",
              psenum().first(), psenum().stop(), ton(i), _psb0[i]->cost());
    }
}


const Ton& PSV::ton(size_t i) const
{
    assert(i < index.size());
    return index.ton(i);
}


//size_t PSV::nbtons() const
//{
//    return _tons.size();
//}


const PSB& PSV::best0(size_t i) const
{
    assert(i < index.size());
    assert(_psb0[i]);   // is a std::unique_ptr<const PSB>
    return *(_psb0[i]);
}


const PSB& PSV::best1(size_t i)
{
    assert(i < index.size());
    
    if (_local == TonIndex::UNDEF)
    {
        ERROR("PSV: best1: local ton must be estimated");
        assert(_psb0[i]);   // is a std::unique_ptr<const PSB>
        return *(_psb0[i]);
    }
    else if (_local == TonIndex::FAILED)
    {
        ERROR("PSV: best1: estimation of local ton failed");
        assert(_psb0[i]);   // is a std::unique_ptr<const PSB>
        return *(_psb0[i]);
    }

    if (_psb1[i] == nullptr) // not tabulated yet
    {
        assert(_local < index.size());
        const Ton& lton = ton(_local);
        const Ton& gton = ton(i);
        // PS Bag is empty if first() = last()
        _psb1[i] = std::make_unique<const PSB>(gton, lton, psenum());
    }
        
    assert(_psb1[i]);
    return *(_psb1[i]);
}


size_t PSV::local() const
{
    if (_local == TonIndex::UNDEF)
    {
        ERROR("PSV: local ton must be estimated before accessed");
    }
    else if (_local == TonIndex::FAILED)
    {
        ERROR("PSV: estimation of local ton failed");
    }

    return _local;
}


// compute _local
bool PSV::estimateLocal(size_t prev)
{
    if (_local != TonIndex::UNDEF)
    {
        WARN("PSV: re-estimation of local tonality. ignored.");
        return true;
    }
    else if (_local != TonIndex::FAILED)
    {
        WARN("PSV: failure in estimation of local tonality.");
        return false;
    }
    
    if (psenum().first() == psenum().stop()) // psenum().empty
    {
        DEBUGU("PSV: estimate local: empty vector {}-{}",
              psenum().first(), psenum().stop());
        // we stay in the previous local tonality
        _local = prev;
        //_estimated = true;
        return true;
    }
    
    assert(prev < index.size());
    const Ton& pton = ton(prev);
    // const PSB& ppsb = *(_psb0[prev]);

    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.

    // cost for the current best local tonality.
    PSCost cbest; // initialized to avoid warning. // was -1
    
    // current best distance
    unsigned int dbest = -1; // initialized to avoid warning

    for (size_t i = 0; i < index.size(); ++i)
    {
        assert(_psb0[i]);
        const PSB& psb = *(_psb0[i]);
        // iff first() == last(), in this case all the bags are empty.
        assert(! psb.empty());

        PSCost cost = psb.cost();
        
        // new best cost
        if ((i == 0) || (cost < cbest))
        {
            ibest = i;
            cbest = cost;
            dbest = pton.distDiatonic(ton(i));
            continue;
        }
        // tie break
        else if (cost == cbest)
        {
            // criteria 1:
            // best distance between the previous local tonality and
            // the current tonality i
            unsigned int dist = pton.distDiatonic(ton(i));
            if (dist < dbest)
            {
                ibest = i;
                dbest = dist;
            }
            // criteria 2: none
            // ALT: best distance between the global tonality and
            // the current tonality i
            // ALT: best distance between the previous local tonality and
            // a config in bag for the current tonality i
            else if (dist == dbest)
            {
                 //const Ton& toni = ton(i);
                //const Ton& tonbest = ton(ibest);
                WARN("PSV {}-{}, estimation locals, tie break fail {} vs {}, cost=[{}], prevton={}, (dist: {} vs {})",
                     psenum().first(), psenum().stop(),
                     ton(i), ton(ibest), // ton(i).fifths(), ton(ibest).fifths(),
                     cost, pton, dist, dbest);
            }
            // otherwise keep the current best
        }
    }

    assert(ibest != TonIndex::UNDEF);
    assert(ibest != TonIndex::FAILED);
    assert(ibest < index.size());
    _local = ibest;
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
    const PSB& psb = best1(i);
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

