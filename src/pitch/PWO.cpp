//
//  PWO.cpp
//  pse
//
//  Created by Florent Jacquemard on 12/02/2023.
//

#include "PWO.hpp"

namespace pse {

PWO::PWO():
name(NoteName::Undef),
alteration(Accid::Undef)
{ }

PWO::PWO(const enum NoteName& n, const enum Accid& a):
name(n),
alteration(a)
{ }

PWO::PWO(const PWO& rhs):
name(rhs.name),
alteration(rhs.alteration)
{ }


PWO& PWO::operator=(const PWO& rhs)
{
    if(this != &rhs)
    {
        name = rhs.name;
        alteration = rhs.alteration;
    };
    return *this;
}


bool PWO::equal(const PWO& rhs) const
{
    return (name == rhs.name) && (alteration == rhs.alteration);
}


bool PWO::undef() const
{
    assert(name != NoteName::Undef || alteration == Accid::Undef);
    return (name == NoteName::Undef);
}

void PWO::print(std::ostream& o) const
{
    
}

bool operator==(const PWO& lhs, const PWO& rhs)
{
    return lhs.equal(rhs);
}

bool operator!=(const PWO& lhs, const PWO& rhs)
{
    return !operator==(lhs, rhs);
}

std::ostream& operator<<(std::ostream& o, const PWO& p)
{
    p.print(o);
    return o;
}



} // namespace pse
