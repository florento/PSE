//
//  MidiNum.cpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "MidiNum.hpp"

namespace pse {

//namespace MidiNum{

// static
int MidiNum::midi_to_octave(unsigned int m, const enum NoteName& n)
{
    return midi_to_octave(m, n, Accid::Undef, false); // no debug mode
}


// static
int MidiNum::midi_to_octave(unsigned int m,
                            const enum NoteName& n,
                            const enum Accid& a,
                            bool debug)
{
    assert(0 <= m);
    assert(m <= 128);
    int oct = int(floor(m/12)) - 1;
    assert(-1 <= oct);
    assert(oct <= 9);
    int chroma = m % 12;
    assert(!debug || a != Accid::Undef);
    assert(!debug || a == accid(chroma, n));
    
    // adjust octave for extreme notes
    if (chroma == 0 && n == pse::NoteName::B)
    {
        assert(!debug || a == pse::Accid::Sharp);
        return oct - 1;
    }
    if (chroma == 0 && n == pse::NoteName::A)
    {
        assert(!debug || a == pse::Accid::TripleSharp);
        return oct - 1;
    }
    else if (chroma == 1 && n == pse::NoteName::B)
    {
        assert(!debug || a == pse::Accid::DoubleSharp);
        return oct - 1;
    }
    else if (chroma == 2 && n == pse::NoteName::B)
    {
        assert(!debug || a == pse::Accid::TripleSharp);
        return oct - 1;
    }
    else if (chroma == 9 && n == pse::NoteName::C)
    {
        assert(!debug || a == pse::Accid::TripleFlat);
        return oct + 1;
    }
    else if (chroma == 10 && n == pse::NoteName::C)
    {
        assert(!debug || a == pse::Accid::DoubleFlat);
        return oct + 1;
    }
    else if (chroma == 11 && n == pse::NoteName::C)
    {
        assert(!debug || a == pse::Accid::Flat);
        return oct + 1;
    }
    else if (chroma == 11 && n == pse::NoteName::D)
    {
        assert(!debug || a == pse::Accid::TripleFlat);
        return oct + 1;
    }
    else
    {
        // TBD assert the name // accid in the other cases
        return oct;
    }
}

const enum Accid MidiNum::_3F = Accid::TripleFlat;
const enum Accid MidiNum::_2F = Accid::DoubleFlat;
const enum Accid MidiNum::_1F = Accid::Flat;
const enum Accid MidiNum::_0N = Accid::Natural;
const enum Accid MidiNum::_1S = Accid::Sharp;
const enum Accid MidiNum::_2S = Accid::DoubleSharp;
const enum Accid MidiNum::_3S = Accid::TripleSharp;
const enum Accid MidiNum::__U = Accid::Undef;


//// static
const enum Accid MidiNum::ACCID[12][7] =
{
//      C    D    E    F    G    A    B
    { _0N, _2F, __U, __U, __U, _3S, _1S }, //   0 = C
    { _1S, _1F, _3F, __U, __U, __U, _2S  }, //  1 = C#, Db
    { _2S, _0N, _2F, __U, __U, __U, _3S  }, //  2 = D
    { _3S, _1S, _1F, _2F, __U, __U, __U  }, //  3 = D#, Eb
    { __U, _2S, _0N, _1F, _3F, __U, __U  }, //  4 = E
    { __U, _3S, _1S, _0N, _2F, __U, __U  }, //  5 = F
    { __U, __U, _2S, _1S, _1F, _3F, __U  }, //  6 = F#, Gb
    { __U, __U, __U, _2S, _0N, _2F, __U  }, //  7 = G
    { __U, __U, __U, _3S, _1S, _1F, _3F  }, //  8 = G#, Ab
    { _3F, __U, __U, __U, _2S, _0N, _2F  }, //  9 = A
    { _2F, __U, __U, __U, _3S, _1S, _1F  }, // 10 = A#, Bb
    { _1F, _3F, __U, __U, __U, _2S, _0N  }  // 11 = B
};


enum Accid MidiNum::accid(int c, const enum NoteName& n)
{
    assert(0 <= c);
    assert(c < 12);
    assert(n != NoteName::Undef);
    int i = toint(n);
    assert(0 <= i);
    assert(i < 7);
    return ACCID[c][i];
}


// FAUX!!
//enum Accid MidiNum::accid(int c, const enum NoteName& n)
//{
//    // pitch class for each note name in 0 (C) .. 7 (B)
//    //                  C  D  E  F  G  A  B
//    const int PC[7] = { 0, 2, 4, 5, 7, 9, 11 };
//
//    assert(0 <= c);
//    assert(c <= 11);
//    assert(n != NoteName::Undef);
//    int ni = toint(n);
//    assert(0 <= ni);
//    assert(ni <= 6);
//    int r = (c - PC[ni]) % 12;
//    if (r < 0) r+= 12;
//    assert(0 <= r);
//    assert(r < 12);
//    switch (r)
//    {
//        case -3:
//            return Accid::TripleFlat;
//        case -2:
//            return Accid::DoubleFlat;
//        case -1:
//            return Accid::Flat;
//        case  0:
//            return Accid::Natural;
//        case  1:
//            return Accid::Sharp;
//        case  2:
//            return Accid::DoubleSharp;
//        case  3:
//            return Accid::TripleSharp;
//        default:
//            return Accid::Undef;
//    }
//}

unsigned int MidiNum::pitchClass(const enum NoteName& n, const enum Accid& a)
{
    assert(n != NoteName::Undef);
    assert(a != Accid::Undef);
    unsigned int m = to_midi(n, a, 2); // octave = 2
    // assert(m < 128);
    return (m % 12);
}


//unsigned int MidiNum::pitchClass(const enum NoteName& n)
//{
//    return pitchClass(n, Accid::Natural);
//}


unsigned int MidiNum::pitchClass(const enum NoteName& n)
{
    switch (n)
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
            ERROR("pitchClass: wrong note name {}", n);
            return 12;
        }
    }
}


// TBC
unsigned int MidiNum::to_midi(const enum NoteName& n,
                              const enum Accid& a,
                              int oct)
{
    int alt = toint(a);
    assert(-3 <= alt);
    assert(alt <= 3);
    assert(-2 <= oct);
    assert(oct <= 9);
    
    assert(n != NoteName::Undef);
    int i = pitchClass(n); // in 0..11
    //int falt = int(floor(alt)); // useless ?
    int r = i + alt;         // in -3..14
    assert(-3 <= i);
    assert(r <= 14);
    if (r < 0)
    {
        --oct;
        r += 12;
    }
    else if (11 < r)
    {
        ++oct;
        r -= 12;   // r = r % 12;
    }
    assert(0 <= r);
    assert(r <= 11);
    int p = ((oct+1) * 12) + r;
    assert(0 <= p);
    assert(p <= 128);
    return p;
    //return ((p)+ int((alt-falt)));
}

//} // namespace MidiNum


//} // namespace MidiNum

} // end namespace pse
