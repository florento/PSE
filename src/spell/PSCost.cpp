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
#include <cmath>        // std::abs

namespace pse {


PSCost::PSCost():
_accid(0),
_ndia(0),
_dist(0),
_color(0)
{}


PSCost::PSCost(const PSCost& c):
_accid(c._accid),
_ndia(c._ndia),
_dist(c._dist),
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
        _ndia  = rhs._ndia;
        _dist  = rhs._dist;
        _color  = rhs._color;
    }
    return *this;
}


PSCost& PSCost::operator+=(const PSCost& rhs)
{
    _accid += rhs._accid;
    _ndia  += rhs._ndia;
    _dist  += rhs._dist;
    _color += rhs._color;
    return *this;
}


//PSCost::PSCost(size_t a, size_t n, size_t d, size_t c):
//_accid(a),
//_dist(n),
//_ndia(d),
//_color(c)
//{ }


PSCost PSCost::operator+(const PSCost& rhs) const
{
    PSCost copy(*this);
    return copy.operator+=(rhs);
}


bool PSCost::operator==(const PSCost& rhs) const
{
    return ((_accid == rhs._accid) &&
            (_ndia  == rhs._ndia)  &&
            (_dist  == rhs._dist)  &&
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
        if (_ndia == rhs._ndia)
        {
            if (_dist == rhs._dist)
            {
                return (_color < rhs._color);
            }
            else
                return (_dist < rhs._dist);
        }
        else
            return (_ndia < rhs._ndia);
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


// static private
bool PSCost::approxeq(size_t a1, size_t a2, size_t base)
{
    assert(base > 0);
    double d = std::abs((double) a2 - (double) a1);
    return (d / (double) base < 0.03);
}


bool PSCost::eq_approx(const PSCost& rhs, size_t base) const
{
    return (approxeq(_accid, rhs._accid, base) &&
            (_ndia  == rhs._ndia)  &&
            (_dist  == rhs._dist)  &&
            (_color  == rhs._color));
}


bool PSCost::neq_approx(const PSCost& rhs, size_t base) const
{
    return !eq_approx(rhs, base);
}


bool PSCost::less_approx(const PSCost& rhs, size_t base) const
{
    if (approxeq(_accid, rhs._accid, base))
    {
        if (_ndia == rhs._ndia)
        {
            if (_dist == rhs._dist)
            {
                return (_color < rhs._color);
            }
            else
                return (_dist < rhs._dist);
        }
        else
            return (_ndia < rhs._ndia);
    }
    else
        return (_accid < rhs._accid);
}


bool PSCost::leq_approx(const PSCost& rhs, size_t base) const
{
    return !greater_approx(rhs, base);
}


bool PSCost::greater_approx(const PSCost& rhs, size_t base) const
{
    return rhs.less_approx(*this, base);
}


bool PSCost::geq_approx(const PSCost& rhs, size_t base) const
{
    return !less_approx(rhs, base);
}







bool PSCost::eq_cumul(const PSCost& rhs) const
{
    return ((_accid+_ndia, rhs._accid + rhs._ndia) &&
            (_dist  == rhs._dist)  &&
            (_color  == rhs._color));
}


bool PSCost::neq_cumul(const PSCost& rhs) const
{
    return !eq_cumul(rhs);
}


bool PSCost::less_cumul(const PSCost& rhs) const
{
    if (_accid + _ndia == rhs._accid + rhs._ndia)
    {
        if (_dist == rhs._dist)
        {
            return (_color < rhs._color);
        }
        else
            return (_dist < rhs._dist);
    }
    else
        return (_accid + _ndia < rhs._accid + rhs._ndia);
}


bool PSCost::leq_cumul(const PSCost& rhs) const
{
    return !greater_cumul(rhs);
}


bool PSCost::greater_cumul(const PSCost& rhs) const
{
    return rhs.less_cumul(*this);
}


bool PSCost::geq_cumul(const PSCost& rhs) const
{
    return !less_cumul(rhs);
}




void PSCost::update(const PSC1& c, const PSEnum& e, const Ton& ton)
{
    // count the cost
    // bool cc = false;

    const NoteName& name = c.name();
    const Accid& accid = c.accidental();
    
    // update cost when accident for the name was updated
    // discount for lead degree
    
    // for min harm and min mel
    // count cost for a lead note if its accidental is not the one of the scale
//    if (ton.lead(name))
//    {
//        cc = (ton.accidDia(name) != accid);
//        // if (ton == Ton(-3, Ton::Mode::Min))
//          // DEBUGU("PSC: {} lead {}: {} != {}",
//          //        ton, name, ton.accidDia(name), accid);
//          // DEBUGU("PSC: {}, {}: {} {}",
//          //       ton, name, ((ton.lead(name))?"lead":"not lead"),
//          //       ((ton.accidDia(name) != accid)?"!=":"=="));
//    }
//    // otherwise, count a cost for every printed accidental
//    else
//        cc = c.printed();

    if ((ton.lead(name) && ton.accidDia(name) != accid) || c.printed())
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
    
    // non-diatonic move from conjoint previous note
    const PSC0* previous = c.previous();
    assert(previous);
    // c is successor of a single note and is not the last note of enum
    if (previous->fromNote() && (c.id() < e.stop()))
    {
        assert(! previous->initial());
        // note read for transition from c into next config
        unsigned int mp = e.midipitch(c.id());
        const PSC1* pc = dynamic_cast<const PSC1*>(previous);
        assert(pc);
        assert(e.inside(pc->id()));
        // previous note (before mp)
        // read for transition from previous config into c
        unsigned int pmp = e.midipitch(pc->id());
        assert(pc->id() == c.id()-1);
        int mdist = std::abs((int) mp - (int) pmp);
        // distance at most 1 ton
        if ((0 < mdist) && (mdist < 3) &&
            (! diatonicStep(pc->name(), c.name())))
        {
            _ndia += 1;
        }
    }
    // otherwise no previous note, _disj not updated
}


void PSCost::update(const PSC1& c, const PSEnum& e,
                    const Ton& ton, const Ton& lton)
{
    // assert(e.inside(c.id()));
    const Accid& accid = c.accidental();
    
    // distance to conjectured local ton.
    _dist += c.state().dist(lton);
    
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


PSCost operator+(const PSCost& c1, const PSCost& c2)
{
    return c1.operator+(c2);
}


std::ostream& operator<<(std::ostream& o, const PSCost& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

/// @}
