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


//const enum Accid PWO::_3F = Accid::TripleFlat;
//const enum Accid PWO::_2F = Accid::DoubleFlat;
//const enum Accid PWO::_1F = Accid::Flat;
//const enum Accid PWO::_0N = Accid::Natural;
//const enum Accid PWO::_1S = Accid::Sharp;
//const enum Accid PWO::_2S = Accid::DoubleSharp;
//const enum Accid PWO::_3S = Accid::TripleSharp;
//const enum Accid PWO::__U = Accid::Undef;
//
//// static
//const enum Accid PWO::ACCID[12][7] =
//{
////      C    D    E    F    G    A    B
//    { _0N, _2F, __U, __U, __U, __U, _1S }, //   0 = C
//    { _1S, _1F, _3F, __U, __U, __U, _2S  }, //  1 = C#, Db
//    { _2S, _0N, _2F, __U, __U, __U, __U  }, //  2 = D
//    { __U, _1S, _1F, _2F, __U, __U, __U  }, //  3 = D#, Eb
//    { __U, _2S, _0N, _1F, _3F, __U, __U  }, //  4 = E
//    { __U, _3S, _1S, _0N, _2F, __U, __U  }, //  5 = F
//    { __U, __U, __U, _1S, _1F, _3F, __U  }, //  6 = F#, Gb
//    { __U, __U, __U, _2S, _0N, _2F, __U  }, //  7 = G
//    { __U, __U, __U, __U, _1S, _1F, _3F  }, //  8 = G#, Ab
//    { __U, __U, __U, __U, _2S, _0N, _2F  }, //  9 = A
//    { _2F, __U, __U, __U, __U, _1S, _1F  }, // 10 = A#, Bb
//    { _1F, _3F, __U, __U, __U, _2S, _0N  }  // 11 = B
//};


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
