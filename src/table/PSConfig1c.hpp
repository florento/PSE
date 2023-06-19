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

#include "trace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
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
public:
    
    /// initial PSC1c for the processing of the given chord.
    PSC1c(std::shared_ptr<const PSC0> c, PSChord& chord);
    // PSC1c(const PSC0& c, PSChord& chord);

    /// next PSC1c for the processing of the given chord.
    /// @param c previous config (origin), to be updated with the received pitch.
    /// @param chord an enumerator of notes read for transition to this config.
    /// @param name chosen name for the received pitch,
    /// or undef if the current pitch class was already met.
    /// @param accid chosen alteration for the received pitch,
    /// or undef if the current pitch class was already met.
    /// @param ton conjectured main (global) tonality (key signature),
    /// or undef if the current pitch class was already met.
    /// @param lton conjectured local tonality,
    /// or undef if it was not yet estimated
    /// or if the current pitch class was already met.
    PSC1c(std::shared_ptr<const PSC1c> c,
          const PSChord& chord,
          const enum NoteName& name, // = NoteName::Undef,
          const enum Accid& accid, // = Accid::Undef,
          const Ton& ton, // = Ton(),
          const Ton& lton = Ton());
    
    /// next PSC1c for the processing of the given chord,
    /// when the current processed pitch class was already met in the chord.
    /// @param c previous config (origin), to be updated with the received pitch.
    /// @param chord an enumerator of notes read for transition to this config.
    /// @param ton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality,
    /// or undef if it was not yet estimated.
    PSC1c(std::shared_ptr<const PSC1c> c,
          const PSChord& chord,
          const Ton& ton,
          const Ton& lton = Ton());

    /// copy constructor.
    PSC1c(const PSC1c& c);
    
    /// destructor.
    virtual ~PSC1c();
    
    /// assignement operator
    PSC1c& operator=(const PSC1c& rhs);

    /// configs have the same list of accidentals
    bool operator==(const PSC1c& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC1c& rhs) const;

    /// @param pc a pitch class in 0..11.
    /// @return if the pitch class pc was already met in chord,
    /// the associated name, or otherwise NoteName::Undef.
    enum NoteName dejavu(unsigned int pc) const;
    
    /// the processing of the chord is terminated.
    bool complete() const;
    
private:
    /// map associating to every pitch class in 0..12
    /// a note name, if the pitch class was encountered while processing the chord
    /// or NoteName::Undef otherwise.
    std::array<enum NoteName, 12> _pcn;

    /// index of the note after the last note of the chord processed.
    bool _complete;
    
private:
    
    /// internal constructor.
    PSC1c(const PSC& c, PSChord& chord);
     
};


} // namespace pse

#endif /* PSConfig1c_hpp */

/// @}
