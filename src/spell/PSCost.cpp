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



namespace pse {


PSCost::PSCost():
_accid(0),
_dist(0),
_disj(0),
_color(0)
{}


PSCost::PSCost(const PSCost& c):
_accid(c._accid),
_dist(c._dist),
_disj(c._disj),
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
        _disj  = rhs._disj;
        _color  = rhs._color;
    }
    return *this;
}


bool PSCost::operator==(const PSCost& rhs) const
{
    return ((_accid == rhs._accid) &&
            (_dist  == rhs._dist)  &&
            (_disj  == rhs._disj)  &&
            (_color  == rhs._color));
}


bool PSCost::operator!=(const PSCost& rhs) const
{
    return (! operator==(rhs));
}


bool PSCost::operator<(const PSCost& rhs) const
{
    if (_accid == rhs._accid)
    {
        if (_dist == rhs._dist)
        {
            if (_disj == rhs._disj)
            {
                return (_color < rhs._color);
            }
            else
                return (_disj < rhs._disj);
        }
        else
            return (_dist < rhs._dist);
    }
    else
        return (_accid < rhs._accid);
}


bool PSCost::operator>(const PSCost& rhs) const
{
    return (operator!=(rhs) && rhs.operator<(*this));
}


void PSCost::print(std::ostream& o) const
{
    o << "accid=" << _accid << ',';
    o << "dist=" << _dist << ',';
    o << "disj=" << _disj << ',';
    o << "color=" << _color;
}


std::ostream& operator<<(std::ostream& o, const PSCost& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

/// @}
