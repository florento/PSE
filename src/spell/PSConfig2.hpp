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
    /// @param c previous config, to be updated with the received chord.
    /// @param e note enumerator containing the notes of the read chord.
    /// @param i0 index of the first note of chord in the given enumerator.
    PSC2(std::shared_ptr<const PSC0> c, PSEnum& e, size_t i0);
    //PSC2(const PSC0& c, const PSChord& e);

    /// target PS config for a transition from given (previous) config,
    /// when reading a chord.
    /// copy and update with given accident for given name and accidental,
    /// in given conjectured global tonality.
    /// only the cost (number of accidentals) is updated.
    /// @param c previous config (origin), to be updated with the received pitch in a chord.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param ton conjectured main (global) tonality (key signature).
    PSC2(const PSC2& c,
         const NoteName& name, const Accid& accid,
         const Ton& ton);

    /// alternative target PS config for a transition
    /// from a given (previous) PS config, when reading a chord.
    /// copy and update with given accident for given name and accidental,
    /// in given conjectured global tonality and local tonality.
    /// the cost (number of accidentals) and distance (to local ton) are updated.
    /// @param c previous config, to be updated with the received pitch in a chord.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param acc chosen alteration for the received pitch, in -2..2.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    PSC2(const PSC2& c,
         const NoteName& name, const Accid& acc,
         const Ton& ton, const Ton& lton);

    /// copy constructor
    PSC2(const PSC2& c);
    
    virtual ~PSC2();
    
    /// assignement operator
    PSC2& operator=(const PSC2& rhs);

    /// configs have the same list of accidentals
    bool operator==(const PSC2& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC2& rhs) const;
    
    bool acceptable(const NoteName& name, const Accid& accid) const;
    
    /// number of simultaneous note read to reach this config.
    size_t size() const;

    /// the construction of this config is terminated.
    bool complete() const;
    
    /// current pitch class number to process in the input chord,
    /// in 0..12.
    inline unsigned int current() const { return _current; };

    /// the proposed update is compatible with this config.
    bool consistent(const NoteName& name, const Accid& accid) const;

    /// midi pitch of the ith note read for the transition
    /// from this config's predecessor.
    /// @param i index of the note in the input chord in 0..size()-1.
    unsigned int midi(size_t i) const;

    /// name of the note read for the transition from this config's predecessor.
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of the note in the input chord in 0..size()-1.
    /// @see NoteName.hpp
    /// @warning not formatted for a Pitch object.
    NoteName name(size_t i) const;

    /// const iterator pointig to the first name of note in this config.
    std::vector<NoteName>::const_iterator cbeginName() const;

    /// const iterator pointig to the one after last name of note in this config.
    std::vector<NoteName>::const_iterator cendName() const;

    /// accidental of the note read for the transition
    /// from this config's predecessor. in -2..2
    /// @param i index of the note in the input chord in 0..size()-1.
    /// @see Accidental.hpp
    /// @warning not formatted for a Pitch object.
    Accid accidental(size_t i) const;

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
    
private:
        
    // MIDI pitchs of the notes read for the transition to this config.
    // std::vector<unsigned int> _midi;

    /// representation of the sequence of simultaneous notes
    /// read to reach this config.
    std::shared_ptr<const PSChord> _chord;
    //const PSChord* _chord;
    //const PSChord& _chord;
    
    /// index of the next pitch class number to process in the input chord.
    unsigned int _current;
    
    /// chosen pitch names, in 0..6 (0 is 'C', 6 is 'B'),
    /// for the notes read for the transition to this config.
    std::vector<NoteName> _names;
    
    // a chosen number of accidents, in -2..2.
    // @todo TBR: it is _state[_name] by construction
    // int _accid;
    
    /// whether the accident must be printed
    /// for the note read for the transition to this config.
    std::vector<bool> _prints;

    // update the cumulated number of accidents for this Config,
    // with the number of printed alterations of this config.
    // void updateAccidents();
    
    /// state of previous config in shortest path to this config.
    const PSState& prevState() const;

    /// first pitch class number to be processed in the input chord
    unsigned int firstChroma() const;

    /// next pitch class number to be processed in the input chord
    unsigned int nextChroma() const;

    // the proposed update is compatible with this config.
    // bool valid(const NoteName& name, const Accid& accid,
    //           const Ton& ton, const Ton& lton) const;
    
    /// all occurrences of the same note receive same name and accid
    /// @return the number of names assigned
    size_t setNames(const NoteName& name, bool print);
    
    /// update the spelling cost for one pitch class in the current chord.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the acciental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @return wheter the cost value of this config was increased
    bool updateCost(const NoteName& name, const Accid& accid, bool print,
                    size_t nbocc, const Ton& ton);

    /// update the spelling cost for one pitch class in the current chord.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the acciental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    /// @return wheter the cost value of this config was increased
    bool updateCost(const NoteName& name, const Accid& accid, bool print,
                    size_t nbocc, const Ton& ton, const Ton& lton);

};


} // namespace pse

#endif /* PSConfig2_hpp */

/// @}
