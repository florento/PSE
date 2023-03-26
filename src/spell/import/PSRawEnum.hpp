//
//  PSRawEnum.hpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//
/// @addtogroup pitch
/// @{


#ifndef PSRawEnum_hpp
#define PSRawEnum_hpp

#include <iostream>
#include <iostream>
//#include <iomanip>
#include <assert.h>
#include <memory>
#include <vector>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "PSEnum.hpp"
//#include "Stream.hpp"

namespace pse {


// 2 possib. for renaming
// - direct call to M21 methods on Note
// - mod. an aux. PSPath stored here
//   pybind accessors to this path (for name and accid)


/// Implementation of a basic PSEnum, with
/// - input buffer of notes (MIDI pitches and associated bar number),
///   filled manually, with the add() function
/// - output buffers of note names, accidentals, octaves and print flags,
///   filled by callback of the rename() function.
struct PSRawEnum : public PSEnum
{
public:

    /// undefined octave number.
    static int OCTAVE_UNDEF;

    /// Pitch Spelling enumerator with initialy empty list of notes.
    /// @param i0 index of the first note accessible by this enumerator.
    /// @param i1 index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to i0.
    PSRawEnum(size_t i0, size_t i1);

    // Pitch Spelling enumerator from list of notes with midi pitch and bar number.
    // PSRawEnum(const std::vector<int>& notes, const std::vector<int>& barnum);

    // @param s list of Music 21 Note objects (references).
    //PSRawEnum(py::list& s);
    
    /// copy constructor
    PSRawEnum(const PSRawEnum& e);

    /// copy and update left bound of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    PSRawEnum(const PSRawEnum& e, size_t i0);
    
    /// copy and update left and right bounds of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    PSRawEnum(const PSRawEnum& e, size_t i0, size_t i1);

    /// destructor.
    virtual ~PSRawEnum();
    
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

    /// number of input notes in this enumerator.
    size_t size() const;
    
    /// add a new input note to the list of enumerated notes.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    void add(int note, int bar, bool simult=false);
    
    // the given note index is within the interval of notes accessible
    // to this enumerator.
    // @param i index of note.
    // virtual bool inside(size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual unsigned int midipitch(size_t i) const;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual long measure(size_t i) const;

    /// whether the note of given index is simultaneous with the next note.
    /// @param i index of a note. must be inside the interval of this enumerator.
    virtual bool simultaneous(size_t i) const;

    /// record new NoteName, Accid, Octave, print_flag for the note of given index.
    /// @param name note name in 'A'..'G'.
    /// @param accid accidetal in [-2, 2] where 1 is a half tone
    /// @param oct octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @see Pitch::rename()
    /// @warning the notes cannot be renamed in place because the Python
    /// lists in argument contain const objects.
    virtual void rename(size_t i,
                        const enum NoteName& name, const enum Accid& accid,
                        int oct, bool altprint);
    
    /// estimated name for the note of given index in the best path,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the list of input notes.
    enum NoteName name(size_t i) const;
    
    /// estimated alteration for the note of given index in the best path,
    /// in -2..2.
    /// @param i index of note in the list of input notes.
    enum Accid accidental(size_t i) const;
    
    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the list of input notes.
    int octave(size_t i) const;

    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    bool printed(size_t i) const;
    
private: // data (shared by all copies of this enumerator)
       
    /// list of MIDI pitch of all notes in input.
    /// extracted from the list of Music 21 notes.
    std::shared_ptr<std::vector<int>> _notes;

    /// list of bar number for each note
    std::shared_ptr<std::vector<int>> _barnum;
    
    /// list of simultaneity with next, for each ntoe
    std::shared_ptr<std::vector<bool>> _simult;
    
    /// list of the estimated best note name (in 0..6) for each input note.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::shared_ptr<std::vector<enum NoteName>> _names;

    /// list of the estimated best accident (in -2..2) for each input note.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::shared_ptr<std::vector<enum Accid>> _accids;

    /// list of the estimated best octave (in -2..9) for each input note.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::shared_ptr<std::vector<int>> _octs;

    /// list of the estimated best print flag for each input note.
    /// This flags says wether the accidental of the note must be printed or not.
    /// copy of the values of the PSPaths (best paths) in the columns of table,
    /// temporaly stored by rename, because the input notes are const protected.
    std::shared_ptr<std::vector<bool>> _prints;

    bool sanity_check() const;
       
};

} // namespace pse

#endif /* PSRawEnum_hpp */

/// @}
