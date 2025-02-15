//
//  PSConfig1c.hpp
//  pse
//
//  Created by Florent Jacquemard on 19/06/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PSConfig1c_hpp
#define PSConfig1c_hpp

#include <iostream>
#include <assert.h>
#include <array>
#include <vector>

#include "pstrace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Ton.hpp"
#include "PSChord.hpp"
#include "PSConfig1.hpp"



namespace pse {


/// target config of transition reached from its predecessor
/// by spelling one note in a chord.
/// It extends of PSC1 with information on the ongoing processing of the chord
/// - pitch classes already met and name associated.
class PSC1c : public PSC1
{
    
public: // construction
    
    // initial PSC1c for the processing of the given chord.
    // PSC1c(std::shared_ptr<const PSC0> c, PSChord& chord);
    // PSC1c(const PSC0& c, PSChord& chord);

    /// configuration reached with the first note in the given chord.
    /// @param c previous config, to be updated with the received pitch.
    /// @param e an enumerator of notes read for transition to this configs.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality, undef if it is not known yet.
    PSC1c(std::shared_ptr<const PSC0> c,
          const PSEnum& e,
          const enum NoteName& name,
          const enum Accid& accid,
          const Ton& gton,
          const Ton& lton = Ton());
      
    /// next PSC1c for the processing of one note in the given chord.
    /// @param c previous config (origin), to be updated with the received pitch.
    // @param chord an enumerator of notes in chord currently processed.
    /// @param name chosen name for the received pitch,
    /// or undef if the current pitch class was already met.
    /// @param accid chosen alteration for the received pitch,
    /// or undef if the current pitch class was already met.
    /// @param force_print whether the note must be counted as printed
    /// (for the computation of cost).
    /// @param gton conjectured main (global) tonality (key signature),
    /// or undef if the current pitch class was already met.
    /// @param lton conjectured local tonality,
    /// or undef if it was not yet estimated
    /// or if the current pitch class was already met.
    PSC1c(std::shared_ptr<const PSC1c> c,
          const enum NoteName& name,
          const enum Accid& accid,
          bool force_print,
          const Ton& gton,
          const Ton& lton = Ton());
    
    // next PSC1c for the processing of the given chord,
    // when the current processed pitch class was already met in the chord.
    // @param c previous config (origin), to be updated with the received pitch.
    // @param chord an enumerator of notes read for transition to this config.
    // @param ton conjectured main (global) tonality (key signature).
    // @param lton conjectured local tonality,
    // or undef if it was not yet estimated.
    // PSC1c(std::shared_ptr<const PSC1c> c,
    //       const PSChord& chord,
    //       const Ton& ton,
    //       const Ton& lton = Ton());

    /// copy constructor.
    PSC1c(const PSC1c& c);
    
    /// destructor.
    virtual ~PSC1c();
    
    /// assignement operator
    PSC1c& operator=(const PSC1c& rhs);

public: // comparison

    /// configs have the same list of accidentals
    bool operator==(const PSC1c& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC1c& rhs) const;

public: // access

    /// @param pc a pitch class in 0..11.
    /// @return if the pitch class pc was already met in chord,
    /// the associated name, or otherwise NoteName::Undef.
    enum NoteName dejavu(unsigned int pc) const;

    /// @param pc a pitch class in 0..11.
    /// @return if the pitch class pc was already met in chord,
    /// and the associated accidental must be printed.
    bool dejaprint(unsigned int pc) const;

    /// the processing of the chord is terminated.
    bool complete() const;
    
    /// the chord currently processed.
    const PSChord& chord() const;

    /// we are currently processing a chord.
    bool inChord() const override;
        
private: // data

    /// chord currently processed.
    std::shared_ptr<const PSChord> _chord;
    
    /// map associating to every pitch class in 0..12
    /// a note name, if the pitch class was encountered while processing the chord
    /// or NoteName::Undef otherwise.
    std::array<enum NoteName, 12> _pcn;

    /// map associating to every pitch class in 0..12
    /// a print flag, if the pitch class was encountered while processing the chord.
    std::array<bool, 12> _pcp;

    /// index of the note after the last note of the chord processed.
    bool _complete;
    
private: // construction
    
    /// internal constructor.
    PSC1c(const PSC& c, PSChord& chord);
    
    void post_init(const PSC0& c, const PSChord& chord);
    
    std::shared_ptr<const PSChord> init_chord(const PSC1c* c) const;
    
};


} // namespace pse

#endif /* PSConfig1c_hpp */

/// @}
