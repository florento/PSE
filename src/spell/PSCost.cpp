//
//  PSCost.cpp
//  pse
//
//  Created by Florent Jacquemard on 11/12/2022.
//
/// @addtogroup pitch
/// @{

#include <cmath>        // std::abs

#include "PSCost.hpp"
#include "PSConfig1.hpp"
#include "PSConfig2.hpp"


namespace pse {


PSCost::PSCost():
_accid(0),
_dist(0),
_ndia(0),
_color(0)
{}


PSCost::PSCost(const PSCost& c):
_accid(c._accid),
_dist(c._dist),
_ndia(c._ndia),
_color(c._color)
{}


PSCost::~PSCost()
{
    TRACE("delete PS Cost");
}


PSCost& PSCost::operator=(const PSCost& rhs)
{
    if (this != &rhs)
    {
        _accid = rhs._accid;
        _dist  = rhs._dist;
        _ndia  = rhs._ndia;
        _color  = rhs._color;
    }
    return *this;
}


bool PSCost::operator==(const PSCost& rhs) const
{
    return ((_accid == rhs._accid) &&
            (_dist  == rhs._dist)  &&
            (_ndia  == rhs._ndia)  &&
            (_color  == rhs._color));
}


bool PSCost::operator!=(const PSCost& rhs) const
{
    return !operator==(rhs);
}


bool PSCost::operator<(const PSCost& rhs) const
{
    if (_accid == rhs._accid)
    {
        if (_dist == rhs._dist)
        {
            if (_ndia == rhs._ndia)
            {
                return (_color < rhs._color);
            }
            else
                return (_ndia < rhs._ndia);
        }
        else
            return (_dist < rhs._dist);
    }
    else
        return (_accid < rhs._accid);
}


bool PSCost::operator<=(const PSCost& rhs) const
{
    return !operator>(rhs);
}


bool PSCost::operator>(const PSCost& rhs) const
{
//  return (operator!=(rhs) && rhs.operator<(*this));
    return rhs.operator<(*this);
}


bool PSCost::operator>=(const PSCost& rhs) const
{
    return !operator<(rhs);
}


void PSCost::update(const PSC1& c, const PSEnum& e, const Ton& ton)
{
    // count the cost
    bool cc = false;

    const NoteName& name = c.name();
    const Accid& accid = c.accidental();
    bool printed = c.printed();
    
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
    // otherwise, count a cost for every printed accidental
    else
    {
        cc = printed;
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
                _accid += 2;
                break;

            case Accid::Sharp:
            case Accid::Flat:
                _accid += 1;
                break;

            case Accid::Natural:
                _accid += 1;
                break;

            default:
            {
                ERROR("PSC: unexpected accidental"); // accid
                break;
            }
        }
    }
}


void PSCost::update(const PSC1& c, const PSEnum& e,
                    const Ton& ton, const Ton& lton)
{
    assert(e.inside(c.id()));
    unsigned int mp = e.midipitch(c.id()); // c.midi();
    const Accid& accid = c.accidental();
    
    // distance to conjectured local ton.
    _dist += c.state().dist(lton);

    // non-diatonic move from conjoint previous note
    const PSC0* previous = c.previous();
    assert(previous);
    if (previous->fromNote())
    {
        assert(! previous->initial());
        const PSC1* pc = dynamic_cast<const PSC1*>(previous);
        assert(pc);
        assert(e.inside(pc->id()));
        // previous note (before mp)
        unsigned int pmp = e.midipitch(pc->id());  // pc->midi();
        int mdist = std::abs((int) mp - (int) pmp);
        // distance at most 1 ton
        if ((0 < mdist) && (mdist < 3) &&
            (! diatonicStep(pc->name(), c.name())))
        {
            _ndia += 1;
        }
    }
    // otherwise no previous note, _disj not updated
    
    // color of accident and color of global ton
    if (((ton.fifths() >= 0) && (flat(accid))) ||
        ((ton.fifths() < 0) && (sharp(accid))))
    {
        _color += 1;
    }

}


void PSCost::print(std::ostream& o) const
{
    o << "accid=" << _accid << ',';
    o << "dist=" << _dist << ',';
    o << "disj=" << _ndia << ',';
    o << "color=" << _color;
}


std::ostream& operator<<(std::ostream& o, const PSCost& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

/// @}
