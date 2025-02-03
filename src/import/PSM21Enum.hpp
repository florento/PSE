//
//  PSM21Enum.hpp
//  qparse
//
//  Created by Florent Jacquemard on 12/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.

/// @addtogroup pitch
/// @{


#ifndef PSM21Enum_hpp
#define PSM21Enum_hpp

#include <iostream>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <memory>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "PSEnum.hpp"
//#include "Stream.hpp"

namespace py = pybind11;

namespace pse {


// 2 possib. for renaming
// - direct call to M21 methods on Note
// - mod. an aux. PSPath stored here
//   pybind accessors to this path (for name and accid)


/// Implementation of PSEnum on the top of a Music21 Stream iterator.
/// @see https://web.mit.edu/music21/doc/moduleReference/moduleStreamIterator.html
/// @see https://web.mit.edu/music21/doc/moduleReference/moduleStreamFilters.html
struct PSM21Enum : public pse::PSEnum
{
public:
    
    /// Pitch Spelling enumerator from a Music 21 Stream.
    /// @param ln Python list of Music 21 Note objects (references).
    /// @param lb Python list of bar numbers (ints).
    /// @warning both list must be of same length.
    PSM21Enum(py::list& ln, const py::list& lb);
    
    // Pitch Spelling enumerator from list of notes with midi pitch and bar number.
    // PSM21Enum(const std::vector<int>& notes, const std::vector<int>& barnum);

    // @param s list of Music 21 Note objects (references).
    //PSM21Enum(py::list& s);
    
    /// copy constructor
    PSM21Enum(const PSM21Enum& e);

    /// copy and update left bound of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    PSM21Enum(const PSM21Enum& e, size_t i0);
    
    /// copy and update left and right bounds of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    PSM21Enum(const PSM21Enum& e, size_t i0, size_t i1);

    /// destructor.
    ~PSM21Enum();
    
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

    // the given note index is within the interval of notes accessible
    // to this enumerator.
    // @param i index of note.
    // virtual bool inside(size_t i) const;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual long measure(size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual unsigned int midipitch(size_t i) const;

    /// record new NoteName, Accid, Octave, print_flag for the note of given index.
    /// @param name note name in 'A'..'G'.
    /// @param accid accidetal in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @see pse::Pitch::rename()
    /// @warning the notes cannot be renamed in place because the Python
    /// lists in argument contain const objects.
    virtual void rename(size_t i, const enum pse::NoteName& name,
                        const enum pse::Accid& accid, int oct, bool altprint);
    
   
private:
    
    /// imported list of Music 21 notes (references).
    /// renamed.
    py::list& _import;
    
    /// list of MIDI pitch of all notes in input.
    /// extracted from the list of Music 21 notes.
    std::vector<int> _notes;

    /// extracted list of bar numbers
    std::vector<int> _barnum;
    
    /// list of the estimated best note name (in 0..6) for each input note.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::vector<enum pse::NoteName> _names;

    /// tmp list of the estimated best accident (in -2..2) for each input note.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::vector<enum pse::Accid> _accids;

    /// tmp list of the estimated best octave (in -2..9) for each input note.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::vector<int> _octave;
       
};



} // namespace scoremodel

#endif /* PSStreamEnum_hpp */

/// @}

