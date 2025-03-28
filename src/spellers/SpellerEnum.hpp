//
//  SpellerEnum.hpp
//  pse
//
//  Created by Florent Jacquemard on 23/01/2025.
//
/// @addtogroup pitch
/// @{

#ifndef SpellerEnum_hpp
#define SpellerEnum_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <time.h>

#include "Speller.hpp"

namespace pse {

/// Special case of speller class
/// with an internal raw note enumerator feed manually.
/// with one or two internal raw enumerators of notes to spell,
/// initially empty and feed manually.
/// The second, auxiliary, enumerator is optional. Its purpose it to contain
/// alternative input notes for the construction of the first table only.
/// @see PSEnum
/// @see PSRawEnum
class SpellerEnum : public Speller
{
public:
    
    /// speller with one or two raw enumerators (initially empty)
    /// of notes to spell.
    /// @param tons use default list of tonalities (default: empty).
    /// @param aux_enum whether we consider an optional auxiliary enumerator
    /// of alternative input notes, for the construction of the first table.
    /// @param algo name of the algorithm implemented in speller class.
    /// obsolete. not used anymore.
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    /// @see PSTable
    /// @warning the enumerator must be feeded with add()
    SpellerEnum(size_t tons, bool aux_enum=false,
                const Algo& algo=Algo::Undef, // TBR
                bool dflag=false);

    // SpellEnum(const Algo& algo=Algo::Undef, size_t nbtons=0, bool dflag=false);
    
    /// destructor
    virtual ~SpellerEnum();

public: // note enumerator

    /// number of input notes in the enumerator of notes to spell.
    /// @param aux whetherwe consider the auxiliary enumerator.
    size_t size(bool aux=false) const override;

    /// empty the list of notes in the enumerator of this speller.
    /// if there is an auxiliary enumerator, it is also reset.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to first.
    void resetEnum(size_t i0, size_t i1 = PSEnum::ID_INF);
    
    /// add the given speller an optional auxiliary enumerator.
    /// @return whether there was no existing auxiliary enumerator.
    /// otherwise, nothing is done.
    bool addAuxEnumerator();
    
    // add a new input note to the enumerator of notes to spell.
    // @param midi MIDI key of the new input note.
    // @param bar bar number of the new input note.
    // @param simult whether the new input note is simultaneous with the
    // next note.
    // @param dur note duration, in fraction of bars.
    // void add(int midi, int bar, bool simult=false,
    //          const PSRatio& dur = PSRatio(0));

    /// add a new input note to the enumerator of notes to spell.
    /// @param midi MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param aux whether the note shall be added to the auxiliary enumerator.
    /// @param name note name in 'A'..'G'. NoteName::Undef id unknown.
    /// @param accid accidental. Accid::Undef is unknown.
    /// @param octave number in Pitch::OCTAVE_MIN, Pitch::OCTAVE_MAX
    /// of Pitch::UNDEF_OCTAVE if unknown.
    /// @param printed whether the accidental must be printed.
    /// @param dur note duration, in fraction of bars.
    /// @warning if one of name, accid, oct is UNDEF, all three must be UNDEF.
    void add(int midi, int bar, bool simult=false,
             bool aux=false,
             const PSRatio& dur = PSRatio(0),
             const enum NoteName& name=NoteName::Undef,
             const enum Accid& accid=Accid::Undef,
             int octave=Pitch::UNDEF_OCTAVE,
             bool printed=false);

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// without duration.
    /// @param midi MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simultaneous whether the new input note is simultaneous with the
    /// next note.
    /// @param aux whether the note shall be added to the auxiliary enumerator.
    /// @warning for Phython binding
    void add0(int midi, int bar, bool simultaneous=false, bool aux=false);

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// with duration.
    /// @param midi MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simultaneous whether the new input note is simultaneous with the
    /// next note.
    /// @param dur_num numerator of note duration, in fraction of bars.
    /// @param dur_den denominator of note duration, in fraction of bars.
    /// @param aux whether the note shall be added to the auxiliary enumerator.
    /// @warning for Phython binding
    void add2(int midi, int bar, bool simultaneous=false,
              long dur_num=0, long dur_den=1, bool aux=false);

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// without duration.
    /// @param midi MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simultaneous whether the new input note is simultaneous
    /// with the next note.
    /// @param name note name in 'A'..'G', if it is known.
    /// @param accid accidental, if it is known.
    /// @param octave octave number in Pitch::OCTAVE_MIN and Pitch::OCTAVE_MAX,
    /// or Pitch::UNDEF_OCTAVE if it is unknown.
    /// @param printed whether the accidental must be printed, if it is known.
    /// @param aux whether the note shall be added to the auxiliary enumerator.
    /// @warning for Phython binding
    void add4(int midi, int bar, bool simultaneous=false,
              const enum NoteName& name=NoteName::Undef,
              const enum Accid& accid=Accid::Undef,
              int octave=Pitch::UNDEF_OCTAVE,
              bool printed=false, bool aux=false);
    
    /// for pybind: add a new input note to the enumerator of notes to spell
    /// with duration.
    /// @param midi MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simultaneous whether the new input note is simultaneous
    /// with the next note.
    /// @param dur_num numerator of note duration, in fraction of bars.
    /// @param dur_den denominator of note duration, in fraction of bars.
    /// @param name note name in 'A'..'G', if it is known.
    /// @param accid accidental, if it is known.
    /// @param octave octave number in Pitch::OCTAVE_MIN and Pitch::OCTAVE_MAX,
    /// or Pitch::UNDEF_OCTAVE if it is unknown.
    /// @param printed whether the accidental must be printed, if it is known.
    /// @param aux whether the note shall be added to the auxiliary enumerator.
    /// @warning for Phython binding
    void add6(int midi, int bar, bool simultaneous=false,
              long dur_num=0, long dur_den=1,
              const enum NoteName& name=NoteName::Undef,
              const enum Accid& accid=Accid::Undef,
              int octave=Pitch::UNDEF_OCTAVE,
              bool printed=false, bool aux=false);
    
private:

    /// access the internal raw note enumerator.
    /// @param aux whether we want the auxiliary enumerator.
    /// in this case it must be set.
    PSRawEnum& rawenum(bool aux=false) const;
    
};


} // namespace pse

#endif /* SpellerEnum_hpp */

/// @}
