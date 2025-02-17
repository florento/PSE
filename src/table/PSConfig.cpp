//
//  PSConfig.cpp
//  qparse
//
//  Created by Florent Jacquemard on 14/03/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include <cmath>        // std::abs
#include "PSConfig.hpp"


namespace pse {


// copy and update
/// @todo TBR
PSC::PSC(std::shared_ptr<const PSC0> c):
PSC0(*c), // clone the state
_pred(c)  // shared ptr copy
{
    assert(_pred);
    assert(_pred->initial() || _pred->fromNote() || _pred->fromChord());
}


//PSC::PSC(const PSC0& c):
//PSC0(c),
//_pred(std::shared_ptr<const PSC0>(&c))
//{
//    assert(c.initial() || c.fromNote() || c.fromChord());
//}


// copy constructor
PSC::PSC(const PSC& c):
PSC0(c),        // clone the state
_pred(c._pred)  // shared ptr copy
{ }


PSC::~PSC()
{
    TRACE("delete PSConfig from note {}", _id);
    //    if (_pred) delete _pred;
}


PSC& PSC::operator=(const PSC& rhs)
{
    if (this != &rhs)
    {
        PSC0::operator=(rhs);
        _pred         = rhs._pred;
    }
    return *this;
}


bool PSC::operator==(const PSC& rhs) const
{
    return (PSC0::operator==(rhs));
    // do not compare pred.
}


bool PSC::operator!=(const PSC& rhs) const
{
    return (! operator==(rhs));
}


//size_t PSC::id() const
//{
//    return _id;
//}


bool PSC::initial() const
{
    return false;
}


const PSC0* PSC::previous() const
{
    //assert(_pred.get());
    assert(_pred);
    assert(_pred->initial() || _pred->fromNote() || _pred->fromChord());
    return _pred.get();
}


//void PSC::succ(PSEnum& _enum, const Ton& ton, PSCQueue& q) const
//{
//
//}


//void PSC::succ(PSEnum& _enum, const Ton& ton, const Ton& lton,
//          PSCQueue& q) const
//{
//
//}


// 1 for simple, 2 for double
//void PSC::updateAccidents()
//{
//    if (printed())
//        _accidents += std::abs(accidental());
//}


// distance to tonic
// TBR
//void PSC::updateDist(const Ton& ton)
//{
    //if (printed())
    //{
        //assert(-7 <= ton);
        //assert(ton <= 7);
        // int alt = accidental();
        //DEBUGU("Dist. of {}({}){} to {}: {}",
        //      pse::Pitch::name_of_int(_name), _name, alt, ton,
        //      KeySig::distTon(_name, alt, ton));
        /// @todo REVISE
        //_dist += KeySig::distTon(_name, alt, ton);
    //}
//    else
//        return 0;
    
//    assert(0 <= _name);
//    assert(_name <= 6);
//    int alt = alteration();
//    assert(-2 <= alt);
//    assert(alt <= 2);
//    // location of the note of this config in array of fifths
//    std::pair<int, int> p = std::make_pair(_name, alt);
//    const int& loc = FIFTHS.at(p);
//
//    // location of the tonic in array of fifths
//    int locton = 15 + ton;
//
//    return std::abs(loc - locton);
//}


} // end namespace pse

/// @}
