//
//  PSNoteEnum.hpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSNoteEnum_hpp
#define PSNoteEnum_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "PSEnum.hpp"
#include "NoteEnum.hpp"


namespace scoremodel {


/// Implementation of PSEnum on the top of a NoteEnum.
/// Pitch Spelling structure defined from a sequence of notes,
/// given by a part enumerator (see NoteEnum)
/// and the index (wrt the enumerator) of first and last note in sequence.
/// @todo obsolete
struct PSNoteEnum : public pse::PSEnum
{
public:
    
    /// Pitch Spelling enumerator with starting note.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 index of the note after the last note accessible by this
    /// enumerator. must be superior of equal to i0.
    /// The sequence of notes is empty if if i0 == i1.
    /// @warning the values of parameters cannot be changed
    /// once the object created.
    PSNoteEnum(NoteEnum& e, size_t i0, size_t i1);

    /// Pitch Spelling enumerator with starting note and
    /// unbounded on the right.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    PSNoteEnum(NoteEnum& e, size_t i0);

    /// copy constructor
    PSNoteEnum(const PSNoteEnum& e);

    /// copy and update left bound of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    PSNoteEnum(const PSNoteEnum& e, size_t i0);
    
    /// copy and update left and right bounds of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    PSNoteEnum(const PSNoteEnum& e, size_t i0, size_t i1);

    /// destructor.
    ~PSNoteEnum();

    /// clone this enumerator into a enumerator of the same type.
    virtual std::unique_ptr<PSEnum> clone() const;
    
    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    virtual std::unique_ptr<PSEnum> clone(size_t i0, size_t i1) const;

    /// clone this enumerator and update the left bound of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    virtual std::unique_ptr<PSEnum> clone(size_t i0) const;

    /// note enumerator.
    inline NoteEnum& enumerator() const { return _enum; }

    // index of the first note to read in enumerator.
    // virtual size_t first() const;
    
    // index of the note after the last note to read in enumerator.
    // @warning an ending note (not NoteEnum::ID_UNDEF)
    // must have been give at initialization of this PSEnum.
    // virtual size_t stop() const;
    
    // number of note read to create the PS structure.
    // @warning an ending note (not NoteEnum::ID_UNDEF)
    // must have been give at initialization of this PSEnum.
    // size_t length() const; // const { return _last - _first; }

    // no note to read.
    // bool empty() const; // { return _first == _last; }

    // the note index is within the range of the note sequence considered.
    // @param i number of note.
    // bool inside(size_t i) const;
    
    /// the given note index is within the interval of notes accessible
    /// to this enumerator.
    /// @param i index of note.
    virtual bool inside(size_t i) const;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual long measure(size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual unsigned int midipitch(size_t i) const;

    /// rename note  of given index NoteName Accid Octave print_flag
    /// @param name note name in 'A'..'G'.
    /// @param accid accidetal in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @see pse::Pitch::rename()
    virtual void rename(size_t i, const pse::NoteName& name,
                        const pse::Accid& accid, int oct, bool altprint);
    
    // note return by the enumerator, by index.
    // @param i number of note, between 0 and length().
    // @todo TBR
    // Note* note(size_t i) const;
        
protected:
    
    /// note enumerator where the input is read.
    NoteEnum& _enum;
    
    // index of the first note to read in enumerator.
    // size_t _first;
    
    // index of the last note to read in enumerator.
    // size_t _stop;

};



} // namespace scoremodel

#endif /* PSNoteEnum_hpp */

/// @}
