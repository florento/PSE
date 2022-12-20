//
//  AccidentState.cpp
//  squant2
//
//  Created by Florent Jacquemard on 14/02/2021.
//  Copyright © 2021 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#include "AccidentState.hpp"


namespace pse {

// int AccidState::undef       =  9;
// int AccidState::triplesharp =  3;
// int AccidState::doublesharp =  2;
// int AccidState::sharp       =  1;
// int AccidState::natural     =  0;
// int AccidState::flat        = -1;
// int AccidState::doubleflat  = -2;
// int AccidState::tripleflat  = -3;

// static abbreviations for accidentals
const Accid AccidState::_2F = Accid::DoubleFlat;
const Accid AccidState::_1F = Accid::Flat;
const Accid AccidState::_0N = Accid::Natural;
const Accid AccidState::_1S = Accid::Sharp;
const Accid AccidState::_2S = Accid::DoubleSharp;
const Accid AccidState::__U = Accid::Undef;

// key signatures for major and minor tonalities
const std::array<std::array<Accid, 7>, 15> AccidState::KEYS =
{{
    { _1F, _1F, _1F, _1F, _1F, _1F, _1F }, // -7  Cb maj / Ab min
    { _1F, _1F, _1F, _0N, _1F, _1F, _1F }, // -6  Gb maj / Eb min
    { _0N, _1F, _1F, _0N, _1F, _1F, _1F }, // -5  Db maj / Bb min
    { _0N, _1F, _1F, _0N, _0N, _1F, _1F }, // -4  Ab maj / F  min
    { _0N, _0N, _1F, _0N, _0N, _1F, _1F }, // -3  Eb maj / C  min
    { _0N, _0N, _1F, _0N, _0N, _0N, _1F }, // -2  Bb maj / G  min
    { _0N, _0N, _0N, _0N, _0N, _0N, _1F }, // -1  F  maj / D  min
    { _0N, _0N, _0N, _0N, _0N, _0N, _0N }, //  0  C  maj / A  min
    { _0N, _0N, _0N, _1S, _0N, _0N, _0N }, //  1  G  maj / E  min
    { _1S, _0N, _0N, _1S, _0N, _0N, _0N }, //  2  D  maj / B  min
    { _1S, _0N, _0N, _1S, _1S, _0N, _0N }, //  3  A  maj / F# min
    { _1S, _1S, _0N, _1S, _1S, _0N, _0N }, //  4  E  maj / C# min
    { _1S, _1S, _0N, _1S, _1S, _1S, _0N }, //  5  B  maj / G# min
    { _1S, _1S, _1S, _1S, _1S, _1S, _0N }, //  6  F# maj / D# min
    { _1S, _1S, _1S, _1S, _1S, _1S, _1S }  //  7  C# maj / A# min
}};

//const std::array<std::array<Accid, 7>, 15> AccidState::MIN =
//{{
//    { _1F, _1F, _1F, _1F, _1F, _1F, _1F }, // -7  Ab min
//    { _1F, _0N, _1F, _1F, _1F, _1F, _1F }, // -6  Eb min
//    { _1F, _0N, _1F, _1F, _0N, _1F, _1F }, // -5  Bb min
//    { _0N, _0N, _1F, _1F, _0N, _1F, _1F }, // -4  F  min
//    { _0N, _0N, _1F, _0N, _0N, _1F, _1F }, // -3  C  min
//    { _0N, _0N, _1F, _0N, _0N, _1F, _0N }, // -2  G  min
//    { _0N, _0N, _0N, _0N, _0N, _1F, _0N }, // -1  D  min
//    { _0N, _0N, _0N, _0N, _0N, _0N, _0N }, //  0  A  min
//    { _0N, _1S, _0N, _0N, _0N, _0N, _0N }, //  1  E  min
//    { _0N, _1S, _0N, _0N, _1S, _0N, _0N }, //  2  B  min
//    { _1S, _1S, _0N, _0N, _1S, _0N, _0N }, //  3  F# min
//    { _1S, _1S, _0N, _1S, _1S, _0N, _0N }, //  4  C# min
//    { _1S, _1S, _0N, _1S, _1S, _0N, _1S }, //  5  G# min
//    { _1S, _1S, _1S, _1S, _1S, _0N, _1S }, //  6  D# min
//    { _1S, _1S, _1S, _1S, _1S, _1S, _1S }  //  7  A# min
//}};

// accidentals in minor harmonic.__U == no accid.
// min harm scale = MAJ + this mask
const std::array<std::array<Accid, 7>, 15> AccidState::MIN_HARM =
{{
    { __U, __U, __U, __U, _0N, __U, __U }, // -7  Ab min harm
    { __U, _0N, __U, __U, __U, __U, __U }, // -6  Eb min harm
    { __U, __U, __U, __U, __U, _0N, __U }, // -5  Bb min harm
    { __U, __U, _0N, __U, __U, __U, __U }, // -4  F  min harm
    { __U, __U, __U, __U, __U, __U, _0N }, // -3  C  min harm
    { __U, __U, __U, _1S, __U, __U, __U }, // -2  G  min harm
    { _1S, __U, __U, __U, __U, __U, __U }, // -1  D  min harm
    { __U, __U, __U, __U, _1S, __U, __U }, //  0  A  min harm
    { __U, _1S, __U, __U, __U, __U, _0N }, //  1  E  min harm
    { __U, __U, __U, __U, __U, _1S, __U }, //  2  B  min harm
    { __U, __U, _1S, __U, __U, __U, __U }, //  3  F# min harm
    { __U, __U, __U, __U, __U, __U, _1S }, //  4  C# min harm
    { __U, __U, __U, _2S, __U, __U, __U }, //  5  G# min harm
    { _2S, __U, __U, __U, __U, __U, __U }, //  6  D# min harm
    { __U, __U, __U, __U, _2S, __U, __U },  // 7  A# min harm
}};

// accidentals in minor natural.__U == no accid.
const std::array<std::array<Accid, 7>, 15> AccidState::MIN_NAT =
{{
    { __U, __U, __U, __U, __U, __U, __U }, // -7  Ab min harm
    { __U, __U, __U, __U, __U, __U, __U }, // -6  Eb min harm
    { __U, __U, __U, __U, __U, __U, __U }, // -5  Bb min harm
    { __U, __U, __U, __U, __U, __U, __U }, // -4  F  min harm
    { __U, __U, __U, __U, __U, __U, __U }, // -3  C  min harm
    { __U, __U, __U, __U, __U, __U, __U }, // -2  G  min harm
    { __U, __U, __U, __U, __U, __U, __U }, // -1  D  min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  0  A  min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  1  E  min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  2  B  min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  3  F# min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  4  C# min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  5  G# min harm
    { __U, __U, __U, __U, __U, __U, __U }, //  6  D# min harm
    { __U, __U, __U, __U, __U, __U, __U },  // 7  A# min harm
}};

// accidentals in minor melodic.__U == no accid.
// min mel scale = MAJ + this mask
const std::array<std::array<Accid, 7>, 15> AccidState::MIN_MEL =
{{
    { __U, __U, __U, _0N, _0N, __U, __U }, // -7  Ab min harm
    { _0N, _0N, __U, __U, __U, __U, __U }, // -6  Eb min harm
    { __U, __U, __U, __U, _0N, _0N, __U }, // -5  Bb min harm
    { __U, _0N, _0N, __U, __U, __U, __U }, // -4  F  min harm
    { __U, __U, __U, __U, __U, _0N, _0N }, // -3  C  min harm
    { __U, __U, _0N, _1S, __U, __U, __U }, // -2  G  min harm
    { _1S, __U, __U, __U, __U, __U, _0N }, // -1  D  min harm
    { __U, __U, __U, _1S, _1S, _0N, _0N }, //  0  A  min harm
    { _1S, _1S, __U, __U, __U, __U, __U }, //  1  E  min harm
    { __U, __U, __U, __U, _1S, _1S, __U }, //  2  B  min harm
    { __U, _1S, _1S, __U, __U, __U, __U }, //  3  F# min harm
    { __U, __U, __U, __U, __U, _1S, _1S }, //  4  C# min harm
    { __U, __U, _1S, _2S, __U, __U, __U }, //  5  G# min harm
    { _2S, __U, __U, __U, __U, __U, _1S }, //  6  D# min harm
    { __U, __U, __U, _2S, _2S, __U, __U },  // 7  A# min harm
}};



AccidState::AccidState(const Ton& ton, bool joker)
{
    int k = ton.fifths();
    assert(-7 <= k);
    assert(k <= 7);
    size_t i = k + 7;
    const Ton::Mode m = ton.mode();
    if (joker)
    {
        switch (m)
        {
            case Ton::Mode::Maj:
                _state.fill(Accid::Undef);
                break;
                
            case Ton::Mode::Min:
                _state = MIN_HARM[i];
                break;
                
            case Ton::Mode::MinNat:
                _state = MIN_NAT[i];
                break;
                
            case Ton::Mode::MinMel:
                _state = MIN_MEL[i];
                break;
                
            case Ton::Mode::Ionian:
            case Ton::Mode::Dorian:
            case Ton::Mode::Phrygian:
            case Ton::Mode::Lydian:
            case Ton::Mode::Mixolydian:
            case Ton::Mode::Eolian:
            case Ton::Mode::Locrian:
            {
                WARN("AccidState Accid::undef for mode"); // m
                _state.fill(Accid::Undef);
                break;
            }
                
            case Ton::Mode::Undef:
            default:
            {
                ERROR("unexpected Ton mode");
                break;
            }
        }
    }
    else
    {
        switch (m)
        {
            case Ton::Mode::Maj:
            case Ton::Mode::Min:
            case Ton::Mode::MinNat:
            case Ton::Mode::MinMel:
                _state = KEYS[i];
                break;
                
            case Ton::Mode::Ionian:
            case Ton::Mode::Dorian:
            case Ton::Mode::Phrygian:
            case Ton::Mode::Lydian:
            case Ton::Mode::Mixolydian:
            case Ton::Mode::Eolian:
            case Ton::Mode::Locrian:
                WARN("AccidState Accid::undef for mode");
                _state.fill(Accid::Undef);
                break;
                
            case Ton::Mode::Undef:
            default:
                ERROR("unexpected Ton mode");
                break;
        }
    }
}


// obsolete
AccidState::AccidState(int ks):
_state(KEYS[ks+7])
{
    assert(-7 <= ks);
    assert(ks <= 7);
}


// obsolete
AccidState::AccidState(const KeyFifth& k)
{
    if (k.flats() > 0)
    {
        assert(k.flats()  <= 7);
        _state = KEYS[7 - k.flats()];
    }
    else
    {
        assert(k.sharps() <= 7);
        _state = KEYS[k.sharps() + 7];
    }
}


AccidState::AccidState(const AccidState& as):
_state(as._state)
{ }


AccidState::AccidState(const AccidState& as,
                       const NoteName& name, const Accid& accid):
_state(as._state)
{
    int n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    _state[n] = accid;
}


AccidState::~AccidState()
{ }


AccidState& AccidState::operator=(const AccidState& rhs)
{
    if (this != &rhs)
    {
        //_state = rhs._state;
        for (size_t i = 0; i < 7; ++i)
            _state[i] = rhs._state[i];
    }
    return *this;
}


bool AccidState::operator==(const AccidState& rhs) const
{
    return equal(rhs);
}


bool AccidState::operator!=(const AccidState& rhs) const
{
    return (! operator==(rhs));
}


bool AccidState::equal(const AccidState& rhs) const
{
    for (size_t i = 0; i < 7; ++i)
    {
        if (_state[i] != rhs._state[i])
            return false;
    }
    return true;
}


Accid AccidState::accid(int n) const
{
    assert(0 <= n);
    assert(n <= 6);
    return _state[n];
}


const Accid AccidState::accid(const NoteName& name) const
{
    return Accid(accid(toint(name)));
}


bool AccidState::member(const NoteName& name,
                        const Accid& accid) const
{
    int n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    return (_state[n] == accid);
}


bool AccidState::update(const NoteName& name, const Accid& accid)
{
    int n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    // accidental of n is unchanged
    if (_state[n] == accid)
    {
        return false;
    }
    else // real update
    {
        _state[n] = accid;
        return true;
    }
}


unsigned int AccidState::dist(const AccidState& rhs) const
{
    unsigned int res = 0;
    
    for (size_t i = 0; i < 7; ++i) // pitch names
    {
        if (_state[i] != rhs._state[i])
        {
            res += 1;
        }
    }
    return res;
}


unsigned int AccidState::dist(const Ton& ton) const
{
    unsigned int res = 0;
    
    for (int i = 0; i < 7; ++i) // pitch names
    {
        NoteName n = NNofint(i); // encapsulation
        if (_state[i] != ton.accidDia(n)) // or ton.accidKey(i) ?
        {
            res += 1;
        }
    }
    return res;
    // OLD version
    // return dist(AccidState(ton, false), AccidState(ton, true));
}


// not used ?
//unsigned int AccidState::dist(const AccidState& astate,
//                              const AccidState& ajoker) const
//{
//    unsigned int res = 0;
//    
//    for (int i = 0; i < 7; ++i) // pitch names
//    {
//        const NoteName n = NNofint(i); // encapsulation
//        //Accid a = accid(n);
//        assert(accid(i) != Accid::Undef);
//        const int jo = ajoker.accid(i);
//        if (jo == Accid::Undef)
//        {
//            // none of them should be undef
//            assert(astate.accid(i) != Accid::Undef);
//            res += astate.accid(n).dist(accid(n));
//        }
//        else
//        {
//            const Accid ajo(jo);
//            res += ajo.dist(accid(n));
//        }
//    }
//    return res;
//}


// static
// not used ?
unsigned int AccidState::dist(const AccidState& astate1,
                              const AccidState& ajoker1,
                              const AccidState& astate2,
                              const AccidState& ajoker2)
{
    unsigned int res = 0;
    
    for (int i = 0; i < 7; ++i) // pitch names
    {
        const NoteName n = NNofint(i); // encapsulation
        const Accid& jo1 = ajoker1.accid(n);
        const Accid& jo2 = ajoker2.accid(n);
        // assert(astate1.accid(i) != Accid::undef);
        // assert(astate2.accid(i) != Accid::undef);

        const Accid& a1 = (defined(jo1))?jo1:astate1.accid(n);
        const Accid& a2 = (defined(jo2))?jo2:astate2.accid(n);
        assert(a1 != Accid::Undef);
        assert(a2 != Accid::Undef);
        res += accidDist(a1, a2);
    }
    return res;
}




} // end namespace pse

/// @}
