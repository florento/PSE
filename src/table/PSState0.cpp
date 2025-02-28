//
//  PSState0.cpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//

#include "PSState0.hpp"
#include "Enharmonic.hpp"

namespace pse {


PSState0::PSState0()
{
    _names.fill(NoteName::Undef);
}


PSState0::PSState0(const PSState0& rhs):
_names(rhs._names) // array copy
{ }


bool PSState0::operator==(const PSState0& rhs) const
{
    // RTTI check
    assert(typeid(*this) == typeid(rhs));
    // if (typeid(*this) != typeid(rhs))
    // {
    //     ERROR("State: equality between different types");
    //     return false;
    // }

    // Invoke equal on derived types
    return equal(rhs);
}


bool PSState0::operator!=(const PSState0& rhs) const
{
    return (! operator==(rhs));
}


const accids_t PSState0::accids(const enum NoteName& name, int oct) const
{
    assert(name != Pitch::UNDEF_NOTE_NAME);
    assert(Pitch::check_octave(oct));
    // int n = toint(name);
    // assert(0 <= n);
    // assert(n <= 6);
    return get(name, oct);
}


const enum Accid PSState0::accid(const enum NoteName& name, int oct) const
{
    accids_t a = accids(name, oct);
    assert(Accids::single(a));
    return Accids::first(a);
}


const enum NoteName PSState0::currentName(unsigned int pc) const
{
    assert(pc < 12);
    for (int i = 0; i < 3; ++i)
    {
        /// @todo options for Cflat and double accids
        const enum Accid& a(Enharmonics::accid(pc, i));
        const enum NoteName n(Enharmonics::name(pc, i));
        assert(defined(a) == defined(n));
        if (defined(n) and member(n, a))
        {
            return n;
        }
    }
    // the pc is not represented in this state
    return NoteName::Undef;
}


const enum NoteName PSState0::lastName(unsigned int pc) const
{
    assert(pc < _names.size());
    return _names[pc];
}


bool PSState0::member(const enum NoteName& name,
                      const enum Accid& accid, int oct) const
{
    accids_t a = accids(name, oct);
    assert(accid != Pitch::UNDEF_ALTERATION);
    return Accids::contained(accid, a);
}


bool PSState0::update(const enum Accid& accid,
                      const enum NoteName& name, int oct)
{
    assert(name != NoteName::Undef);
    assert(accid != Accid::Undef);
    assert(Pitch::check_octave(oct));
    // int n = toint(name);
    // assert(0 <= n);
    // assert(n <= 6);
    accids_t a = get(name, oct);
    
    // associate name to pitch class
    unsigned int pc = MidiNum::pitchClass(name, accid);
    assert(pc < 12);
    _names[pc] = name;
    
    // accidental of n is unchanged
    if (Accids::single(a) && Accids::contained(accid, a))
    {
        return false;
    }
    // real update
    else
    {
        return set(Accids::encode(accid), name, oct);
    }
}


} // end namespace pse
