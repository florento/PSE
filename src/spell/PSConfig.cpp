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
PSC::PSC(const PSC0& c, unsigned int mp,
         const NoteName& name, const Accid& accid,
         const Ton& ton):
PSC0(c),
_pred(&c),
_midi(mp),
_name(name),
_print(false)
{
    _print = _state.update(name, accid);
    _id = c.id()+1; // next note in enum
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


PSC::PSC(const PSC0& c, unsigned int mp,
         const NoteName& name, const Accid& accid,
         const Ton& ton, const Ton& lton):
PSC(c, mp, name, accid, ton)
{
    // complete the update

    // distance to conjectured local ton.
    _cost.incrDist(_state.dist(lton));

    // disjoint move from previous note
    assert(_pred);
    if (! _pred->initial())
    {
        const PSC* pred = dynamic_cast<const PSC*>(_pred);
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


PSC::PSC(const PSC& c):
PSC0(c),
_pred(c._pred),
_midi(c._midi),
_name(c._name),
_print(c._print)
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
        _pred      = rhs._pred;
        _midi        = rhs._midi;
        _name      = rhs._name;
        _print      = rhs._print;
    }
    return *this;
}


bool PSC::operator==(const PSC& rhs) const
{
    return (PSC0::operator==(rhs) &&
            (_midi == rhs._midi) &&
            (_name == rhs._name) &&
            (_print == rhs._print));

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
    return _pred;
}


unsigned int PSC::midi() const
{
    return _midi;
}


NoteName PSC::name() const
{
    assert(defined(_name));
    return _name;
}


Accid PSC::accidental() const
{
    Accid accid(_state.accid(_name)); // copy
    //assert(-2 <= toint(accid));
    //assert(toint(accid) <= 2);
    return accid; // cast to float format for Pitch ?
}


bool PSC::printed() const
{
    return _print;
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
        //      Pitch::name_of_int(_name), _name, alt, ton,
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
