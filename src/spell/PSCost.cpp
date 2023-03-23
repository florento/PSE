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


PSCost::PSCost(): // const CostOrdering& co
_accid(0),
_ndia(0),
_dist(0),
_color(0)
//_order(co)
{}


PSCost::PSCost(const PSCost& rhs):
_accid(rhs._accid),
_ndia(rhs._ndia),
_dist(rhs._dist),
_color(rhs._color)
//_order(rhs._order)
{}


//PSCost::PSCost(size_t a, size_t n, size_t d, size_t c):
//_accid(a),
//_dist(n),
//_ndia(d),
//_color(c)
//{ }


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
        //_order  = rhs._order;
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


PSCost PSCost::operator+(const PSCost& rhs) const
{
    PSCost copy(*this);
    return copy.operator+=(rhs);
}


PSCost operator+(const PSCost& c1, const PSCost& c2)
{
    return c1.operator+(c2);
}


//bool PSCost::operator==(const PSCost& rhs) const
//{
//    switch (_order)
//    {
//        case CostOrdering::Default:
//            return eq_lex(rhs);
//
//        case CostOrdering::Lex:
//            return eq_lex(rhs);
//
//        case CostOrdering::Cumul:
//            return eq_cumul(rhs);
//
//        case CostOrdering::Approx:
//            // pb: needs ar. base
//            return eq_approx(rhs);
//
//        case CostOrdering::Undef:
//        {
//            ERROR("PSCost: undef ordering code");
//            return false;
//        }
//
//        default:
//        {
//            ERROR("PSCost: unknown ordering code");
//            return false;
//        }
//    }
//}


bool PSCost::operator!=(const PSCost& rhs) const
{
    return !operator==(rhs);
}


//bool PSCost::operator<(const PSCost& rhs) const
//{
//    switch (_order)
//    {
//        case CostOrdering::Default:
//            return less_lex(rhs);
//
//        case CostOrdering::Lex:
//            return less_lex(rhs);
//
//        case CostOrdering::Cumul:
//            return less_cumul(rhs);
//
//        case CostOrdering::Approx:
//            // pb: needs ar. base
//            return less_approx(rhs);
//
//        case CostOrdering::Undef:
//        {
//            ERROR("PSCost: undef ordering code");
//            return false;
//        }
//
//        default:
//        {
//            ERROR("PSCost: unknown ordering code");
//            return false;
//        }
//    }
//}


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


// lexicographic ordering

bool PSCost::eq_lex(const PSCost& rhs) const
{
    return ((_accid == rhs._accid) &&
            (_ndia  == rhs._ndia)  &&
            (_dist  == rhs._dist)  &&
            (_color  == rhs._color));
}


bool PSCost::neq_lex(const PSCost& rhs) const
{
    return !eq_lex(rhs);
}


bool PSCost::less_lex(const PSCost& rhs) const
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


bool PSCost::leq_lex(const PSCost& rhs) const
{
    return !greater_lex(rhs);
}


bool PSCost::greater_lex(const PSCost& rhs) const
{
    return rhs.less_lex(*this);
}


bool PSCost::geq_lex(const PSCost& rhs) const
{
    return !less_lex(rhs);
}




// approx ordering

// default value of approx degree (static)
double PSCost::approx_degree = 0.05;


// static
double PSCost::getApproxDegree()
{
    return approx_degree;
}


// static
void PSCost::setApproxDegree(double d)
{
    if (d > 0)
        approx_degree = d;
    else
        ERROR("PSCost: Approx Degree must be > 0. keeping {}",
              approx_degree);
}


// static private
bool PSCost::approxeq(size_t a1, size_t a2, size_t base)
{
    assert(base > 0);
    double d = std::abs((double) a2 - (double) a1);
    return (d / (double) base < approx_degree);
}


bool PSCost::eq_approx(const PSCost& rhs, size_t base) const
{
    return (approxeq(_accid, rhs._accid, base) &&
            (_ndia  == rhs._ndia)  &&
            (_dist  == rhs._dist)  &&
            (_color == rhs._color));
}


bool PSCost::neq_approx(const PSCost& rhs, size_t base) const
{
    return !eq_approx(rhs, base);
}


/// @todo TBR
/// not correct when
/// lhs._accid < rhs._accid and lhs._accid approx_eq rhs._accid
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





// ordering based on cumul of criteria

bool PSCost::eq_cumul(const PSCost& rhs) const
{
    return ((_accid +_ndia == rhs._accid + rhs._ndia) &&
            (_dist  == rhs._dist)  &&
            (_color == rhs._color));
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


void PSCost::update(const PSC1& c, const PSEnum& e, const Ton& gton)
{
    // count the cost
    // bool cc = false;

    const NoteName& name = c.name();
    const Accid& accid = c.accidental();
    
    // update cost when accident for the name was updated
    // discount for lead degree
    if (c.printed() && !(gton.lead(name) && gton.accidDia(name) == accid))
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
    } // otherwise no previous note, _disj not updated
    
    // color of accident and color of global ton
    int ks = gton.fifths();
    const Accid& a = c.accidental();
    if (((ks >= 0) && (flat(a))) || ((ks <  0) && (sharp(a))))
    {
        _color += 1;
    }
}


void PSCost::update(const PSC1& c, const PSEnum& e,
                    const Ton& gton, const Ton& lton)
{
    // distance to conjectured local ton.
    _dist += c.state().dist(lton);
}


void PSCost::print(std::ostream& o) const
{
    o << "accid=" << _accid << ',';
    o << "disj=" << _ndia << ',';
    o << "dist=" << _dist << ',';
    o << "color=" << _color;
}


std::ostream& operator<<(std::ostream& o, const PSCost& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

/// @}
