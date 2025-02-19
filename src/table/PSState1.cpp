//
//  PSState1.cpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//

#include "PSState1.hpp"


namespace pse {


PSState1::PSState1(const Ton& ton, bool tonal)
{
    // for each note name (0 is 'C', 6 is 'B').
    for (int n = 0; n < 7; ++n)
    {
        if (tonal)
        {
            _state.at(n) = Accids::encode(ton.accidKey(n)); // singleton
        }
        else
        {
            _state.at(n) = ton.accidScale(n);
        }
    }
}


PSState1::PSState1(const PSState1& rhs):
_state(rhs._state) // array copy
{ }


PSState1::~PSState1()
{ }


std::shared_ptr<PSState0> PSState1::clone() const
{
    return std::shared_ptr<PSState0>(new PSState1(*this));
}


PSState1& PSState1::operator=(const PSState1& rhs)
{
    if (this != &rhs)
    {
        //_state = rhs._state;
        for (size_t i = 0; i < 7; ++i)
            _state[i] = rhs._state[i];
    }
    return *this;
}


bool PSState1::equal(const PSState0& rhs) const
{
    return equal(dynamic_cast<const PSState1&>(rhs));
}


bool PSState1::equal(const PSState1& rhs) const
{
    for (size_t i = 0; i < 7; ++i)
    {
        if (_state[i] != rhs._state[i])
            return false;
    }
    return true;
}


const accids_t PSState1::get(const enum NoteName& name, int oct) const
{
    assert(name != Pitch::UNDEF_NOTE_NAME);
    //assert(oct == Pitch::UNDEF_OCTAVE);
    size_t n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    return _state[n];
}


bool PSState1::set(const accids_t a, const enum NoteName& name, int oct)
{
    assert(name != Pitch::UNDEF_NOTE_NAME);
    //assert(oct == Pitch::UNDEF_OCTAVE);
    size_t n = toint(name);
    assert(0 <= n);
    assert(n <= 6);
    
    if (_state[n] == a)
    {
        return false;
    }
    else
    {
        _state[n] = a;
        return true;
    }
}


} // end namespace pse
