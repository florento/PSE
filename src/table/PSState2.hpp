//
//  PSState2.hpp
//  pse
//
//  Created by Florent Jacquemard on 15/02/2025.
//
/// @addtogroup pitch
/// @{


#ifndef PSState2_hpp
#define PSState2_hpp

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

/// mapping of pitch name and octave number
/// into accidentals.
class PSState2: public PSState0
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
    PSState2(const Ton& ton, bool tonal = true);
    
    /// copy constructor.
    PSState2(const PSState2& rhs);
    
    /// destructor.
    ~PSState2();
    
    /// allocate a new copy of this state.
    std::shared_ptr<PSState0> clone() const override;
       
    /// assignement operator
    PSState2& operator=(const PSState2& rhs);
    
    public: // comparison
    
    /// same accidentals
    bool equal(const PSState0& rhs) const override;
    
    /// same list of accidentals
    bool equal(const PSState2& rhs) const;
    
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
    
    /// number of octaves.
    static const size_t OCTAVES = Pitch::OCTAVE_MAX - Pitch::OCTAVE_MIN;
    
    /// association to every pitch name (in 0..6, 0 is 'C', 6 is 'B')
    /// and every octave number
    /// of zero or one or two accidentals.
    std::array<std::array<accids_t, OCTAVES>, 7> _state;
    
};

} // namespace pse

#endif /* PSState2_hpp */

/// @}
