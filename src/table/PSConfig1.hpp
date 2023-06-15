//
//  PSConfig1.hpp
//  qparse
//
//  Created by Florent Jacquemard on 14/03/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSConfig1_hpp
#define PSConfig1_hpp


#include <iostream>
#include <assert.h>
#include <array>
#include <vector>

#include "trace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"
#include "PSConfig0.hpp"
#include "PSConfig.hpp"


namespace pse {


/// target config of transition,
/// reached from its predecessor by spelling one single note
/// (spelling = choosing a name/accidental for one MIDI note).
/// It extends of PSC0 with annotations for the note read:
/// - the name choosen for the input note
/// - a print flag (the accidental or natural must be printed)
///
/// Configurations of this class are always non-initial
/// in a best path solution for pitch spelling.
class PSC1 : public PSC
{
    
public:

    /// target PS config for a transition from given (previous) config,
    /// when reading a pitch.
    /// copy and update with given accident for given name and accidental,
    /// in given conjectured global tonality.
    /// only the cost (number of accidentals) is updated.
    /// @param c previous config (origin), to be updated with the received pitch.
    // @param mp midi pitch to be named, used to update the config.
    /// @param e an enumerator of notes read for transition tp this configs.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param ton conjectured main (global) tonality (key signature).
    PSC1(std::shared_ptr<const PSC0> c,
         const PSEnum& e,
         const enum NoteName& name, const enum Accid& accid,
         const Ton& ton);

//    PSC1(const PSC0& c,
//         const PSEnum& e,               //unsigned int mp,
//         const NoteName& name, const Accid& accid,
//         const Ton& ton);
    
    /// alternative target PS config for a transition
    /// from a given (previous) PS config, when reading a pitch.
    /// copy and update with given accident for given name and accidental,
    /// in given conjectured global tonality and local tonality.
    /// the cost (number of accidentals) and distance (to local ton) are updated.
    /// @param c previous config, to be updated with the received pitch.
    // @param p pitch to be named, used to update the config.
    /// @param e an enumerator of notes read for transition tp this configs.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param acc chosen alteration for the received pitch, in -2..2.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    PSC1(std::shared_ptr<const PSC0> c,
         const PSEnum& e,               
         const enum NoteName& name, const enum Accid& acc,
         const Ton& ton, const Ton& lton);

//    PSC1(const PSC0& c,
//         const PSEnum& e,               //unsigned int mp,
//         const NoteName& name, const Accid& acc,
//         const Ton& ton, const Ton& lton);

    /// copy constructor
    PSC1(const PSC1& c);
    
    virtual ~PSC1();
    
    /// assignement operator
    PSC1& operator=(const PSC1& rhs);

    /// configs have the same list of accidentals
    bool operator==(const PSC1& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC1& rhs) const;
    
    /// midi pitch of the note read for the transition
    /// from this config's predecessor to this config.
    unsigned int midi() const;
    
    /// name of the note read for the transition from this config's predecessor.
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @see NoteName.hpp
    /// @warning not formatted for a Pitch object.
    enum NoteName name() const;
    
    /// accidental of the note read for the transition
    /// from this config's predecessor. in -2..2
    /// @see Accidental.hpp
    /// @warning not formatted for a Pitch object.
    enum Accid accidental() const;
    
    /// whether the accidental of the note
    /// (read for the transition from predecessor)
    /// must be printed or not.
    bool printed() const;
    
    /// this configuration was reached by reading a single note.
    /// Always true for this class.
    virtual bool fromNote() const;

    /// this configuration was reached by reading
    /// several simultaneous notes (an interval or a "chord").
    /// Always false for this class.
    virtual bool fromChord() const;

private:
        
    /// MIDI pitch of the note read for the transition to this config.
    unsigned int _midi;

    /// a chosen pitch name, in 0..6 (0 is 'C', 6 is 'B'),
    /// for the note read for the transition to this config.
    enum NoteName _name;
    
    // a chosen number of accidents, in -2..2.
    // @todo TBR: it is _state[_name] by construction
    // int _accid;
    
    /// whether the accident must be printed
    /// for the note read for the transition to this config.
    bool _print;
    
    
    // update the cumulated number of accidents for this Config,
    // with the number of printed alterations of this config.
    // void updateAccidents();

};


} // namespace pse

#endif /* PSConfig_hpp */

/// @}
