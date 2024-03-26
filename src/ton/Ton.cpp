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
#include "Weber_static.hpp" // Weber distance
#include "WeberModal_static.hpp" // Weber distance
//#include "Scale.hpp" // dependency


namespace pse {


int Ton::init_KSofMode(int ks, ModeName mode)
{
    assert(-7 <= ks);
    assert(ks <= 7);
    
    switch (mode)
    {
        case ModeName::Major:
        case ModeName::Ionian:
        case ModeName::Minor:  // harmonic
        case ModeName::MinorNat:
        case ModeName::Aeolian:
        case ModeName::MinorMel:
        case ModeName::Dorian:
        case ModeName::Phrygian:
        case ModeName::Lydian:
        case ModeName::Mixolydian:
        case ModeName::Locrian:
            return ks;

        case ModeName::MajorBlues:
        case ModeName::MinorBlues:
            return ks;
            
        case ModeName::Augmented:
            if (ks == -7)
            {
                ERROR("Ton, blues scale, unsupported KS {}", ks);
                return 5;
            }
            else if (ks == -6)
            {
                ERROR("Ton, blues scale, unsupported KS {}", ks);
                return 6;
            }
            else if (ks == 7)
            {
                ERROR("Ton, blues scale, unsupported KS {}", ks);
                return -5;
            }
            else
            {
                assert(-5 <= ks && ks <= 6);
                return ks;
            }

        case ModeName::Diminished:
        case ModeName::DiminishedHW:
            if (ks == -7 || ks == -4  || ks == 2 || ks == 5)
            {
                ERROR("Ton, diminished scale, unsupported KS {}", ks);
                return -1;
            }
            if (ks == -6 || ks == -3  || ks == 3 || ks == 6)
            {
                ERROR("Ton, diminished scale, unsupported KS {}", ks);
                return 0;
            }
            if (ks == -5 || ks == -2  || ks == 4 || ks == 7)
            {
                ERROR("Ton, diminished scale, unsupported KS {}", ks);
                return 1;
            }
            else
            {
                assert(-1 <= ks && ks <= 1);
                return ks;
            }

            
        case ModeName::Whole:
            if (ks == -6 || ks == -4 || ks == -2  || 
                ks == 2 || ks == 4 || ks == 6)
            {
                ERROR("Ton, whole-ton scale, unsupported KS {}", ks);
                return 0;
            }
            if (ks == -7 || ks == -5 || ks == -3  || ks == -1 ||
                ks == 1 || ks == 5 || ks == 7)
            {
                ERROR("Ton, whole-ton scale, unsupported KS {}", ks);
                return 1;
            }
            else
            {
                assert(0 <= ks && ks <= 1);
                return ks;
            }

        case ModeName::Undef:
            ERROR("Ton constructor: UNDEF mode");
            return ks;
            
        default:
        {
            ERROR("Ton constructor: unknown mode {}", mode);
            return ks;
        }
    }
}


Ton::Ton():
KeyFifth(0),
_mode(ModeName::Undef),
_chromatic()
{ }


Ton::Ton(int ks, ModeName mode):
KeyFifth(init_KSofMode(ks, mode)),
_mode(mode),
_chromatic(*this, ModeName::Chromatic)
{
    assert(mode != ModeName::Undef);
}


Ton::Ton(const KeyFifth& ks, ModeName mode):
Ton(ks.fifths(), mode)
{ }


Ton::Ton(const Ton& ton):
KeyFifth(ton),
_mode(ton.getMode()),
_chromatic(ton._chromatic) // shared ptr copy
{ 
    assert(init_KSofMode(ton.fifths(), ton.getMode()) == ton.fifths());
}


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
    return Fifths::name(tonicFifth());
}


const enum Accid Ton::getAccidental() const
{
    return Fifths::accid(tonicFifth());
}


int Ton::getPitchClass() const
{
    return Fifths::pc(tonicFifth());
}


int Ton::getRealKS() const
{
    assert(-7 <= _sig);
    assert(_sig <= 7);
    if (_mode == ModeName::Dorian) return _sig + 1 ;
    if (_mode == ModeName::Phrygian) return _sig - 1 ;
    if (_mode == ModeName::Lydian) return _sig + 1 ;
    if (_mode == ModeName::Mixolydian) return _sig - 1 ;
    if (_mode == ModeName::Locrian) return _sig - 2 ;
    else return _sig ;
}


// m is a pitch class
const enum NoteName Ton::chromaName(int m)
{
    assert(0 <= m);
    assert(m < 12);

    // name of the tonic of scale
    enum NoteName tonic = getName();
    // pitch class of the tonic of scale
    int p = getPitchClass();
    // degree of m in the chromatic harmonic scale of p
    size_t deg = (p <= m)?(m - p):(12-p+m);
    assert(0 <= deg); // debug
    assert(deg < 12);

    return tonic+CHROMA_NAMES[deg];
}


// static
enum Accid Ton::accidSingle(const accids_t a)
{
    assert(Accids::single(a));
    return Accids::first(a);
}


enum Accid Ton::accidKey(int n) const
{
    assert(-7 <= _sig);
    assert(_sig <= 7);
    assert(0 <= n);
    assert(n <= 6);
    return MAJOR[_sig + 7][n];
}


enum Accid Ton::accidKey(const enum NoteName& name) const
{
    assert(name != NoteName::Undef);
    return accidKey(toint(name));
}


accids_t Ton::accidScale(int n, ModeName mode) const
{
    assert(-7 <= _sig && _sig <= 7);
    assert(0 <= n);
    assert(n <= 6);

    switch (mode)
    {
        case ModeName::Undef:
            return Accids::encode();
            
        case ModeName::Major:
        case ModeName::Ionian:
            return Accids::encode(MAJOR[_sig + 7][n]);
            
        case ModeName::Minor:  // harmonic
            return Accids::encode(MIN_HARM[_sig + 7][n]);
            
        case ModeName::MinorNat:
        case ModeName::Aeolian:
            return Accids::encode(MAJOR[_sig + 7][n]);
            
        case ModeName::MinorMel:
            return Accids::encode(MIN_MEL[_sig + 7][n]);
            
        case ModeName::Dorian:
            return Accids::encode(DORIAN[_sig + 7][n]);
            
        case ModeName::Phrygian:
            return Accids::encode(PHRYGIAN[_sig + 7][n]);
            
        case ModeName::Lydian:
            return Accids::encode(LYDIAN[_sig + 7][n]);
            
        case ModeName::Mixolydian:
            return Accids::encode(MIXOLYDIAN[_sig + 7][n]);
                        
        case ModeName::Locrian:
            return Accids::encode(LOCRIAN[_sig + 7][n]);

        case ModeName::MajorBlues:
            return MAJ_BLUES[_sig + 7][n];
            
        case ModeName::MinorBlues:
            return MIN_BLUES[_sig + 7][n];

        case ModeName::Augmented:
            assert(-5 <= _sig && _sig <= 6);
            return AUGMENTED[_sig+5][n];

        case ModeName::Diminished:
            assert(-1 <= _sig && _sig <= 1);
            return DIMINISHED_WH[_sig+1][n];
            
        case ModeName::DiminishedHW:
            assert(-1 <= _sig && _sig <= 1);
            return DIMINISHED_HW[_sig+1][n];

        case ModeName::Whole:
            assert(0 <= _sig && _sig <= 1);
            return WHOLE_TONE[_sig][n];

        default:
        {
            ERROR("Ton accidScale: unknown Ton mode");
            return Accids::encode(); // undef
        }
    }
}


accids_t Ton::accidScale(const enum NoteName& name) const
{
    assert(name != NoteName::Undef);
    return accidScale(toint(name), _mode);
}


accids_t Ton::accidScale(int n) const
{
    return accidScale(n, _mode);
}


accids_t Ton::accidScale(const enum NoteName& name, ModeName mode) const
{
    assert(name != NoteName::Undef);
    return accidScale(toint(name), mode);
}


//accids_t Ton::accidScale(const enum NoteName& n) const
//{
//    assert(-7 <= _sig);
//    assert(_sig <= 7);
//    assert(0 <= n);
//    assert(n <= 6);
//
//}


enum Accid Ton::accidDia(int n, ModeName mode) const
{
    assert(-7 <= _sig);
    assert(_sig <= 7);
    assert(0 <= n);
    assert(n <= 6);

    switch (mode)
    {
        case ModeName::Undef:
            return Accid::Undef;
            
        case ModeName::Major:
        case ModeName::Ionian:
            return MAJOR[_sig + 7][n];
            
        case ModeName::Minor:  // harmonic
            return MIN_HARM[_sig + 7][n];
            
        case ModeName::MinorNat:
        case ModeName::Aeolian:
            return MAJOR[_sig + 7][n];
            
        case ModeName::MinorMel:
            return MIN_MEL[_sig + 7][n];
            
        case ModeName::Dorian:
            return DORIAN[_sig + 7][n];
            
        case ModeName::Phrygian:
            return PHRYGIAN[_sig + 7][n];
            
        case ModeName::Lydian:
            return LYDIAN[_sig + 7][n];
            
        case ModeName::Mixolydian:
            return MIXOLYDIAN[_sig + 7][n];
                        
        case ModeName::Locrian:
            return LOCRIAN[_sig + 7][n];
            
        default:
        {
            ERROR("Ton accidDia: unknown Ton mode");
            return Accid::Undef;
        }
    }
}


enum Accid Ton::accidDia(const enum NoteName& name, ModeName mode) const
{
    assert(name != NoteName::Undef);
    return accidDia(toint(name), mode);
}


enum Accid Ton::accidDia(const enum NoteName& name) const
{
    assert(name != NoteName::Undef);
    return accidDia(toint(name), _mode);
}


enum Accid Ton::accidDia(int n) const
{
    assert(-7 <= _sig);
    assert(_sig <= 7);
    assert(0 <= n);
    assert(n <= 6);
 
    return accidDia(n, _mode);
}


enum NoteName Ton::name(int d) const
{
    // diatonic scale
//    assert(_mode == ModeName::Major ||
//           _mode == ModeName::Minor ||
//           _mode == ModeName::MinorNat ||
//           _mode == ModeName::MinorMel);
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
    return accidDia(name(d), _mode);
}


bool Ton::lead(const enum NoteName& name) const
{
    assert(name != NoteName::Undef);
    int n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    assert(-7 <= _sig);
    assert(_sig <= 7);

    // harmonic minor
    if (_mode == ModeName::Minor)
    {
        // DEBUGU("{} ({}) is lead of {}", name, n, *this);
        return (MAJOR[_sig + 7][n] != MIN_HARM[_sig + 7][n]);
    }
    // melodic minor
    else if (_mode == ModeName::MinorMel)
    {
        // DEBUGU("{} ({}) is lead of {}", name, n, *this);
        return (MAJOR[_sig + 7][n] != MIN_MEL[_sig + 7][n]);
    }
    else
        return false;
}


unsigned int Ton::dist(const enum NoteName& name, const enum Accid& accid) const
{
    assert(name != NoteName::Undef);
    assert(accid != Accid::Undef);
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
        const enum Accid& this_ai = accidDia(n, _mode);
        const enum Accid& rhs_ai  = rhs.accidDia(n, _mode);
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
        const enum Accid& this_ai = accidDia(n, _mode);
        const enum Accid& rhs_ai  = rhs.accidDia(n, _mode);
        assert(this_ai != Accid::Undef);
        assert(rhs_ai != Accid::Undef);
        res += accidDist(this_ai, rhs_ai);
    }
    return res;
}


unsigned int Ton::distWeber(const Ton& rhs) const
{
    int res = Weber_static::dist(*this, rhs);
    assert(res != Weber_static::UNDEF_DIST);
    return res;
}

unsigned int Ton::distWeberModal(const Ton& rhs) const
{
    int res = WeberModal_static::dist(*this, rhs);
    assert(res != Weber_static::UNDEF_DIST);
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


// computation on demand
//const Scale& Ton::chromatic()
//{
//    if (_chromatic == nullptr)
//    {
//        _chromatic = std::make_shared<Scale>(*this, ModeName::Chromatic);
//    }
//    assert(_chromatic);
//    return *(_chromatic);
//}

const Scale& Ton::chromatic() const
{
    return _chromatic;
}


int Ton::tonicFifth() const
{
    int i = 99;
    switch (_mode)
    {
        case ModeName::Major:
        case ModeName::MajorBlues:
            i = fifths();
            break;

        case ModeName::Minor:
        case ModeName::MinorNat:
        case ModeName::MinorMel:
        case ModeName::MinorBlues:
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

     // case ModeName::Augmented:
     //     i = fifths();
     //     break;
     //
     // case ModeName::Diminished:
     //     i = fifths();
     //     break;
     //
     // case ModeName::DiminishedHW:
     //     i = fifths();
     //     break;
     //
     // case ModeName::Whole:
     //     i = fifths();
     //     break;

        case ModeName::Undef:
        {
            ERROR("Ton tonicFifth: undef mode");
            break;
        }

        default:
        {
            ERROR("Ton tonicFifth: unexpected mode {}", _mode);
            break;
        }
    }
    
    if (i < -15 || 19 < i)
        ERROR("Ton: tonic = {}", i);

    return i;
}


void Ton::print(std::ostream& o) const
{
    int i = tonicFifth();
    
    if ((-15 <= i) && (i <= 19))
    {
        o << Fifths::name(i);
        if (Fifths::accid(i) != Accid::Natural)
            o << Fifths::accid(i);
        o << ' ' << tostring(_mode);
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


// static abbreviations for accidentals
const enum Accid Ton::_F = Accid::DoubleFlat;
const enum Accid Ton::_f = Accid::Flat;
const enum Accid Ton::_n = Accid::Natural;
const enum Accid Ton::_s = Accid::Sharp;
const enum Accid Ton::_S = Accid::DoubleSharp;
const enum Accid Ton::_U = Accid::Undef;

const accids_t Ton::_Ff = Accids::encode(Accid::DoubleFlat, Accid::Flat);
const accids_t Ton::_f_ = Accids::encode(Accid::Flat);
const accids_t Ton::_fn = Accids::encode(Accid::Flat, Accid::Natural);
const accids_t Ton::_n_ = Accids::encode(Accid::Natural);
const accids_t Ton::_ns = Accids::encode(Accid::Natural, Accid::Sharp);
const accids_t Ton::_s_ = Accids::encode(Accid::Sharp);
const accids_t Ton::_sS = Accids::encode(Accid::Sharp, Accid::DoubleSharp);
const accids_t Ton::_UU = Accids::encode();


// accidentals in key signatures
// and major or minor natural scales, for each key signature.
const std::array<std::array<enum Accid, 7>, 15> Ton::MAJOR =
{{
    { _f, _f, _f, _f, _f, _f, _f }, // -7  Cb maj / Ab min nat
    { _f, _f, _f, _n, _f, _f, _f }, // -6  Gb maj / Eb min nat
    { _n, _f, _f, _n, _f, _f, _f }, // -5  Db maj / Bb min nat
    { _n, _f, _f, _n, _n, _f, _f }, // -4  Ab maj / F  min nat
    { _n, _n, _f, _n, _n, _f, _f }, // -3  Eb maj / C  min nat
    { _n, _n, _f, _n, _n, _n, _f }, // -2  Bb maj / G  min nat
    { _n, _n, _n, _n, _n, _n, _f }, // -1  F  maj / D  min nat
    { _n, _n, _n, _n, _n, _n, _n }, //  0  C  maj / A  min nat
    { _n, _n, _n, _s, _n, _n, _n }, //  1  G  maj / E  min nat
    { _s, _n, _n, _s, _n, _n, _n }, //  2  D  maj / B  min nat
    { _s, _n, _n, _s, _s, _n, _n }, //  3  A  maj / F# min nat
    { _s, _s, _n, _s, _s, _n, _n }, //  4  E  maj / C# min nat
    { _s, _s, _n, _s, _s, _s, _n }, //  5  B  maj / G# min nat
    { _s, _s, _s, _s, _s, _s, _n }, //  6  F# maj / D# min nat
    { _s, _s, _s, _s, _s, _s, _s }  //  7  C# maj / A# min nat
}};

// lead node in minor harmonic tons, in 0..6
const std::array<int, 15> Ton::LEAD_HARM =
{   4, 1, 5, 2, 6, 3, 0, 4, 1, 5, 2, 6, 3, 6, 4 };
// -7 -6 -5 -4 -3 -2 -1  0  1  2  3  4  5  6  7
//  Ab Eb Bb F  C  G  D  A  E  B  F# C# G# D# A#

// accidentals in minor harmonic scales for each key signature.
const std::array<std::array<enum Accid, 7>, 15> Ton::MIN_HARM =
{{
    { _f, _f, _f, _f, _n, _f, _f }, // -7  Ab min harm
    { _f, _n, _f, _n, _f, _f, _f }, // -6  Eb min harm
    { _n, _f, _f, _n, _f, _n, _f }, // -5  Bb min harm
    { _n, _f, _n, _n, _n, _f, _f }, // -4  F  min harm
    { _n, _n, _f, _n, _n, _f, _n }, // -3  C  min harm
    { _n, _n, _f, _s, _n, _n, _f }, // -2  G  min harm
    { _s, _n, _n, _n, _n, _n, _f }, // -1  D  min harm
    { _n, _n, _n, _n, _s, _n, _n }, //  0  A  min harm
    { _n, _s, _n, _s, _n, _n, _n }, //  1  E  min harm
    { _s, _n, _n, _s, _n, _s, _n }, //  2  B  min harm
    { _s, _n, _s, _s, _s, _n, _n }, //  3  F# min harm
    { _s, _s, _n, _s, _s, _n, _s }, //  4  C# min harm
    { _s, _s, _n, _S, _s, _s, _n }, //  5  G# min harm
    { _S, _s, _s, _s, _s, _s, _n }, //  6  D# min harm
    { _s, _s, _s, _s, _S, _s, _s }, //  7  A# min harm
}};

// accidentals in minor melodic scales for each key signature.
const std::array<std::array<enum Accid, 7>, 15> Ton::MIN_MEL =
{{
    { _f, _f, _f, _n, _n, _f, _f }, // -7  Ab min mel
    { _n, _n, _f, _n, _f, _f, _f }, // -6  Eb min mel
    { _n, _f, _f, _n, _n, _n, _f }, // -5  Bb min mel
    { _n, _n, _n, _n, _n, _f, _f }, // -4  F  min mel
    { _n, _n, _f, _n, _n, _n, _n }, // -3  C  min mel
    { _n, _n, _n, _s, _n, _n, _f }, // -2  G  min mel
    { _s, _n, _n, _n, _n, _n, _n }, // -1  D  min mel
    { _n, _n, _n, _s, _s, _n, _n }, //  0  A  min mel
    { _s, _s, _n, _s, _n, _n, _n }, //  1  E  min mel
    { _s, _n, _n, _s, _s, _s, _n }, //  2  B  min mel
    { _s, _s, _s, _s, _s, _n, _n }, //  3  F# min mel
    { _s, _s, _n, _s, _s, _s, _s }, //  4  C# min mel
    { _s, _s, _s, _S, _s, _s, _n }, //  5  G# min mel
    { _S, _s, _s, _s, _s, _s, _s }, //  6  D# min mel
    { _s, _s, _s, _S, _S, _s, _s },  // 7  A# min mel
}};

const std::array<std::array<enum Accid, 7>, 15> Ton::DORIAN =
{{
    { _f, _f, _f, _n, _f, _f, _f }, // -7  Ab dor
    { _n, _f, _f, _n, _f, _f, _f }, // -6  Eb dor
    { _n, _f, _f, _n, _n, _f, _f }, // -5  Bb dor
    { _n, _n, _f, _n, _n, _f, _f }, // -4  F  dor
    { _n, _n, _f, _n, _n, _n, _f }, // -3  C  dor
    { _n, _n, _n, _n, _n, _n, _f }, // -2  G  dor
    { _n, _n, _n, _n, _n, _n, _n }, // -1  D  dor
    { _n, _n, _n, _s, _n, _n, _n }, //  0  A  dor
    { _s, _n, _n, _s, _n, _n, _n }, //  1  E  dor
    { _s, _n, _n, _s, _s, _n, _n }, //  2  B  dor
    { _s, _s, _n, _s, _s, _n, _n }, //  3  F# dor
    { _s, _s, _n, _s, _s, _s, _n }, //  4  C# dor
    { _s, _s, _s, _s, _s, _s, _n }, //  5  G# dor
    { _s, _s, _s, _s, _s, _s, _s }, //  6  D# dor
    { _s, _s, _s, _S, _s, _s, _s },  // 7  A# dor
}};


const std::array<std::array<enum Accid, 7>, 15> Ton::PHRYGIAN =
{{
    { _f, _f, _f, _f, _f, _f, _F }, // -7  Ab phryg
    { _f, _f, _f, _f, _f, _f, _f }, // -6  Eb phryg
    { _f, _f, _f, _n, _f, _f, _f }, // -5  Bb phryg
    { _n, _f, _f, _n, _f, _f, _f }, // -4  F  phryg
    { _n, _f, _f, _n, _n, _f, _f }, // -3  C  phryg
    { _n, _n, _f, _n, _n, _f, _f }, // -2  G  phryg
    { _n, _n, _f, _n, _n, _n, _f }, // -1  D  phryg
    { _n, _n, _n, _n, _n, _n, _f }, //  0  A  phryg
    { _n, _n, _n, _n, _n, _n, _n }, //  1  E  phryg
    { _n, _n, _n, _s, _n, _n, _n }, //  2  B  phryg
    { _s, _n, _n, _s, _n, _n, _n }, //  3  F# phryg
    { _s, _n, _n, _s, _s, _n, _n }, //  4  C# phryg
    { _s, _s, _n, _s, _s, _n, _n }, //  5  G# phryg
    { _s, _s, _n, _s, _s, _s, _n }, //  6  D# phryg
    { _s, _s, _s, _s, _s, _s, _n },  // 7  A# phryg
}};


const std::array<std::array<enum Accid, 7>, 15> Ton::LYDIAN =
{{
    { _f, _f, _f, _n, _f, _f, _f }, // -7  Cb lyd
    { _n, _f, _f, _n, _f, _f, _f }, // -6  Gb lyd
    { _n, _f, _f, _n, _n, _f, _f }, // -5  Db lyd
    { _n, _n, _f, _n, _n, _f, _f }, // -4  Ab lyd
    { _n, _n, _f, _n, _n, _n, _f }, // -3  Eb lyd
    { _n, _n, _n, _n, _n, _n, _f }, // -2  Bb lyd
    { _n, _n, _n, _n, _n, _n, _n }, // -1  F  lyd
    { _n, _n, _n, _s, _n, _n, _n }, //  0  C  lyd
    { _s, _n, _n, _s, _n, _n, _n }, //  1  G  lyd
    { _s, _n, _n, _s, _s, _n, _n }, //  2  D  lyd
    { _s, _s, _n, _s, _s, _n, _n }, //  3  A  lyd
    { _s, _s, _n, _s, _s, _s, _n }, //  4  E  lyd
    { _s, _s, _s, _s, _s, _s, _n }, //  5  B  lyd
    { _s, _s, _s, _s, _s, _s, _s }, //  6  F# lyd
    { _s, _s, _s, _S, _s, _s, _s }  //  7  C# lyd
}};


const std::array<std::array<enum Accid, 7>, 15> Ton::MIXOLYDIAN =
{{
    { _f, _f, _f, _f, _f, _f, _F }, // -7  Cb mix
    { _f, _f, _f, _f, _f, _f, _f }, // -6  Gb mix
    { _f, _f, _f, _n, _f, _f, _f }, // -5  Db mix
    { _n, _f, _f, _n, _f, _f, _f }, // -4  Ab mix
    { _n, _f, _f, _n, _n, _f, _f }, // -3  Eb mix
    { _n, _n, _f, _n, _n, _f, _f }, // -2  Bb mix
    { _n, _n, _f, _n, _n, _n, _f }, // -1  F  mix
    { _n, _n, _n, _n, _n, _n, _f }, //  0  C  mix
    { _n, _n, _n, _n, _n, _n, _n }, //  1  G  mix
    { _n, _n, _n, _s, _n, _n, _n }, //  2  D  mix
    { _s, _n, _n, _s, _n, _n, _n }, //  3  A  mix
    { _s, _n, _n, _s, _s, _n, _n }, //  4  E  mix
    { _s, _s, _n, _s, _s, _n, _n }, //  5  B  mix
    { _s, _s, _n, _s, _s, _s, _n }, //  6  F# mix
    { _s, _s, _s, _s, _s, _s, _n }  //  7  C# mix
}};


const std::array<std::array<enum Accid, 7>, 15> Ton::LOCRIAN =
{{
    { _f, _f, _F, _f, _f, _f, _F }, // -7  Ab loc -> G#
    { _f, _f, _f, _f, _f, _f, _F }, // -6  Eb loc -> D#
    { _f, _f, _f, _f, _f, _f, _f }, // -5  Bb loc
    { _f, _f, _f, _n, _f, _f, _f }, // -4  F  loc
    { _n, _f, _f, _n, _f, _f, _f }, // -3  C  loc
    { _n, _f, _f, _n, _n, _f, _f }, // -2  G  loc
    { _n, _n, _f, _n, _n, _f, _f }, // -1  D  loc
    { _n, _n, _f, _n, _n, _n, _f }, //  0  A  loc
    { _n, _n, _n, _n, _n, _n, _f }, //  1  E  loc
    { _n, _n, _n, _n, _n, _n, _n }, //  2  B  loc
    { _n, _n, _n, _s, _n, _n, _n }, //  3  F# loc
    { _s, _n, _n, _s, _n, _n, _n }, //  4  C# loc
    { _s, _n, _n, _s, _s, _n, _n }, //  5  G# loc
    { _s, _s, _n, _s, _s, _n, _n }, //  6  D# loc
    { _s, _s, _n, _s, _s, _s, _n }  // 7  A# loc -> Bb
}};


const std::array<std::array<accids_t, 7>, 15> Ton::MAJ_BLUES =
{{
    { _f_, _f_, _Ff, _UU, _f_, _f_, _UU }, // -7  Cb maj blues -> B
    { _UU, _f_, _f_, _UU, _f_, _f_, _Ff }, // -6  Gb maj blues -> Gb
    { _UU, _f_, _f_, _fn, _UU, _f_, _f_ }, // -5  Db maj blues
    { _fn, _UU, _f_, _n_, _UU, _f_, _f_ }, // -4  Ab maj blues
    { _n_, _UU, _f_, _n_, _fn, _UU, _f_ }, // -3  Eb maj blues
    { _n_, _fn, _UU, _n_, _n_, _UU, _f_ }, // -2  Bb maj blues
    { _n_, _n_, _UU, _n_, _n_, _fn, _UU }, // -1  F  maj blues
    { _n_, _n_, _fn, _UU, _n_, _n_, _UU }, //  0  C  maj blues
    { _UU, _n_, _n_, _UU, _n_, _n_, _fn }, //  1  G  maj blues
    { _UU, _n_, _n_, _ns, _UU, _n_, _n_ }, //  2  D  maj blues
    { _ns, _UU, _n_, _s_, _UU, _n_, _n_ }, //  3  A  maj blues
    { _s_, _UU, _n_, _s_, _ns, _UU, _n_ }, //  4  E  maj blues
    { _s_, _ns, _UU, _s_, _s_, _UU, _n_ }, //  5  B  maj blues
    { _s_, _s_, _UU, _s_, _s_, _ns, _UU }, //  6  F# maj blues
    { _s_, _s_, _ns, _UU, _s_, _s_, _UU }  //  7  C# maj blues -> Db
}};


const std::array<std::array<accids_t, 7>, 15> Ton::MIN_BLUES =
{{
    { _f_, _f_, _Ff, _UU, _f_, _f_, _UU }, // -7  Ab min blues -> G#
    { _UU, _f_, _f_, _UU, _f_, _f_, _Ff }, // -6  Eb min blues -> D#
    { _UU, _f_, _f_, _fn, _UU, _f_, _f_ }, // -5  Bb min blues
    { _fn, _UU, _f_, _n_, _UU, _f_, _f_ }, // -4  F  min blues
    { _n_, _UU, _f_, _n_, _fn, _UU, _f_ }, // -3  C  min blues
    { _n_, _fn, _UU, _n_, _n_, _UU, _f_ }, // -2  G  min blues
    { _n_, _n_, _UU, _n_, _n_, _fn, _UU }, // -1  D  min blues
    { _n_, _n_, _fn, _UU, _n_, _n_, _UU }, //  0  A  min blues
    { _UU, _n_, _n_, _UU, _n_, _n_, _fn }, //  1  E  min blues
    { _UU, _n_, _n_, _ns, _UU, _n_, _n_ }, //  2  B  min blues
    { _ns, _UU, _n_, _s_, _UU, _n_, _n_ }, //  3  F# min blues
    { _s_, _UU, _n_, _s_, _ns, _UU, _n_ }, //  4  C# min blues
    { _s_, _ns, _UU, _s_, _s_, _UU, _n_ }, //  5  G# min blues
    { _s_, _s_, _UU, _s_, _s_, _ns, _s_ }, //  6  D# min blues
    { _s_, _s_, _ns, _UU, _s_, _s_, _UU }  //  7  A# min blues -> Bb
}};


// two augmented chords
const std::array<std::array<accids_t, 7>, 12> Ton::AUGMENTED =
{{
//  { _f_, _f_, _Ff, _UU, _f_, _f_, _UU }, // -7  Cb aug -> B
//  { _UU, _f_, _f_, _UU, _f_, _f_, _Ff }, // -6  Gb aug -> Gb
    { _n_, _f_, _UU, _fn, _UU, _fn, _UU }, // -5  Db aug
    { _fn, _UU, _fn, _UU, _n_, _f_, _UU }, // -4  Ab aug
    { _UU, _n_, _f_, _UU, _fn, _UU, _fn }, // -3  Eb aug
    { _UU, _fn, _UU, _ns, _UU, _n_, _f_ }, // -2  Bb aug
    { _ns, _UU, _n_, _n_, _UU, _fn, _UU }, // -1  F  aug
    { _n_, _UU, _fn, _UU, _ns, _UU, _n_ }, //  0  C  aug
    { _UU, _ns, _UU, _s_, _n_, _UU, _fn }, //  1  G  aug
    { _s_, _n_, _UU, _ns, _UU, _ns, _UU }, //  2  D  aug
    { _ns, _UU, _ns, _UU, _s_, _n_, _UU }, //  3  A  aug
    { _UU, _s_, _n_, _UU, _ns, _UU, _ns }, //  4  E  aug
    { _UU, _ns, _UU, _sS, _UU, _s_, _n_ }, //  5  B  aug
    { _sS, _UU, _s_, _s_, _UU, _ns, _UU }  //  6  F# aug
//  { _s_, _s_, _ns, _UU, _s_, _s_, _UU }  //  7  C# aug -> Db
}};


const std::array<std::array<accids_t, 7>, 3> Ton::DIMINISHED_WH =
{{
    { _s_, _n_, _n_, _n_, _n_, _f_, _fn }, // -1 F dim wh = Ab (-4), D (2), B (5)
    { _n_, _n_, _f_, _ns, _s_, _n_, _n_ }, //  0 C dim wh = Eb (-3), A (3), F# (6)
    { _ns, _s_, _n_, _s_, _n_, _n_, _f_ }  //  1 G dim wh = Db (-5), Bb (-2), E (4)
}};
 

const std::array<std::array<accids_t, 7>, 3> Ton::DIMINISHED_HW =
{{
    { _n_, _n_, _f_, _n_, _f_, _fn, _n_ }, // -1 F dim hw = Ab (-4), D (2), B (5)
    { _n_, _f_, _fn, _s_, _n_, _n_, _f_ }, //  0 C dim hw = Eb (-3), A (3), F# (6)
    { _s_, _n_, _n_, _n_, _n_, _f_, _fn }  //  1 G dim hw = Bb (-2), E (4), C# (7)
//  { _f_, _fn, _n_, _n_, _n_, _f_, _f_ }, // -5 G dim hw = Bb (-2), E (4), Db (-5)
}};


const std::array<std::array<accids_t, 7>, 2> Ton::WHOLE_TONE =
{{
    { _n_, _n_, _n_, _s_, _s_, _s_, _UU }, // 0 C whole tone
                          // = D (2), E (4), F# (6, -6eq), G# (-4eq), A# (-2eq)
    { _UU, _f_, _f_, _n_, _n_, _n_, _n_ }  // 5 1 whole tone
                          // = Db (-5), Eb (-3), F (-1), G (1), B (5)
}};


/// @todo uncomplete
const std::array<std::array<accids_t, 7>, 18> Ton::CHROMA =
{{
    { _f_, _f_, _Ff, _UU, _f_, _f_, _UU }, // -7  Cb chromatic
    { _UU, _f_, _f_, _UU, _f_, _f_, _Ff }, // -6  Gb chromatic
    { _UU, _f_, _f_, _fn, _UU, _f_, _f_ }, // -5  Db chromatic
    { _fn, _UU, _f_, _n_, _UU, _f_, _f_ }, // -4  Ab chromatic
    { _n_, _UU, _f_, _n_, _fn, _UU, _f_ }, // -3  Eb chromatic
    { _n_, _fn, _UU, _n_, _n_, _UU, _f_ }, // -2  Bb chromatic
    { _n_, _n_, _UU, _n_, _n_, _fn, _UU }, // -1  F  chromatic
    { _n_, _fn, _fn, _ns, _n_, _fn, _fn }, //  0  C  chromatic
    { _ns, _n_, _fn, _ns, _n_, _fn, _fn }, //  1  G  chromatic
    { _n_, _fn, _ns, _ns, _n_, _fn, _ns }, //  2  D  chromatic
    { _ns, _UU, _n_, _s_, _UU, _n_, _n_ }, //  3  A  chromatic
    { _s_, _UU, _n_, _s_, _ns, _UU, _n_ }, //  4  E  chromatic
    { _s_, _ns, _UU, _s_, _s_, _UU, _n_ }, //  5  B  chromatic
    { _s_, _s_, _UU, _s_, _s_, _ns, _UU }, //  6  F# chromatic
    { _s_, _s_, _ns, _UU, _s_, _s_, _UU },  // 7  C# chromatic
    { _s_, _s_, _ns, _UU, _s_, _s_, _UU },  // 8  G# chromatic
    { _s_, _s_, _ns, _UU, _s_, _s_, _UU },  // 9  D# chromatic
    { _s_, _s_, _ns, _UU, _s_, _s_, _UU },  // 10 A# chromatic
}};


const std::array<int, 12> Ton::CHROMA_NAMES =
{{ 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6 }};


} // end namespace pse
