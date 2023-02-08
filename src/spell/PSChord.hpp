//
//  PSChord.hpp
//  pse
//
//  Created by Florent Jacquemard on 05/02/2023.
//
/// @addtogroup pitch
/// @{


#ifndef PSChord_hpp
#define PSChord_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <array>
#include <vector>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "PSEnum.hpp"



namespace pse {

/// sequence of simultaneous notes called a "chord".
/// It can point notes in different voices or in one or several simultaneous
/// chords.
class PSChord : public PSEnum
{
public:

    /// main constructor.
    /// @param e underlying note enumerator.
    /// @param i0 index of the first note accessible by this enumerator.
    PSChord(const PSEnum& e, size_t i0);

    /// copy constructor.
    PSChord(const PSChord& rhs);

    /// destructor
    virtual ~PSChord();
    
    /// clone this enumerator into a enumerator of the same type.
    /// @warning should not be called for this enumerator.
    virtual std::unique_ptr<PSEnum> clone() const;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    /// @warning should not be called for this enumerator.
    virtual std::unique_ptr<PSEnum> clone(size_t i0, size_t i1) const;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    /// @warning should not be called for this enumerator.
    virtual std::unique_ptr<PSEnum> clone(size_t i0) const;
    
    /// number of notes in this chord.
    /// @see PSEnum::length()
    inline size_t size() const { return length(); }
    
    /// number of occurrences of the given pitch class in this chord.
    /// @param c pitch class number, between 0 and 11.
    /// @return number of occurrences of the pitch class c in this chord.
    size_t occurences(unsigned int c) const;

    /// index of one occurrence of the given pitch class in this chord.
    /// @param c pitch class number, between 0 and 11.
    /// @param i index in 0..occurences(c).
    /// @return number of occurrences of the pitch class c in this chord.
    size_t occurence(unsigned int c, size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual unsigned int midipitch(size_t i) const;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual long measure(size_t i) const;

    /// whether the note of given index is simultaneous with the next note.
    /// @param i index of a note. must be inside the interval of this enumerator.
    /// @warning always true for the notes in this enumerator,
    /// except the last one.
    virtual bool simultaneous(size_t i) const;
    
    /// rename the note of given index
    /// @param name note name in 'A'..'G'.
    /// @param accid accidental in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @warning should not be called for this enumerator.
    virtual void rename(size_t i, const NoteName& name, const Accid& accid,
                        int oct, bool altprint);
    
private:

    /// seq of occurrences of each pitch class
    /// in the sequence of simultaneous notes.
    std::array<std::vector<size_t>, 12> _occurences;
    //std::array<std::vector<bool>, 12> _occurences;

    // number of occurrence of each pitch class in thos sequence of simultaneous
    // notes. stored separatly from occurences because std::vector<bool> does
    // not provide count similar to std::bitset
    //unsigned int _nboccurences[12];
    
    /// underlying enumerator
    const PSEnum& _enum;
    
    /// @return the index of the first note in enumerator e
    /// not simultaneous (and after) the note of index i0,
    /// or ID_INF if there is none.
    static size_t firstNonSimult(const PSEnum& e, size_t i0);

    /// initialize the table of occurrences
    void init();
    
}; // class PSChord


} // namespace pse

#endif /* PSChord_hpp */

/// @}
