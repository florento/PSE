//
//  PSState2.cpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//

#include "PSState2.hpp"

namespace pse {

// const size_t PSSTate2::OCTAVES = Pitch::OCTAVE_MAX - Pitch::OCTAVE_MIN;


PSState2::PSState2(const Ton& ton, bool tonal):
PSState0() // allocation initial _names (all undef)
{
    // for each note name (0 is 'C', 6 is 'B').
    for (int n = 0; n < 7; ++n)
    {
        // same initial accid for each octave.
        for (size_t o = 0; o < OCTAVES; ++o)
        {
            if (tonal)
            {
                // singleton
                _state.at(n).at(o) = Accids::encode(ton.accidKey(n));
            }
            else
            {
                _state.at(n).at(o) = ton.accidScale(n);
            }
        }
    }
}


PSState2::PSState2(const PSState2& rhs):
PSState0(rhs),
_state(rhs._state) // array copy
{
    // std::copy(rhs._state.begin(), rhs._state.end(), _state.begin());
    for (int n = 0; n < 7; ++n)
    {
        for (size_t o = 0; o < OCTAVES; ++o)
        {
            _state.at(n).at(o) = rhs._state.at(n).at(o);
        }
    }
}


PSState2::~PSState2()
{ }


std::shared_ptr<PSState0> PSState2::clone() const
{
    return std::shared_ptr<PSState0>(new PSState2(*this));
}


PSState2& PSState2::operator=(const PSState2& rhs)
{
    if (this != &rhs)
    {
        // std::copy(rhs._state.begin(), rhs._state.end(), _state.begin());
        for (int n = 0; n < 7; ++n)
        {
            for (size_t o = 0; o < OCTAVES; ++o)
            {
                _state.at(n).at(o) = rhs._state.at(n).at(o);
            }
        }
    }
    return *this;
}


bool PSState2::equal(const PSState0& rhs) const
{
    return equal(dynamic_cast<const PSState2&>(rhs));
}


bool PSState2::equal(const PSState2& rhs) const
{
    for (int n = 0; n < 7; ++n)
    {
        for (size_t o = 0; o < OCTAVES; ++o)
        {
            if (_state.at(n).at(o) != rhs._state.at(n).at(o))
                return false;
        }
    }
    return true;
}


const accids_t PSState2::get(const enum NoteName& name, int oct) const
{
    assert(name != Pitch::UNDEF_NOTE_NAME);
    assert(Pitch::check_octave(oct));
    assert(oct != Pitch::UNDEF_OCTAVE);
    size_t n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    size_t o = oct - Pitch::OCTAVE_MIN;
    assert(o < OCTAVES);
    return _state[n][o];
}


bool PSState2::set(const accids_t a, const enum NoteName& name, int oct)
{
    assert(name != Pitch::UNDEF_NOTE_NAME);
    assert(Pitch::check_octave(oct));
    assert(oct != Pitch::UNDEF_OCTAVE);
    size_t n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    size_t o = oct - Pitch::OCTAVE_MIN;
    assert(o < OCTAVES);

    if (_state[n][o] == a)
    {
        return false;
    }
    else
    {
        _state[n][o] = a;
        return true;
    }
}




} // end namespace pse
