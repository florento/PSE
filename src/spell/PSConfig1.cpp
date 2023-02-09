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
PSC1::PSC1(const PSC0* c, unsigned int mp,
           const NoteName& name, const Accid& accid,
           const Ton& ton):
PSC(c),
_midi(mp),
_name(name),
_print(false)
{
    assert(c);
    _print = _state.update(name, accid);
    _id = c->id()+1; // next note in enum
    assert(defined(accid));

    // count the cost
    bool cc = false;

    // update cost when accident for the name was updated
    // discount for lead degree
    
    // for min harm and min mel
    // count cost for a lead note if its accidental is not the one of the scale
    if (ton.lead(name))
    {
        cc = (ton.accidDia(name) != accid);
        // if (ton == Ton(-3, Ton::Mode::Min))
          // DEBUGU("PSC: {} lead {}: {} != {}",
          //        ton, name, ton.accidDia(name), accid);
          // DEBUGU("PSC: {}, {}: {} {}",
          //       ton, name, ((ton.lead(name))?"lead":"not lead"),
          //       ((ton.accidDia(name) != accid)?"!=":"=="));
    }
    // otherwise, count cost for every printed accidental
    else
    {
        cc = _print;
    }

    if (cc)
    {
        // int a = toint(accid);
        // assert(-2 <= a);
        // assert(a <= 2);
        // if (accid == Accid::Natural) // natural
        //     _accidents += 1;
        // else        // single or double sharp or flat
        //     _accidents += std::abs(a);
        switch (accid)
        {
            case Accid::DoubleSharp:
            case Accid::DoubleFlat:
                _cost.incrAccid(2);
                break;

            case Accid::Sharp:
            case Accid::Flat:
                _cost.incrAccid(1);
                break;

            case Accid::Natural:
                _cost.incrAccid(1);
                break;

            default:
            {
                ERROR("PSC: unexpected accidental"); // accid
                break;
            }
        }
    }
}


PSC1::PSC1(const PSC0* c, unsigned int mp,
           const NoteName& name, const Accid& accid,
           const Ton& ton, const Ton& lton):
PSC1(c, mp, name, accid, ton)
{
    assert(c);
    // complete the update

    // distance to conjectured local ton.
    _cost.incrDist(_state.dist(lton));

    // disjoint move from previous note
    assert(previous());
    if (! previous()->initial())
    {
        const PSC1* pred = dynamic_cast<const PSC1*>(previous());
        assert(pred);
        // previous note (before mp)
        unsigned int pmp = pred->midi();
        if ((std::abs((int) mp - (int) pmp) < 5) &&
            (! diatonicStep(pred->name(), name)))
        {
            _cost.incrDisj(1);
        }
    }
    // otherwise no previous note, _disj not updated
    
    // color of accident and color of global ton
    if (((ton.fifths() >= 0) && (flat(accid))) ||
        ((ton.fifths() < 0) && (sharp(accid))))
    {
        _cost.incrColor(1);
    }
}


// copy
PSC1::PSC1(const PSC1& c):
PSC(c),
_midi(c._midi),
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
        _midi        = rhs._midi;
        _name        = rhs._name;
        _print       = rhs._print;
    }
    return *this;
}


bool PSC1::operator==(const PSC1& rhs) const
{
    return (PSC::operator==(rhs) &&
            (_midi == rhs._midi) &&
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


unsigned int PSC1::midi() const
{
    return _midi;
}


NoteName PSC1::name() const
{
    assert(defined(_name));
    return _name;
}


Accid PSC1::accidental() const
{
    Accid accid(_state.accid(_name)); // copy
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
