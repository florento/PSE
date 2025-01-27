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
/// @see PSEnum
/// @see PSRawEnum
class SpellerEnum : public Speller
{
public:
    
    /// speller with raw enumerator (initially empty) of notes to spell.
    /// @param nbtons use default list of tonalities (default: empty).
    /// @param algo name of the algorithm implemented in speller class.
    /// obsolete. not used anymore.
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    /// @see PSTable
    /// @warning the enumerator must be feeded with add()
    SpellerEnum(size_t nbtons,
                const Algo& algo=Algo::Undef, // TBR
                bool dflag=false);

 // SpellEnum(const Algo& algo=Algo::Undef, size_t nbtons=0, bool dflag=false);
    
    /// destructor
    virtual ~SpellerEnum();

public: // note enumerator

    /// number of input notes in the enumerator of notes to spell.
    size_t size() const;

    /// empty the list of notes in the enumerator of this speller.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to first.
    void resetEnum(size_t i0, size_t i1 = PSEnum::ID_INF);
    
    /// add a new input note to the enumerator of notes to spell.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param dur note duration, in fraction of bars.
    void add(int note, int bar, bool simult=false,
             const PSRatio& dur = PSRatio(0));

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// with duration.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param dur_num numerator of note duration, in fraction of bars.
    /// @param dur_den denominator of note duration, in fraction of bars.
    /// @warning for Phython binding
    void add2(int note, int bar, bool simult=false,
                      long dur_num=0, long dur_den=1);

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// without duration.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @warning for Phython binding
    void add0(int note, int bar, bool simult=false);

private:
    
    PSRawEnum& rawenum() const;
    
};


} // namespace pse

#endif /* SpellerEnum_hpp */

/// @}
