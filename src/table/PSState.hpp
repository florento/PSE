//
//  PSState.hpp
//  squant2
//
//  Created by Florent Jacquemard on 14/02/2021.
//  Copyright © 2021 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSState_hpp
#define PSState_hpp

#include <iostream>
#include <assert.h>
#include <array>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
//#include "Pitch.hpp"
#include "KeyFifth.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"


namespace pse {


/// An Accident State is a mapping associated one accident (in -2..2)
/// to each pitch name (in 0..6).
/// - accident number -2 is a double flat
/// - accident number -1 is a flat
/// - accident number  0 is a neutral 
/// - accident number  1 is a sharp
/// - accident number  2 is a double sharp
/// - pitch names correspond to  chars 'A'..'G', where 0 is 'C' and 6 is 'B'.
/// @see current diatonic pitch set in https://hal.archives-ouvertes.fr/hal-02886399v2
/// @see current diatonic scale in https://hal.archives-ouvertes.fr/hal-02162936v2
class PSState
{
public:
    
    /// initial state for a given tonality, or jokers for minor tons.
    /// @param ton a tonality.
    /// @param joker flag: true for computing the set of jokers for a minor ton,
    /// false for computing the initial state of the given ton.
    /// @warning the joker flag is obsolete.
    PSState(const Ton& ton, bool joker = false);

    /// initial state for a given tonality.
    /// @param ks a key signature given in number of flats (negative int)
    /// or number of sharps (positive int). must be in -7..7.
    /// @warning obsolete. use PSState(const Ton&).
    PSState(int ks);

    /// initial state for a given key signature.
    /// @param k a key signature.
    /// @warning obsolete. use PSState(const Ton&).
    PSState(const KeyFifth& k);
    
    /// copy constructor
    PSState(const PSState& rhs);

    /// copy and update given name with given accident
    PSState(const PSState& as,
            const enum NoteName& name, const enum Accid& accid);

    ~PSState();
    
    /// assignement operator
    PSState& operator=(const PSState& rhs);

    /// states have  the same list of accidentals
    bool operator==(const PSState& rhs) const;

    /// states have different list of accidentals
    bool operator!=(const PSState& rhs) const;

    /// same list of accidentals
    bool equal(const PSState& rhs) const;
    
    /// accidental in this state for a given pitch name.
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the number of accidents, in state, for n, in -2..2.
    const enum Accid accid(const enum NoteName& n) const;
    
    /// the given note (given by name and alt) belongs to this state.
    /// @param name a note name in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid accidental in -2..2 (-2 = double flats, 2 = double sharps).
    bool member(const enum NoteName& name, const enum Accid& accid) const;
    
    /// update this state, setting the given accident for the given name.
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @param a a number of accidentals, for n, in -2..2.
    /// @return whether this state was effectively modified.
    bool update(const enum NoteName& n, const enum Accid& a);
    
    // distance from other state of accidentals
    // unsigned int dist(const PSState& rhs) const;

    /// pointwise distance between this state of accidentals and another one.
    /// It is the number of accidentals that differ between the two states.
    /// @param rhs one state of accidentals.
    unsigned int dist(const PSState& rhs) const;

    /// distance distance between this state of accidentals and a tonality.
    /// It is the number of accidentals that differ.
    /// @param ton a tonality.
    unsigned int dist(const Ton& ton) const;

    /// pointwise distance between this state of accidentals and
    /// two other states of accidentals.
    /// @param astate one state of accidentals  without undef
    /// @param ajoker one joker state of accidentals, possibly with undef.
    ///        The joker state is given the priority over the first state in
    ///        distance computation.
    /// @todo TBR, obsolete.
    unsigned int dist(const PSState& astate,
                      const PSState& ajoker) const;

    /// pointwise distance between two pairs of states of accidentals
    /// Every pair of state corresponds to a tonality.
    /// @param astate1 one state of accidentals  without undef
    /// @param ajoker1 one joker state of accidentals, possibly with undef.
    /// @param astate2 one state of accidentals  without undef
    /// @param ajoker2 one joker state of accidentals, possibly with undef.
    /// The joker state is given the priority over the resp. first state in
    /// distance computation.
    /// @todo TBR, obsolete.
    static unsigned int dist(const PSState& astate1,
                             const PSState& ajoker1,
                             const PSState& astate2,
                             const PSState& ajoker2);
private: // data

    /// association to each pitch name (in 0..6)
    /// of an accidental (in -2..2).
    std::array<enum Accid, 7> _state; // _state[7]

private: // static data

    /// abbreviations for accidentals
    static const enum Accid _2F;
    static const enum Accid _1F;
    static const enum Accid _0N;
    static const enum Accid _1S;
    static const enum Accid _2S;
    static const enum Accid __U;
    
    /// key signatures, formatted for _state, in major and minor harm modes,
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> KEYS;
    // static const int TON[15][7];

    // key signatures, formatted for _state, in minor modes,
    // for every key signature in -7..7.
    // static const std::array<std::array<int, 7>, 15> MIN;

    /// accidentals in minor harmonic mode
    /// = leading-tone, or subsemitone, or sensible;
    /// formatted for _state, for every minor harmonic mode
    /// with key signature in -7..7.
    /// accidental in -2..2 or 9 = no accidental.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_HARM;

    /// accidentals in minor natural mode
    /// = leading-tone, or subsemitone, or sensible;
    /// formatted for _state, for every minor harmonic mode
    /// with key signature in -7..7.
    /// accidental in -2..2 or 9 = no accidental.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_NAT;

    /// accidentals in minor melodic mode
    /// = leading-tone, or subsemitone, or sensible;
    /// formatted for _state, for every minor harmonic mode
    /// with key signature in -7..7.
    /// accidental in -2..2 or 9 = no accidental.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_MEL;

    /// accidental in this state for a given pitch name.
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the number of accidents, in state, for n, in -2..2.
    enum Accid accid(int n) const;

};


} // namespace pse

#endif /* PsState_hpp */

/// @}

