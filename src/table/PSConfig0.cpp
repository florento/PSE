//
//  PSConfig0.cpp
//  qparse
//
//  Created by Florent Jacquemard on 15/02/2021.
//  Copyright © 2021 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#include <cmath>        // std::abs
#include <stack>

#include "PSConfig0.hpp"
//#include "PSConfig1.hpp"
//#include "PSConfig1c.hpp"
//#include "PSConfig2.hpp"
#include "PSState1.hpp"
#include "PSState2.hpp"
#include "Enharmonic.hpp"


namespace pse {


PSC0::PSC0(const Ton& ton, size_t id, const Cost& seed,
           bool tonal, bool octave):
_state(nullptr),
_id(id),
_cost(seed.shared_zero()) // zero
{
    if (octave)
    {
        _state = std::shared_ptr<PSState0>(new PSState2(ton, tonal));
    }
    else
    {
        _state = std::shared_ptr<PSState0>(new PSState1(ton, tonal));
    }
    
    assert(ton.getMode() != ModeName::Undef);
}


//PSC0::PSC0(const KeySig& ks, size_t i):
//_state(ks),
//_id(i),
//_pred(nullptr),
//_accidents(0),
//_dist(0)
//{ }
//
//
//PSC0::PSC0(const PSState& s, size_t i):
//_state(s), // copy
//_id(i),
//_pred(nullptr),
//_accidents(0),
//_dist(0)
//{ }


// deep copy
PSC0::PSC0(const PSC0& rhs):
_state(rhs._state->clone()),
_id(rhs._id),
_cost(rhs._cost->shared_clone())
{
    assert(_cost);
}


PSC0::~PSC0()
{
    TRACE("delete initial PSConfig0");
}


PSC0& PSC0::operator=(const PSC0& rhs)
{
    if (this != &rhs)
    {
        assert(rhs._state);
        _state = rhs._state->clone();
        _id    = rhs._id;
        _cost  = rhs._cost;  // copy
    }
    return *this;
}


bool PSC0::operator==(const PSC0& rhs) const
{
    assert(_state);
    assert(rhs._state);
    return (_id == rhs._id) and _state->equal(*(rhs._state));
}


bool PSC0::operator!=(const PSC0& rhs) const
{
    return (! operator==(rhs));
}


bool PSC0::initial() const
{
    return true;
}


bool PSC0::fromNote() const
{
    return false;
}


bool PSC0::fromChord() const
{
    return false;
}


bool PSC0::inChord() const
{
    return false;
}


const PSC0* PSC0::previous() const
{
    return nullptr;
}


const PSState0& PSC0::state() const
{
    assert(_state);
    return *_state;
}


size_t PSC0::id() const
{
    // ERROR("id(): should not be called for a PSC0");
    return _id;
}


const Cost& PSC0::cost() const
{
    assert(_cost);
    return *(_cost);
}


} // end namespace pse

/// @}



//const enum Accid PSC0::accidental(const enum NoteName&  name) const
//{
//    return _state.accid(name);
//}


//std::vector<std::shared_ptr<PSC0>>
//PSC0::successors(const Pitch& p, int ton) const
//{
//    unsigned int pm = p.midi();
//    assert(0 <= pm);
//    assert(pm <= 127);
//    size_t m = pm % 12; // chroma in 0..11
//    std::vector<std::shared_ptr<PSC0>> v =
//    std::vector<std::shared_ptr<PSC0>>();
//
//    for (size_t i = 0; i < 3; ++i)
//    {
//        int name = ENHARMONIC[m][i][0];
//        int accid = ENHARMONIC[m][i][1];
//        // case of 8 and (short list) 1, 3, 6, 10
//        if ((name != ENHARMONIC_UNDEF) && (accid != ENHARMONIC_UNDEF))
//            v.push_back(std::make_shared<PSC>(*this, name, accid, ton));
//    }
//
//    return v;
//}


// @todo mv to PSBag ?
//void PSC0::succ(PSEnum& e, const Ton& ton, PSCQueue& q) const
//{
//    if (e.simultaneous(_id))
//        succ2_TBR(e, ton, q);    // chord
//    else
//        succ1(e, ton, q);    // single note
//}
    
//void PSC0::succ1(PSEnum& e, const Ton& ton, PSCQueue& q) const
//{
//    // midi pitch of the note read for transition from this config
//    unsigned int pm = e.midipitch(_id);
//    assert(0 <= pm);
//    assert(pm <= 127);
//    int m = pm % 12; // chroma in 0..11
//
//    for (int j = 0; j < 3; ++j)
//    {
//        NoteName name = Enharmonics::name(m, j);
//        Accid accid = Enharmonics::accid(m, j);
//        // case of 8 and (short list) 1, 3, 6, 10
//        if (defined(name) && defined(accid))
//            q.push(std::make_shared<PSC1>(this, pm, name, accid, ton));
//    }
//}


//void PSC0::succ2_TBR(PSEnum& e, const Ton& ton, PSCQueue& q) const
//{
//    //PSChord c(e, _id);
//    //assert(c.size() > 1);
//    std::stack<std::shared_ptr<const PSC2>> cs;
//    cs.push(std::make_shared<const PSC2>(this, e, _id));     // initial config
//
//    while (! cs.empty())
//    {
//        std::shared_ptr<const PSC2> c = cs.top();
//        cs.pop();
//        assert(c);
//        if (c->complete())
//            q.push(c);
//        else
//        {
//            unsigned int m = c->current(); // chroma in 0..11
//            assert(0 <= m);
//            assert(m < 11);
//
//            // 3 enharmonics
//            for (int j = 0; j < 3; ++j)
//            {
//                NoteName name = Enharmonics::name(m, j);
//                Accid accid = Enharmonics::accid(m, j);
//                // case of 8 and (short list) 1, 3, 6, 10
//                if (defined(name) && defined(accid) &&
//                    c->consistent(name, accid))
//                    cs.push(std::make_shared<PSC2>(*c, name, accid, ton));
//            }
//        }
//    }
//}


// mv to PSBag ?
//void PSC0::succ(PSEnum& e, const Ton& ton, const Ton& lton, PSCQueue& q) const
//{
//    if (e.simultaneous(_id))
//        succ2_TBR(e, ton, lton, q);    // chord
//    else
//        succ1(e, ton, lton, q);    // single note
//}


//void PSC0::succ1(PSEnum& e, const Ton& ton, const Ton& lton, PSCQueue& q) const
//{
//    // midi pitch of the note read for transition from this config
//    unsigned int pm = e.midipitch(_id);
//    assert(0 <= pm);
//    assert(pm <= 127);
//    int m = pm % 12; // chroma in 0..11
//
//    for (int j = 0; j < 3; ++j)
//    {
//        NoteName name = Enharmonics::name(m, j);
//        Accid accid = Enharmonics::accid(m, j);
//        // case of 8 and (short list) 1, 3, 6, 10
//        if (defined(name) && defined(accid))
//            q.push(std::make_shared<PSC1>(this, pm, name, accid, ton, lton));
//    }
//}


//void PSC0::succ2_TBR(PSEnum& e, const Ton& ton, const Ton& lton, PSCQueue& q) const
//{
//    //PSChord c(e, _id);
//    //assert(c.size() > 1);
//    std::stack<std::shared_ptr<const PSC2>> cs;
//    cs.push(std::make_shared<const PSC2>(this, e, _id));     // initial config
//
//    while (! cs.empty())
//    {
//        std::shared_ptr<const PSC2> c = cs.top();
//        cs.pop();
//        assert(c);
//        if (c->complete())
//            q.push(c);
//        else
//        {
//            unsigned int m = c->current(); // chroma in 0..11
//            assert(0 <= m);
//            assert(m < 11);
//
//            // 3 enharmonics
//            for (int j = 0; j < 3; ++j)
//            {
//                NoteName name = Enharmonics::name(m, j);
//                Accid accid = Enharmonics::accid(m, j);
//                // case of 8 and (short list) 1, 3, 6, 10
//                if (defined(name) && defined(accid) &&
//                    c->consistent(name, accid))
//                    cs.push(std::make_shared<PSC2>(*c, name, accid, ton, lton));
//            }
//        }
//    }
//}


//std::stack<const PSC0*> PSC0::stack() const
//{
//    // initialy empty
//    std::stack<const PSC0*> res = std::stack<const PSC0*>();
//
//    for (const PSC0* p = this; p != nullptr; p = p->previous())
//    {
//        res.push(p);
//    }
//
//    return res;
//}


//const PSC0* PSC0::origin() const
//{
//    if (_pred == nullptr)
//        return this;
//    else
//        return _pred->origin();
//}
