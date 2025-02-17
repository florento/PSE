//
//  PSState0.cpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//

#include "PSState0.hpp"

namespace pse {


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
    assert(name != Pitch::UNDEF_NOTE_NAME);
    assert(Pitch::check_octave(oct));
    // int n = toint(name);
    // assert(0 <= n);
    // assert(n <= 6);
    accids_t a = get(name, oct);
    
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
