//
//  PSEnum.hpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSEnum_hpp
#define PSEnum_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "MidiNum.hpp"

namespace pse {


/// @todo
/// - PSEnum abstract
/// - PSNoteEnum descendant, avec un NoteEnum
/// - PSM21Enum descendant avec pybind
///
/// descendants de PSEnum ou has_a PSEnum (?)
/// constructeur avec arg PSEnum
/// - PSB
/// - PSV
/// - PST
/// - PSP


/// Abstract class with functionalities needed by Pitch Spelling algos
/// to access (read and modify/rename) a sequence of notes.
/// The notes are accessed by an index in an interval of positive integers.
/// The interval of accessible indexes is left bounded and can be
/// right bounded or right unbounded (open PS Enumerator).
class PSEnum
{
public:
    
    /// Pitch Spelling enumerator with starting note and (optional) ending note.
    /// @param i0 index of the first note accessible by this enumerator.
    /// @param i1 index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to i0.
    PSEnum(size_t i0 = 0, size_t i1 = PSEnum::ID_INF);

    /// copy constructor.
    PSEnum(const PSEnum& e);

    /// copy and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to i0.
    PSEnum(const PSEnum& e, size_t i0, size_t i1 = PSEnum::ID_INF);

    /// destructor
    virtual ~PSEnum();
    
    /// clone this enumerator into a enumerator of the same type.
    virtual std::unique_ptr<PSEnum> clone() const = 0;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    virtual std::unique_ptr<PSEnum> clone(size_t i0, size_t i1) const = 0;

    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    virtual std::unique_ptr<PSEnum> clone(size_t i0) const = 0;

    /// whther this PS Enumerator is open: the interval of index of
    /// accessible notes is unbounded on right.
    bool open() const;
        
    /// index of the first note to read in enumerator.
    size_t first() const;
    
    /// index of the note after the last note to read in enumerator.
    /// @warning if this PS Enumerator is open, the return value is undeterminated.
    /// @see open()
    size_t stop() const;
    
    /// number of notes accessible to this enumerator.
    /// @warning if this PS Enumerator is open, the return value is undeterminated.
    /// @see open()
    virtual size_t size() const;

    /// no note is accessible by this PS Enumerator.
    bool empty() const;

    /// the given note index is within the interval of notes accessible
    /// to this enumerator.
    /// @param i index of note.
    virtual bool inside(size_t i) const;

    /// the given note index is not within the interval of notes accessible
    /// to this eumerator.
    /// @param i index of note.
    bool outside(size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual unsigned int midipitch(size_t i) const = 0;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual long measure(size_t i) const = 0;

    /// whether the note of given index is simultaneous with the next note.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual bool simultaneous(size_t i) const = 0;

    /// name for the note of given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    virtual enum NoteName name(size_t i) const = 0;
    
    /// accidental for the note of given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    virtual enum Accid accidental(size_t i) const = 0;
    
    /// octave number for the note of given index, if it has been set,
    /// otherwise 10.
    /// @param i index of note in the list of input notes.
    virtual int octave(size_t i) const = 0;

    /// print flag for the note of given index, if it has been set,
    /// otherwise true.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    virtual bool printed(size_t i) const = 0;
    
    /// rename the note of given index
    /// @param i index of a note in this enumerator.
    /// @param name note name in 'A'..'G'.
    /// @param accid accidental in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @see Pitch::rename()
    virtual void rename(size_t i,
                        const enum NoteName& name, const enum Accid& accid,
                        int oct, bool altprint) = 0;
    
    /// rename the note of given index, given a name.
    /// The accidental and octave number are deduced from the other parameters
    /// (MIDI key and the name).
    /// @param i index of a note in this enumerator.
    /// @param name note name in 'A'..'G'.
    /// @param altprint whether the accidental must be printed.
    virtual void rename(size_t i, const enum NoteName& name,
                        bool altprint=true);

    /// count the number of occurrence of a pitch class in a window around
    /// a given note.
    /// @param c a pitch class in 0..11.
    /// @param i index of a note in this enumerator.
    /// @param pre number of notes to consider before i.
    /// @param post number of notes to consider after i.
    /// @return the number of occurrence of c in the interval
    /// from i - pre (included) to i + post (excluded).
    size_t count(int c, size_t i, size_t pre, size_t post) const;
    
    /// correct the passing notes using 6 rewrite rules proposed by
    /// D. Meredith in his PS13 Pitch-Spelling algorithm.
    /// rules apply to trigrams of notes.
    /// the lhs of every rule is defined by the distance in 1/2 tons between
    /// the 3 notes:
    /// - -1 +1  (broderie down)
    ///   ex. `C` `Cb` `C` $\to$ `C` `B` `C`
    /// - +1 -1  (broderie up)
    ///   ex. `C` `C#` `C` $\to$ `C` `Db` `C`
    /// - -1 -2  (descending 1)
    ///   ex. `C` `Cb` `A` $\to$ `C` `B` `A`
    /// - +1 +2  (ascending 1)
    ///   ex. `A` `A#` `C` $\to$ `A` `Bb` `C`
    /// - -2 -1  (descending 2)
    ///   ex. `C` `A#` `A` $\to$ `C` `Bb` `A`
    /// - +2 +1  (ascending 2)
    ///   ex. `A` `Cb` `C` $\to$ `A` `B` `C`
    /// @return whether at least one rewriting was done.
    bool rewritePassing();
    
protected:

    /// index of the first note accessible by this enumerator.
    size_t _first;
    
    /// index of the note after the last note accessible by this enumerator.
    size_t _stop;

    /// value of right bound when this enumerator is open.
    static const size_t ID_INF;
    
    /// set the bounds of interval of accessible indexes to new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to first.
    void reset(size_t i0, size_t i1 = PSEnum::ID_INF);
    
private:
    /// rewrite a trigram of notes starting at the given position.
    /// @param i index of a note in this enumerator.
    /// @return whether the trigram at i was rewritten.
    /// @see rewritePassing()
    bool rewritePassing(size_t i);

    
}; // class PSEnum

} // namespace pse

#endif /* PSEnum_hpp */

/// @}
