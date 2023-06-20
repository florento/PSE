//
//  PSConfig2.hpp
//  pse
//
//  Created by Florent Jacquemard on 04/02/2023.
//
/// @addtogroup pitch
/// @{


#ifndef PSConfig2_hpp
#define PSConfig2_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <array>
#include <vector>

#include "trace.hpp"
#include "Accidental.hpp"
#include "PSConfig0.hpp"
#include "PSConfig.hpp"
#include "PSEnum.hpp"
#include "PSChord.hpp"
#include "PSConfig1c.hpp"



namespace pse {


/// target config of transition,
/// reached from its predecessor by spelling several simultaneous notes
/// (a "chord").
/// It extends of PSC0 with annotations for the chord read:
/// - the chosen names for the input notes
/// - print flags
/// see PSC1 for the meaning of these extensions.
///
/// Configurations of this class are always non-initial
/// in a best path solution for pitch spelling.
class PSC2 : public PSC
{
    
public:

    /// target PS config for a transition from given (previous) config,
    /// when reading a chord.
    /// @param c0 previous config, to be updated with the received chord.
    /// @param c1 last config reached after processing the chord.
    /// @param chord note enumerator containing the notes of the read chord.
    PSC2(std::shared_ptr<const PSC0> c0,
         std::shared_ptr<const PSC1c> c1, PSChord& chord);

    /// copy constructor
    PSC2(const PSC2& c);
    
    virtual ~PSC2();
    
    /// assignement operator
    PSC2& operator=(const PSC2& rhs);

    /// configs have the same list of accidentals
    bool operator==(const PSC2& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC2& rhs) const;
    
    // bool acceptable(const enum NoteName& name, const enum Accid& accid) const;
    
    /// number of simultaneous note in chord read to reach this config.
    size_t size() const;

    /// midi pitch of the ith note read for the transition
    /// from this config's predecessor.
    /// @param i index of the note in the input chord in 0..size()-1.
    unsigned int midi(size_t i) const;

    /// name of the note read for the transition from this config's predecessor.
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of the note in the input chord in 0..size()-1.
    /// @see NoteName.hpp
    /// @warning not formatted for a Pitch object.
    enum NoteName name(size_t i) const;

    /// const iterator pointig to the first name of note in this config.
    std::vector<enum NoteName>::const_iterator cbeginName() const;

    /// const iterator pointig to the one after last name of note in this config.
    std::vector<enum NoteName>::const_iterator cendName() const;

    /// accidental of the note read for the transition
    /// from this config's predecessor. in -2..2
    /// @param i index of the note in the input chord in 0..size()-1.
    /// @see Accidental.hpp
    /// @warning not formatted for a Pitch object.
    enum Accid accidental(size_t i) const;

    /// whether the accidental of the note (read for the transition from predecessor)
    /// must be printed or not.
    /// @param i index of the note in the input chord in 0..size()-1.
    bool printed(size_t i) const;

    /// const iterator pointig to the first print flag in this config.
    std::vector<bool>::const_iterator cbeginPrint() const;

    /// const iterator pointig to the one after last print flag in this config.
    std::vector<bool>::const_iterator cendPrint() const;

    /// this configuration was reached by reading a single note.
    /// Always false for this class.
    virtual bool fromNote() const;

    /// this configuration was reached by reading
    /// several simultaneous notes (a "chord").
    /// Always true for this class.
    virtual bool fromChord() const;
    
private: // data
        
    // representation of the sequence of simultaneous notes
    // read to reach this config from previous config.
    // std::shared_ptr<const PSChord> _chord;

    // index of the next pitch class number to process in the input chord.
    // unsigned int _current;
    
    /// MIDI pitchs of the notes read for the transition to this config.
    std::vector<unsigned int> _midis;

    /// chosen pitch names, in 0..6 (0 is 'C', 6 is 'B'),
    /// for the chord read for the transition to this config.
    std::vector<enum NoteName> _names;
    
    /// chosen number of accidents, in -2..2.
    // @todo TBR: it is _state[_name] by construction
    std::vector<enum Accid> _accids;
   
    /// whether the accident must be printed
    /// for the note read for the transition to this config.
    std::vector<bool> _prints;

};


} // namespace pse

#endif /* PSConfig2_hpp */

/// @}
