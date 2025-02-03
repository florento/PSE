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

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Accids.hpp"
//#include "Pitch.hpp"
#include "KeyFifth.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"


namespace pse {


/// An Accident State is a mapping associating one accident (in -2..2)
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
    
    /// initial state for a given tonality.
    /// @param ton a tonality.
    /// @param tonal flag:
    /// - if true: tonal mode:
    ///   compute the initial state only according to the KS of ton,
    ///   i.e. initial state is the one of the major mode (whatever the actual mode)
    /// - if false: modal mode:
    ///   compute the initial state according to the actual mode,
    ///   i.e. discount alterations of the mode that do not belong to the KS.
    PSState(const Ton& ton, bool tonal = true);

    // initial state for a given tonality.
    // @param ks a key signature given in number of flats (negative int)
    // or number of sharps (positive int). must be in -7..7.
    // @warning TBR obsolete. use PSState(const Ton&).
    // PSState(int ks);

    // initial state for a given key signature.
    // @param k a key signature.
    // @warning TBR obsolete. use PSState(const Ton&).
    // PSState(const KeyFifth& k);
    
    /// copy constructor
    PSState(const PSState& rhs);

    /// copy and update a given name with a given accidental.
    /// @todo TBR obsolete. not used.
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
    
    /// accidental(s) in this state for a given pitch name.
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidents, in state, for note n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    const accids_t accids(const enum NoteName& n) const;
    
    /// accidental in this state for a given pitch name
    /// (when there is exactly one possibility).
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidents, in state, for note n.
    /// @warning the value of state at n must be single.
    const enum Accid accid(const enum NoteName& n) const;
    
    /// the given note (given by name and accidental) belongs to this state.
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

    // pointwise distance between two pairs of states of accidentals
    // Every pair of state corresponds to a tonality.
    // @param astate1 one state of accidentals  without undef
    // @param ajoker1 one joker state of accidentals, possibly with undef.
    // @param astate2 one state of accidentals  without undef
    // @param ajoker2 one joker state of accidentals, possibly with undef.
    // The joker state is given the priority over the resp. first state in
    // distance computation.
    // @todo TBR, obsolete.
    // static unsigned int dist(const PSState& astate1,
    //                          const PSState& ajoker1,
    //                          const PSState& astate2,
    //                          const PSState& ajoker2);

private: // data

    /// association to each pitch name (in 0..6)
    /// of zero or one or two accidentals.
    std::array<accids_t, 7> _state; // _state[7]

private:
    
    /// accidental(s) in this state for a given pitch name.
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidents, in state, for note n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    const accids_t accids(int n) const;
    // accids_t accid(int n) const;

    
private: // static tables OBSOLETE

    /// abbreviations for accidentals
    static const enum Accid _2F;
    static const enum Accid _1F;
    static const enum Accid _0N;
    static const enum Accid _1S;
    static const enum Accid _2S;
    static const enum Accid __U;
    
    /// key signatures, formatted for _state, in major and minor harm modes,
    /// for every key signature in -7..7.
    /// @todo rm OBSOLETE. we use the tables of Ton.
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
    /// @todo rm OBSOLETE. we use the tables of Ton.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_HARM;

    /// accidentals in minor natural mode
    /// = leading-tone, or subsemitone, or sensible;
    /// formatted for _state, for every minor harmonic mode
    /// with key signature in -7..7.
    /// accidental in -2..2 or 9 = no accidental.
    /// @todo rm OBSOLETE. we use the tables of Ton.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_NAT;

    /// accidentals in minor melodic mode
    /// = leading-tone, or subsemitone, or sensible;
    /// formatted for _state, for every minor harmonic mode
    /// with key signature in -7..7.
    /// accidental in -2..2 or 9 = no accidental.
    /// @todo rm OBSOLETE. we use the tables of Ton.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_MEL;


};


} // namespace pse

#endif /* PsState_hpp */

/// @}

