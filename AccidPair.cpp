//
//  AccidPair.cpp
//  pse
//
//  Created by Florent Jacquemard on 19/03/2024.
//

#include "AccidPair.hpp"


namespace pse {

AccidPair::AccidPair(const enum Accid& a1, const enum Accid& a2):
_first(a1),
_second(a2)
{
    assert(a1 != Accid::Undef || a2 == Accid::Undef);
}


// default
AccidPair& AccidPair::operator=(const AccidPair& rhs)
{
    if (this != &rhs)
    {
        _first = rhs._first;
        _second = rhs._second;
    }
    return *this;
}


bool AccidPair::operator==(const AccidPair& rhs) const
{
    return (_first == rhs._first && _second == rhs._second);
}


bool AccidPair::operator!=(const AccidPair& rhs) const
{
    return (! operator==(rhs));
}


enum Accid AccidPair::first() const
{
    return _first;
}


enum Accid AccidPair::second() const
{
    return _second;
}


bool AccidPair::undef() const
{
    assert(_first != Accid::Undef || _second == Accid::Undef);
    return (_first == Accid::Undef);
}


bool AccidPair::defined() const
{
    return !undef();
}


bool AccidPair::single() const
{
    return (_first != Accid::Undef && _second == Accid::Undef);
}


bool AccidPair::pair() const
{
    return (_first != Accid::Undef && _second != Accid::Undef);
}


bool AccidPair::contains(const enum Accid& a) const
{
    assert(a != Accid::Undef);
    return (_first == a || _second == a);
}


void AccidPair::print(std::ostream& o) const
{
    if (undef())
        o << "UNDEF";
    else if (_second == Accid::Undef)
        o << _first;
    else
        o << "(" << _first << ", " << _second << ")";
}


std::ostream& operator<<(std::ostream& o, const AccidPair& p)
{
    p.print(o);
    return o;
}


} // end namespace pse
