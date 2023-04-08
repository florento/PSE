//
//  Speller.hpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//
/// @addtogroup pitch
/// @{


#ifndef Speller_hpp
#define Speller_hpp

#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
#include "PSRawEnum.hpp"
#include "PSTable.hpp"

// TODO
// - const ln & lb

namespace pse {


/// names of pitch spelling algos implemented
enum class Algo
{
    Undef,
    PSE,
    PS13,
    PS14,
    RewritePassing
};

/// string of algo name.
std::string tostring(const Algo& m);

std::ostream& operator<<(std::ostream& o, const Algo& m);


/// abstract class wrapping main pitch-spelling functionalities 
class Speller
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param algo name of the algorithm implemented in speller class.
    /// @param dflag debug mode.
    /// @see PSTable
    Speller(const Algo& algo=Algo::Undef, bool dflag=false);

    /// copy constructor.
    /// makes a deep copy of the note enumerator.
    Speller(const Speller& rhs);

    // Speller for the given input notes.
    // @param ln Python list of Music 21 Note objects (references).
    // @param lb Python list of bar numbers (ints).
    // @warning both list must be of same length.
    // Speller(py::list& ln, const py::list& lb);

    // Speller for a list of notes with midi pitch and bar number.
    // Speller(const std::vector<int>& notes, const std::vector<int>& barnum);

    /// destructor
    virtual ~Speller();

    /// name of algorithm implemented.
    /// @return Algo::Undef by default.
    const Algo& algo() const { return _algo; }
    
    /// set debug mode (log messages up to debug)
    void debug(bool flag);

    /// number of input notes to the list of notes to spell.
    size_t size() const;

    /// add a new input note to the list of notes to spell.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    void add(int note, int bar, bool simult=false);
           
    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    virtual bool spell() = 0;
    
    /// rewrite the passing notes using the 6 rewrite rules proposed by
    /// D. Meredith in the PS13 Pitch-Spelling algorithm, step 2.
    /// @return whether at least one rewriting was done.
    /// @see PSEnum::rewritePassing()
    bool rewritePassing();
    
    /// estimated name for the note of given index, in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the list of input notes.
    enum NoteName name(size_t i) const;

    /// estimated name for the note of given index, in -2..2.
    /// @param i index of note in the list of input notes.
    enum Accid accidental(size_t i) const;

    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the list of input notes.
    int octave(size_t i) const;

    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    bool printed(size_t i) const;
    
    /// estimated global tonality.
    /// @warning spell() must have been called.
     virtual const Ton& global() const = 0;

    // estimated local tonality at note of given index.
    // @param i index of note in the list of input notes.
    // @warning spell() must have been called.
    // virtual const Ton& local(size_t i) const = 0;

protected: // data
        
    /// name of the algorithm implemented.
    const Algo _algo;
    
    /// enumerator of the input notes
    PSRawEnum _enum;

    /// debug mode activated
    bool _debug;

};

} // namespace pse

#endif /* Speller_hpp */

/// @}
