//
//  MidiNum.cpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "MidiNum.hpp"

namespace pse {

namespace MidiNum{

// static
int midi_to_octave(unsigned int m, const enum pse::NoteName& n)
{
    assert(0 <= m);
    assert(m <= 128);
    int oct = int(floor(m/12)) - 1;
    assert(-1 <= oct);
    assert(oct <= 9);
    int chroma = m % 12;
    // adjust octave for extreme notes
    if (chroma == 0 && n == pse::NoteName::B)
    {
        //assert(a == pse::Accid::Sharp);
        return oct - 1;
    }
    else if (chroma == 1 && n == pse::NoteName::B)
    {
        //assert(a == pse::Accid::DoubleSharp);
        return oct - 1;
    }
    else if (chroma == 10 && n == pse::NoteName::C)
    {
        //assert(a == pse::Accid::DoubleFlat);
        return oct + 1;
    }
    else if (chroma == 11 && n == pse::NoteName::C)
    {
        //assert(a == pse::Accid::Flat);
        return oct + 1;
    }
    else
    {
        // TBD assert the name // accid in the other cases
        return oct;
    }
}


// static
int midi_to_octave(unsigned int m,
                   const enum pse::NoteName& n, const enum pse::Accid& a)
{
    assert(0 <= m);
    assert(m <= 128);
    int oct = int(floor(m/12)) - 1;
    assert(-1 <= oct);
    assert(oct <= 9);
    int chroma = m % 12;
    // adjust octave for extreme notes
    if (chroma == 0 && n == pse::NoteName::B)
    {
        assert(a == pse::Accid::Sharp);
        return oct - 1;
    }
    else if (chroma == 1 && n == pse::NoteName::B)
    {
        assert(a == pse::Accid::DoubleSharp);
        return oct - 1;
    }
    else if (chroma == 10 && n == pse::NoteName::C)
    {
        assert(a == pse::Accid::DoubleFlat);
        return oct + 1;
    }
    else if (chroma == 11 && n == pse::NoteName::C)
    {
        assert(a == pse::Accid::Flat);
        return oct + 1;
    }
    else
    {
        // TBD assert the name // accid in the other cases
        return oct;
    }
}

enum Accid accid(int c, const enum NoteName& n)
{
    // pitch class for each note name in 0 (C) .. 7 (B)
    //                  C  D  E  F  G  A  B
    const int PC[7] = { 0, 2, 4, 5, 7, 9, 11 };

    assert(0 <= c);
    assert(c <= 11);
    int ni = toint(n);
    assert(0 <= ni);
    assert(ni <= 6);
    switch (c - PC[ni])
    {
        case -3:
            return Accid::TripleFlat;
        case -2:
            return Accid::DoubleFlat;
        case -1:
            return Accid::Flat;
        case  0:
            return Accid::Natural;
        case  1:
            return Accid::Sharp;
        case  2:
            return Accid::DoubleSharp;
        case  3:
            return Accid::TripleSharp;
        default:
            return Accid::Undef;
    }
}


unsigned int pitchClass(const enum NoteName& name)
{
    switch (name)
    {
        case NoteName::C:
            return 0;
        case NoteName::D:
            return 2;
        case NoteName::E:
            return 4;
        case NoteName::F:
            return 5;
        case NoteName::G:
            return 7;
        case NoteName::A:
            return 9;
        case NoteName::B:
            return 11;
        default:
        {
            ERROR("pitchClass: wrong note name {}", name);
            return 12;
        }
    }
}


// TBC
unsigned int to_midi(const enum NoteName& name, const enum Accid& accid, int oct)
{
    int alt = toint(accid);
    assert(-2 <= alt);
    assert(alt <= 2);
    assert(-1 <= oct);
    assert(oct <= 9);
    
    unsigned int i = pitchClass(name);
    int falt = int(floor(alt)); // useless ?
    int r = (i + falt)%12;
    assert(0 <= r);
    assert(r <= 11);
    unsigned int p = ((oct+1) * 12) + r;
    assert(0 <= p);
    assert(p <= 127);
    
    return ((p)+ int((alt-falt)));
}

} // namespace MidiNum


//} // namespace MidiNum

} // end namespace pse
