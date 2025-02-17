//
//  PSState1.hpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//
/// @addtogroup pitch
/// @{


#ifndef PSState1_hpp
#define PSState1_hpp

#include <stdio.h>

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
#include "PSState0.hpp"


namespace pse {

/// mapping of pitch name into accidentals.
class PSState1: public PSState0
{
    
public: // construction
    
    /// initial state for a given tonality.
    /// @param ton a tonality.
    /// @param tonal flag:
    /// - if true: tonal mode:
    ///   compute the initial state only according to the KS of ton,
    ///   i.e. initial state is the one of the major mode (whatever the actual mode)
    /// - if false: modal mode:
    ///   compute the initial state according to the actual mode,
    ///   i.e. discount alterations of the mode that do not belong to the KS.
    PSState1(const Ton& ton, bool tonal = true);

    /// copy constructor.
    PSState1(const PSState1& rhs);

    /// destructor.
    ~PSState1();
    
    /// assignement operator
    PSState1& operator=(const PSState1& rhs);

public: // comparison

    /// same accidentals
    bool equal(const PSState0& rhs) const override;

    /// same list of accidentals
    bool equal(const PSState1& rhs) const;

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
                               int oct=Pitch::UNDEF_OCTAVE) const override;

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
                     int oct=Pitch::UNDEF_OCTAVE) override;

private: // data

    /// association to each pitch name (in 0..6, 0 is 'C', 6 is 'B')
    /// of zero or one or two accidentals.
    std::array<accids_t, 7> _state; // _state[7]

};


} // namespace pse

#endif /* PSState1_hpp */

/// @}
