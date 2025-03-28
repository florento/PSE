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

#include "pstrace.hpp"
#include "PSRational.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
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
    
public: // constants
    
    /// value of right bound when this enumerator is open.
    static const size_t ID_INF;
    
public: // construction

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

public: // access
    
    /// whether this PS Enumerator is open: the interval of index of
    /// accessible notes is unbounded on right.
    bool open() const;
        
    /// index of the first note to read in enumerator.
    size_t first() const;
    
    /// index of the note after the last note to read in enumerator.
    /// @warning if this PS Enumerator is open, the return value
    /// is undeterminated.
    /// @see open()
    size_t stop() const;
    
    /// number of notes accessible to this enumerator.
    /// @warning if this PS Enumerator is open, the return value is undeterminated.
    /// @see open()
    virtual size_t size() const;

    /// no note is accessible by this PS Enumerator.
    virtual bool empty() const;

    /// the note at the given index is within the interval of notes accessible
    /// to this enumerator.
    /// @param i index of note.
    virtual bool inside(size_t i) const;

    /// the note at the given index is not within the interval of notes
    /// accessible to this eumerator.
    /// @param i index of note.
    bool outside(size_t i) const;

    /// midi key number in 0..128 of the note at the given index.
    /// @param i index of a note. must be inside the interval
    /// of this enumerator.
    virtual unsigned int midipitch(size_t i) const = 0;

    /// number of the measure containing the note at the given index.
    /// @param i index of a note. must be inside the interval
    /// of this enumerator.
    virtual long measure(size_t i) const = 0;

    /// whether the note at the given index is simultaneous with the next note.
    /// @param i index of a note. must be inside the interval
    /// of this enumerator.
    virtual bool simultaneous(size_t i) const = 0;

    /// name for the note at the given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    virtual enum NoteName name(size_t i) const = 0;
    
    /// accidental for the note at the given index, if it has been set,
    /// otherwise Undef.
    /// @param i index of note in the list of input notes.
    virtual enum Accid accidental(size_t i) const = 0;
    
    /// octave number for the note at the given index, if it has been set,
    /// otherwise 10.
    /// @param i index of note in the list of input notes.
    virtual int octave(size_t i) const = 0;

    /// duration, in number of bars, of the note at the given index,
    /// if it has been set, otherwise 0.
    /// @param i index of note in the list of input notes.
    virtual long duration_num(size_t i) const = 0;
    virtual long duration_den(size_t i) const = 0;

    /// print flag for the note at the given index, if it has been set,
    /// otherwise true.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    virtual bool printed(size_t i) const = 0;
    
public: // modification : rename and rewrite passing notes
    
    /// set the bounds of interval of accessible indexes to new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to first.
    virtual void reset(size_t i0, size_t i1 = PSEnum::ID_INF);
    
    /// rename the note at the given index
    /// @param i index of a note in this enumerator.
    /// @param name note name in 'A'..'G'.
    /// @param accid accidental in [-2, 2] where 1 is a half tone
    /// @param oct octave number in Pitch::OCTAVE_MIN and Pitch::OCTAVE_MAX.
    /// @param printed whether the accidental must be printed.
    /// @see pse::Pitch::rename()
    virtual void rename(size_t i,
                        const enum NoteName& name, const enum Accid& accid,
                        int oct, bool printed) = 0;
    
    /// rename the note at the given index, given a name.
    /// The accidental and octave number are deduced from the other parameters
    /// (MIDI key and the name).
    /// @param i index of a note in this enumerator.
    /// @param name note name in 'A'..'G'.
    /// @param printed whether the accidental must be printed.
    virtual void rename(size_t i, const enum NoteName& name,
                        bool printed=true);

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
    /// D. Meredith in the PS13 Pitch-Spelling algorithm, step 2.
    /// rules apply to trigrams of notes.
    /// the lhs of every rule is defined by the distance in 1/2 tons
    /// between the 3 notes:
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
    /// @return how many rewriting wer performed.
    size_t rewritePassing();
    
protected:

    /// index of the first note accessible by this enumerator.
    size_t _first;
    
    /// index of the note after the last note accessible by this enumerator.
    size_t _stop;
    
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
