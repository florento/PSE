//
//  PSConfig1.cpp
//  qparse
//
//  Created by Florent Jacquemard on 14/03/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include <cmath>        // std::abs
#include "PSConfig1.hpp"


namespace pse {


// copy and update
// @todo TBR
PSC1::PSC1(std::shared_ptr<const PSC0> c, const PSEnum& e,
           const enum NoteName& name, const enum Accid& accid,
           const Ton& ton):
PSC(c),
_name(name),
_print(false)
{
    assert(c);
    _print = _state.update(name, accid);
    _id = c->id()+1; // next note in enum
    assert(_id <= e.stop());
    assert(defined(accid));
    _cost.update(*this, e, ton);
}


//PSC1::PSC1(const PSC0& c, const PSEnum& e,
//           const NoteName& name, const Accid& accid,
//           const Ton& ton):
//PSC(c),
//_name(name),
//_print(false)
//{
//    _print = _state.update(name, accid);
//    _id = c.id()+1; // next note in enum
//    assert(_id <= e.stop());
//    assert(defined(accid));
//    _cost.update(*this, e, ton);
//}


// @todo TBR
PSC1::PSC1(std::shared_ptr<const PSC0> c, const PSEnum& e,
           const enum NoteName& name, const enum Accid& accid,
           const Ton& ton, const Ton& lton):
PSC1(c, e, name, accid, ton)
{
    // complete the update
    _cost.update(*this, e, ton, lton);
}


//PSC1::PSC1(const PSC0& c, const PSEnum& e,
//           const NoteName& name, const Accid& accid,
//           const Ton& ton, const Ton& lton):
//PSC1(c, e, name, accid, ton)
//{
//    // complete the update
//    _cost.update(*this, e, ton, lton);
//}


// copy
PSC1::PSC1(const PSC1& c):
PSC(c),
_name(c._name),
_print(c._print)
{ }


PSC1::~PSC1()
{
    TRACE("delete PSConfig1 from note {}", _id);
    //    if (_pred) delete _pred;
}


PSC1& PSC1::operator=(const PSC1& rhs)
{
    if (this != &rhs)
    {
        PSC::operator=(rhs);
        //_midi        = rhs._midi;
        _name        = rhs._name;
        _print       = rhs._print;
    }
    return *this;
}


bool PSC1::operator==(const PSC1& rhs) const
{
    return (PSC::operator==(rhs) &&
            //(_midi == rhs._midi) &&
            (_name == rhs._name) &&
            (_print == rhs._print));

}


bool PSC1::operator!=(const PSC1& rhs) const
{
    return (! operator==(rhs));
}


//size_t PSC::id() const
//{
//    return _id;
//}


//unsigned int PSC1::midi() const
//{
//    return _midi;
//}


enum NoteName PSC1::name() const
{
    assert(defined(_name));
    return _name;
}


enum Accid PSC1::accidental() const
{
    enum Accid accid(_state.accid(_name)); // copy
    //assert(-2 <= toint(accid));
    //assert(toint(accid) <= 2);
    return accid; // cast to float format for Pitch ?
}


bool PSC1::printed() const
{
    return _print;
}


bool PSC1::fromNote() const
{
    return true;
}


bool PSC1::fromChord() const
{
    return false;
}


} // end namespace pse

/// @}
