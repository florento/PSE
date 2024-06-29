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
#include "PSRational.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "MidiNum.hpp"
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

    /// Pitch Spelling enumerator with list of notes, initialy empty.
    /// @param i0 index of the first note accessible by this enumerator.
    /// @param i1 index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to i0.
    PSRawEnum(size_t i0, size_t i1 = PSEnum::ID_INF);

    // Pitch Spelling enumerator from list of notes with midi pitch 
    // and bar number.
    // PSRawEnum(const std::vector<int>& notes, const std::vector<int>& barnum);

    // @param s list of Music 21 Note objects (references).
    //PSRawEnum(py::list& s);
    
    /// copy constructor.
    /// deep copies of the containers.
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
    std::unique_ptr<PSEnum> clone() const override;
    
    /// clone this enumerator and update the bounds with new values.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. it must be larger than or equal to i0.
    /// The enumerated sequence of notes is empty if i0 == i1.
    std::unique_ptr<PSEnum> clone(size_t i0, size_t i1) const override;

    /// clone this enumerator and update the left bound of interval.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// The enumeration starts at i0 and stops when there are no more notes
    /// to read in e.
    std::unique_ptr<PSEnum> clone(size_t i0) const override;

    /// number of notes accessible to this enumerator.
    /// if the enumerator is open, it the real number of notes minus first(),
    /// otherwise, it is stop() - first().
    /// @warning it may not be smaller than the number of notes added
    /// if first() > 0.
    size_t size() const override;
    
    /// empty the list of notes in this enumerator.
    void reset(size_t i0, size_t i1 = PSEnum::ID_INF) override;
    
    /// add a new input note to the list of enumerated notes.
    /// @param note MIDI key of the new input note. must be in 0..128.
    /// @param bar bar number of the new input note. must be positive.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param dur note duration, in fraction of bars.
    void add(int note, int bar, bool simult=false,
             const Rational& dur = Rational(0));
    
    /// add a new input note to the list of enumerated notes.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param dur_num numerator of note duration, in fraction of bars.
    /// @param dur_den denominator of note duration, in fraction of bars.
    /// @warning for Phython binding
    void addlong(int note, int bar, bool simult=false,
                 long dur_num=0, long dur_den=1);
    
    // the given note index is within the interval of notes accessible
    // to this enumerator.
    // @param i index of note.
    // virtual bool inside(size_t i) const;

    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    unsigned int midipitch(size_t i) const override;

    /// number of measure the note of given index belongs to.
    /// midi key number in 0..128 of the note of the given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    long measure(size_t i) const override;

    /// whether the note of given index is simultaneous with the next note.
    /// @param i index of a note. must be inside the interval of this enumerator.
    bool simultaneous(size_t i) const override;

    /// record new NoteName, Accid, Octave, print flag for the note of given index.
    /// @param i index of a note. must be inside the interval of this enumerator.
    /// @param n note name in 'A'..'G'.
    /// @param a accidental in [-2, 2] where 1 is a half tone
    /// @param o octave number in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @see Pitch::rename()
    /// @warning the triplet n, a, o must correspond to the midi value
    /// of this pitch.
    /// @warning the notes cannot be renamed in place because the Python
    /// lists in argument contain const objects.
    void rename(size_t i,
                const enum NoteName& n, const enum Accid& a, int o,
                bool altprint) override;
    
    /// record new note name, accidental, octave, print flag for the note
    /// of given index. The accidental and octave are deduced from
    /// @param i index of a note. must be inside the interval of this enumerator.
    /// @param n note name in 'A'..'G'.
    /// @see Pitch::rename()
    /// @warning the name n must be a possible name for the current midi value
    /// of this pitch.
    /// @warning the alt-print flag is set arbitrarily to true.
    /// @warning the notes cannot be renamed in place because the Python
    /// lists in argument contain const objects.
    void rename(size_t i, const enum NoteName& n, bool altprint=true) override;
       
    /// estimated name for the note of given index in the best path,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the list of input notes.
    enum NoteName name(size_t i) const override;
    
    /// estimated alteration for the note of given index in the best path,
    /// in -2..2.
    /// @param i index of note in the list of input notes.
    enum Accid accidental(size_t i) const override;
    
    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the list of input notes.
    int octave(size_t i) const override;

    /// duration, in number of bars, of the note of given index,
    /// if it has been set, otherwise 0.
    /// @param i index of note in the list of input notes.
    virtual Rational duration(size_t i) const override;
    
    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    bool printed(size_t i) const override;
    
    // count the number of occurrence of a pitch class in a window around
    // a given note.
    // @param c a pitch class in 0..11.
    // @param i index of a note in this enumerator.
    // @param pre number of notes to consider before i.
    // @param post number of notes to consider after i.
    // @return the number of occurrence of c in the interval
    // from i - pre (included) to i + post (excluded).
    // virtual size_t count(int c, size_t i, size_t pre, size_t post);
    
private: // data (shared by all copies of this enumerator)
       
    /// list of MIDI pitch of all notes in input.
    /// entered with method add().
    std::shared_ptr<std::vector<int>> _notes;

    /// list of bar number to which belongs each input note.
    /// entered with method add().
    std::shared_ptr<std::vector<int>> _barnum;
    
    /// list of simultaneity with next note, for each input note
    /// entered with method add().
    std::shared_ptr<std::vector<bool>> _simult;
    
    /// list of durations of input notes.
    /// entered with method add().
    std::shared_ptr<std::vector<Rational>> _durations;
    
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
