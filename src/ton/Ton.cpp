//
//  Ton.cpp
//  qparse
//
//  Created by Florent Jacquemard on 03/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include <cmath>        // std::abs
#include <algorithm>    // std::max

#include "Ton.hpp"
#include "Accidental.hpp"
//#include "Pitch.hpp"
#include "Fifths.hpp"
//#include "PSState.hpp"
#include "Weber.hpp" // Weber distance
#include "Scale.hpp" // dependency


namespace pse {

// static abbreviations for accidentals
const enum Accid Ton::_2F = Accid::DoubleFlat;
const enum Accid Ton::_1F = Accid::Flat;
const enum Accid Ton::_0N = Accid::Natural;
const enum Accid Ton::_1S = Accid::Sharp;
const enum Accid Ton::_2S = Accid::DoubleSharp;
const enum Accid Ton::__U = Accid::Undef;

// accidentals in key signatures
// and major or minor natural scales, for each key signature.
const std::array<std::array<enum Accid, 7>, 15> Ton::KEYS =
{{
    { _1F, _1F, _1F, _1F, _1F, _1F, _1F }, // -7  Cb maj / Ab min nat
    { _1F, _1F, _1F, _0N, _1F, _1F, _1F }, // -6  Gb maj / Eb min nat
    { _0N, _1F, _1F, _0N, _1F, _1F, _1F }, // -5  Db maj / Bb min nat
    { _0N, _1F, _1F, _0N, _0N, _1F, _1F }, // -4  Ab maj / F  min nat
    { _0N, _0N, _1F, _0N, _0N, _1F, _1F }, // -3  Eb maj / C  min nat
    { _0N, _0N, _1F, _0N, _0N, _0N, _1F }, // -2  Bb maj / G  min nat
    { _0N, _0N, _0N, _0N, _0N, _0N, _1F }, // -1  F  maj / D  min nat
    { _0N, _0N, _0N, _0N, _0N, _0N, _0N }, //  0  C  maj / A  min nat
    { _0N, _0N, _0N, _1S, _0N, _0N, _0N }, //  1  G  maj / E  min nat
    { _1S, _0N, _0N, _1S, _0N, _0N, _0N }, //  2  D  maj / B  min nat
    { _1S, _0N, _0N, _1S, _1S, _0N, _0N }, //  3  A  maj / F# min nat
    { _1S, _1S, _0N, _1S, _1S, _0N, _0N }, //  4  E  maj / C# min nat
    { _1S, _1S, _0N, _1S, _1S, _1S, _0N }, //  5  B  maj / G# min nat
    { _1S, _1S, _1S, _1S, _1S, _1S, _0N }, //  6  F# maj / D# min nat
    { _1S, _1S, _1S, _1S, _1S, _1S, _1S }  //  7  C# maj / A# min nat
}};

// lead node in minor harmonic tons, in 0..6
const std::array<int, 15> Ton::LEAD_HARM =
{   4, 1, 5, 2, 6, 3, 0, 4, 1, 5, 2, 6, 3, 6, 4 };
// -7 -6 -5 -4 -3 -2 -1  0  1  2  3  4  5  6  7
//  Ab Eb Bb F  C  G  D  A  E  B  F# C# G# D# A#

// accidentals in minor harmonic scales for each key signature.
const std::array<std::array<enum Accid, 7>, 15> Ton::MIN_HARM =
{{
    { _1F, _1F, _1F, _1F, _0N, _1F, _1F }, // -7  Ab min harm
    { _1F, _0N, _1F, _0N, _1F, _1F, _1F }, // -6  Eb min harm
    { _0N, _1F, _1F, _0N, _1F, _0N, _1F }, // -5  Bb min harm
    { _0N, _1F, _0N, _0N, _0N, _1F, _1F }, // -4  F  min harm
    { _0N, _0N, _1F, _0N, _0N, _1F, _0N }, // -3  C  min harm
    { _0N, _0N, _1F, _1S, _0N, _0N, _1F }, // -2  G  min harm
    { _1S, _0N, _0N, _0N, _0N, _0N, _1F }, // -1  D  min harm
    { _0N, _0N, _0N, _0N, _1S, _0N, _0N }, //  0  A  min harm
    { _0N, _1S, _0N, _1S, _0N, _0N, _0N }, //  1  E  min harm
    { _1S, _0N, _0N, _1S, _0N, _1S, _0N }, //  2  B  min harm
    { _1S, _0N, _1S, _1S, _1S, _0N, _0N }, //  3  F# min harm
    { _1S, _1S, _0N, _1S, _1S, _0N, _1S }, //  4  C# min harm
    { _1S, _1S, _0N, _2S, _1S, _1S, _0N }, //  5  G# min harm
    { _2S, _1S, _1S, _1S, _1S, _1S, _0N }, //  6  D# min harm
    { _1S, _1S, _1S, _1S, _2S, _1S, _1S }, //  7   A# min harm
}};

// accidentals in minor melodic scales for each key signature.
const std::array<std::array<enum Accid, 7>, 15> Ton::MIN_MEL =
{{
    { __U, __U, __U, _0N, _0N, __U, __U }, // -7  Ab min mel
    { _0N, _0N, __U, __U, __U, __U, __U }, // -6  Eb min mel
    { __U, __U, __U, __U, _0N, _0N, __U }, // -5  Bb min mel
    { __U, _0N, _0N, __U, __U, __U, __U }, // -4  F  min mel
    { __U, __U, __U, __U, __U, _0N, _0N }, // -3  C  min mel
    { __U, __U, _0N, _1S, __U, __U, __U }, // -2  G  min mel
    { _1S, __U, __U, __U, __U, __U, _0N }, // -1  D  min mel
    { __U, __U, __U, _1S, _1S, _0N, _0N }, //  0  A  min mel
    { _1S, _1S, __U, __U, __U, __U, __U }, // 1  E  min mel
    { __U, __U, __U, __U, _1S, _1S, __U }, //  2  B  min mel
    { __U, _1S, _1S, __U, __U, __U, __U }, //  3  F# min mel
    { __U, __U, __U, __U, __U, _1S, _1S }, //  4  C# min mel
    { __U, __U, _1S, _2S, __U, __U, __U }, //  5  G# min mel
    { _2S, __U, __U, __U, __U, __U, _1S }, //  6  D# min mel
    { __U, __U, __U, _2S, _2S, __U, __U },  // 7  A# min mel
}};


Ton::Ton():
KeyFifth(0),
_mode(ModeName::Undef),
_chromatic(nullptr) // computed on demand
{ }


Ton::Ton(int ks, ModeName mode):
KeyFifth(ks),
_mode(mode),
_chromatic(nullptr) // computed on demand
{
    assert(mode != ModeName::Undef);
}


Ton::Ton(const KeyFifth& ks, ModeName mode):
KeyFifth(ks),
_mode(mode),
_chromatic(nullptr) // computed on demand
{
    assert(mode != ModeName::Undef);
}


Ton::Ton(const Ton& ton):
KeyFifth(ton),
_mode(ton.getMode()),
_chromatic(ton._chromatic) // shared ptr copy
{ }


Ton::~Ton()
{
    // desalloc _chromatic
}


Ton& Ton::operator=(const Ton& rhs)
{
    if (this != &rhs)
    {
        KeyFifth::operator=(rhs);
        _mode = rhs._mode;
    }
    return *this;
}


bool Ton::operator==(const Ton& rhs) const
{
    return (KeyFifth::operator==(rhs) && (_mode == rhs._mode));
}


bool Ton::operator!=(const Ton& rhs) const
{
    return (! operator==(rhs));
}


bool Ton::undef() const
{
    assert(_mode != ModeName::Undef || _sig == 0);
    return (_mode == ModeName::Undef);
}


const enum NoteName Ton::getName() const
{
    return Fifths::name(tonic());
}


const enum Accid Ton::getAccidental() const
{
    return Fifths::accid(tonic());
}


int Ton::getPitchClass() const
{
    return Fifths::pc(tonic());
}


enum Accid Ton::accidKey(int n) const
{
    assert(-7 <= _sig);
    assert(_sig <= 7);
    assert(0 <= n);
    assert(n <= 6);
    return KEYS[_sig + 7][n];
}


enum Accid Ton::accidKey(const enum NoteName& name) const
{
    assert(defined(name));
    return accidKey(toint(name));
}


enum Accid Ton::accidDia(int n) const
{
    assert(-7 <= _sig);
    assert(_sig <= 7);
    assert(0 <= n);
    assert(n <= 6);

    switch (_mode)
    {
        case ModeName::Undef:
            return Accid::Undef;
            
        case ModeName::Major:
            return KEYS[_sig + 7][n];
            
        case ModeName::Minor:  // harmonic
            return MIN_HARM[_sig + 7][n];
            
        case ModeName::MinorNat:
            return KEYS[_sig + 7][n];
            
        case ModeName::MinorMel:
            return MIN_MEL[_sig + 7][n];
            
        case ModeName::Ionian:
        case ModeName::Dorian:
        case ModeName::Phrygian:
        case ModeName::Lydian:
        case ModeName::Mixolydian:
        case ModeName::Aeolian:
        case ModeName::Locrian:
            return KEYS[_sig + 7][n];
            
        default:
        {
            ERROR("unknown Ton mode");
            return Accid::Undef;
        }
    }
}


enum Accid Ton::accidDia(const enum NoteName& name) const
{
    assert(defined(name));
    return accidDia(toint(name));
}


enum NoteName Ton::name(int d) const
{
    // diatonic scale
    assert(_mode == ModeName::Major ||
           _mode == ModeName::Minor ||
           _mode == ModeName::MinorNat ||
           _mode == ModeName::MinorMel);
    assert(d < 7);
    return getName() + d;
}


enum Accid Ton::accidental(int d) const
{
//    assert(_mode == ModeName::Major ||
//           _mode == ModeName::Minor ||
//           _mode == ModeName::MinorNat ||
//           _mode == ModeName::MinorMel);
    assert(d < 7); // diatonic scale
    return accidDia(name(d));
}


bool Ton::lead(const enum NoteName& name) const
{
    assert(defined(name));
    int n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    assert(-7 <= _sig);
    assert(_sig <= 7);

    // harmonic minor
    if (_mode == ModeName::Minor)
    {
        // DEBUGU("{} ({}) is lead of {}", name, n, *this);
        return (KEYS[_sig + 7][n] != MIN_HARM[_sig + 7][n]);
    }
    // melodic minor
    else if (_mode == ModeName::MinorMel)
    {
        // DEBUGU("{} ({}) is lead of {}", name, n, *this);
        return (KEYS[_sig + 7][n] != MIN_MEL[_sig + 7][n]);
    }
    else
        return false;
}


unsigned int Ton::dist(const enum NoteName& name, const enum Accid& accid) const
{
    assert(defined(name));
    assert(defined(accid));
    // int n = name.toint();
    // assert(0 <= n);
    // assert(n <= 6);
    // int a = accid.toint();
    // assert(-2 <= a);
    // assert(a <= 2);
    // location of the given note in the array of fifths
    int loc = Fifths::index(name, accid);
    assert(-15 <= loc);
    assert(loc <= 19);   // 0 is 'C'
    return std::abs(loc - this->fifths());
}


unsigned int Ton::distFifths(const Ton& rhs) const
{
    return std::abs(fifths() - rhs.fifths());
}


unsigned int Ton::distHamming(const Ton& rhs) const
{
    unsigned int res = 0;
    
    for (int i = 0; i < 7; ++i) // pitch names
    {
        const enum NoteName n = NoteName(i);
        const enum Accid& this_ai = accidDia(n);
        const enum Accid& rhs_ai  = rhs.accidDia(n);
        assert(this_ai != Accid::Undef);
        assert(rhs_ai != Accid::Undef);
        if (this_ai != rhs_ai)
            res++;
    }
    return res;
}


unsigned int Ton::distDiatonic(const Ton& rhs) const
{
    unsigned int res = 0;
    
    for (int i = 0; i < 7; ++i) // pitch names
    {
        const enum NoteName n = NoteName(i);
        const enum Accid& this_ai = accidDia(n);
        const enum Accid& rhs_ai  = rhs.accidDia(n);
        assert(this_ai != Accid::Undef);
        assert(rhs_ai != Accid::Undef);
        res += accidDist(this_ai, rhs_ai);
    }
    return res;
}


unsigned int Ton::distWeber(const Ton& rhs) const
{
    int res = Weber::static_dist(*this, rhs);
    assert(res != Weber::UNDEF_DIST);
    return res;
}


//unsigned int Ton::dist(const Ton& rhs) const
//{
//    if (_mode == ModeName::Undef)
//    {
//        WARN("Ton distance {} {}: one undef mode", *this, rhs);
//        return std::abs(fifths() - rhs.fifths());
//    }
//    else if (_mode == rhs._mode)
//    {
//        // dist in array of fifths
//        return std::abs(fifths() - rhs.fifths());
//    }
//    else if (_mode == ModeName::Maj)
//    {
//
//    }
//}


// static
//int Ton::dist(int name, int alt, int sig)
//{
//    assert(0 <= name);
//    assert(name <= 6);
//    assert(-2 <= alt);
//    assert(alt <= 2);
//    assert(-7 <= sig);
//    assert(sig <= 7);
//    // location of the given note in the array of fifths
//    //std::pair<int, int> p = std::make_pair(_name, alt);
//    // const int& loc = FIFTHS.at(p);
//    int loc = 99;
//    for (int i = 0; i < 35; ++i)
//    {
//        if ((FIFTHS[i][0] == name) && (FIFTHS[i][1] == alt))
//        {
//            loc = i;
//            break;
//        }
//    }
//    // not found
//    if (loc > 34)
//    {
//        ERROR("KeySig distTon {} {} not found in array of fifths", name, alt);
//        return 0;
//    }
//
//    // location of the tonic in array of fifths
//    int locton = 15 + sig;
//
//    return std::abs(loc - locton);
//}


const Scale& Ton::chromatic()
{
    if (_chromatic == nullptr)
    {
        _chromatic = std::make_shared<Scale>(*this, ModeName::Chromatic);
    }
    assert(_chromatic);
    return *(_chromatic);
}


int Ton::tonic() const
{
    int i = 99;
    switch (_mode)
    {
        case ModeName::Undef:
            break;

        case ModeName::Major:
            i = fifths();
            break;

        case ModeName::Minor:
        case ModeName::MinorNat:
        case ModeName::MinorMel:
            i = fifths()+3;
            break;
            
        case ModeName::Ionian:
            i = fifths();
            break;

        case ModeName::Dorian:
            i = fifths(); // + 2;
            break;

        case ModeName::Phrygian:
            i = fifths(); // + 4;
            break;

        case ModeName::Lydian:
            i = fifths();
            break;

        case ModeName::Mixolydian:
            i = fifths();
            break;
            
        case ModeName::Aeolian:
            i = fifths();
            break;

        case ModeName::Locrian:
            i = fifths();
            break;

        default:
        {
            ERROR("unknown Ton mode");
            break;
        }
    }
    
    if (i < -15 || 19 < i)
        ERROR("Ton: tonic = {}", i);

    return i;
}


void Ton::print(std::ostream& o) const
{
    int i = tonic();
    
    if ((-15 <= i) && (i <= 19))
    {
        o << Fifths::name(i);
        if (Fifths::accid(i) != Accid::Natural)
            o << Fifths::accid(i);
        o << tostring(_mode);
        o << ' ';
        o << '(';
        KeyFifth::print(o);
        o << ')';
    }
    else
        o << "ERROR";
}


std::ostream& operator<<(std::ostream& o, const Ton& ton)
{
    ton.print(o);
    return o;
}


} // end namespace pse
