//
//  PSState0.hpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//
/// @addtogroup pitch
/// @{

#ifndef PSState0_hpp
#define PSState0_hpp

#include <iostream>
#include <assert.h>
#include <array>

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Accids.hpp"
#include "Pitch.hpp"
#include "KeyFifth.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"


namespace pse {

/// abstract class for a state associating one accidental to
/// every pitch names (in 0..6), and, optionaly, octave number.
class PSState0
{

public: // comparison

    /// same accidentals
    virtual bool equal(const PSState0& rhs) const = 0;
    
    /// states have  the same list of accidentals
    bool operator==(const PSState0& rhs) const;

    /// states have different list of accidentals
    bool operator!=(const PSState0& rhs) const;

public: // access

    /// accidental(s) in this state for a given pitch name and octave.
    /// @param name a pitch name, between 'C' and 'B'.
    /// @param oct an optional octave number between OCTAVE_MIN and OCTAVE_MAX
    /// or Pitch::UNDEF_OCTAVE when irrelevant.
    /// @return the accidents, in state, for note n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    /// @see same as get(name, oct)
    const accids_t accids(const enum NoteName& name,
                          int oct=Pitch::UNDEF_OCTAVE) const;
    
    /// accidental in this state for a given pitch name and octave
    /// (when there is exactly one possibility).
    /// @param name a pitch name, between 'C' and 'B'.
    /// @param oct an optional octave number between OCTAVE_MIN and OCTAVE_MAX
    /// or Pitch::UNDEF_OCTAVE when irrelevant.
    /// @return the accidents, in state, for note n.
    /// @warning the value of state at n must be single accid.
    const enum Accid accid(const enum NoteName& name,
                           int oct=Pitch::UNDEF_OCTAVE) const;
    
    /// the note given by name, accidental and octave, belongs to this state.
    /// @param name a pitch name, between 'C' and 'B'.
    /// @param accid accidental in -2..2 (-2 = double flats, 2 = double sharps).
    /// @param oct an optional octave number between OCTAVE_MIN and OCTAVE_MAX
    /// or Pitch::UNDEF_OCTAVE when irrelevant.
    bool member(const enum NoteName& name,
                const enum Accid& accid,
                int oct=Pitch::UNDEF_OCTAVE) const;

public: // modification

    /// update this state, setting the given accident for the given name
    /// and octave.
    /// @param a a number of accidentals, for n, in -2..2.
    /// @param name a pitch name, between 'C' and 'B'.
    /// @param oct an optional octave number between OCTAVE_MIN and OCTAVE_MAX
    /// or Pitch::UNDEF_OCTAVE when irrelevant.
    /// @return whether this state was effectively modified.
    bool update(const enum Accid& a,
                const enum NoteName& name,
                int oct=Pitch::UNDEF_OCTAVE);

protected: // low-level access and modification, implemented in descendants.
    
    /// get accidental(s) in this state for a given pitch name
    /// and (optionaly) octave.
    /// @param name a pitch name, between 'C' and 'B'.
    // @param n a pitch name, encoded in 0..6 (0 is 'C', 6 is 'B').
    /// @param oct an optional octave number between OCTAVE_MIN and OCTAVE_MAX
    /// or Pitch::UNDEF_OCTAVE when irrelevant.
    /// @return the accidents, in state, for given note n and octave oct.
    /// can be a single accidental value, a pair of accidents or UNDEF.
    virtual const accids_t get(const enum NoteName& name,
                               int oct=Pitch::UNDEF_OCTAVE) const = 0;

    /// set accidental(s) in this state for a given pitch name
    /// and (optionaly) octave.
    /// @param a the accidents for given note n and octave oct.
    /// @param name a pitch name, between 'C' and 'B'.
    // @param n a pitch name, encoded in 0..6 (0 is 'C', 6 is 'B').
    /// @param oct an optional octave number between OCTAVE_MIN and OCTAVE_MAX
    /// or Pitch::UNDEF_OCTAVE when irrelevant.
    /// can be a single accidental value, a pair of accidents or UNDEF.
    /// @return whether this state was effectively modified.
    virtual bool set(const accids_t a,
                     const enum NoteName& name,
                     int oct=Pitch::UNDEF_OCTAVE) = 0;
    
};

} // namespace pse

#endif /* PSState0_hpp */

/// @}
