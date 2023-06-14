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

#include <iostream>
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
#include "AlgoName.hpp"

// TODO
// - const ln & lb

namespace pse {


/// abstract class wrapping main pitch-spelling functionalities.
/// interface between pitch spelling algorithm and structures.
/// It contains:
/// - one algorithm name.
/// - a note enumerator. @see PSRawEnum
/// - one array of tonalities considered for pitch spelling.
/// - one debug flag.
class Speller
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param algo name of the algorithm implemented in speller class.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @see TonIndex for supported values pf nbTons.
    /// @param dflag debug mode.
    /// @see PSTable
    Speller(const Algo& algo=Algo::Undef,
            size_t nbTons=0,
            bool dflag=false);

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

    /// number of input notes in the enumerator of notes to spell.
    size_t size() const;

    /// add a new input note to the enumerator of notes to spell.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param dur note duration, in fraction of bars.
    void add(int note, int bar, bool simult=false,
             const Rational& dur = Rational(0));

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// with duration.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @param dur_num numerator of note duration, in fraction of bars.
    /// @param dur_den denominator of note duration, in fraction of bars.
    /// @warning for Phython binding
    void add_pybindwd(int note, int bar, bool simult=false,
                      long dur_num=0, long dur_den=1);

    /// for pybind: add a new input note to the enumerator of notes to spell
    /// without duration.
    /// @param note MIDI key of the new input note.
    /// @param bar bar number of the new input note.
    /// @param simult whether the new input note is simultaneous with the
    /// next note.
    /// @warning for Phython binding
    void add_pybindwod(int note, int bar, bool simult=false);

    /// number of tonalities considered for pitch spelling.
    /// It is the size of array of tonalities.
    size_t nbTons() const;

    /// tonality (for pitch spelling) of given index.
    /// @param i an index in the array of tonalities.
    /// must be smaller than nbtons().
    /// @see nbTons()
    const Ton& ton(size_t i) const;
    
    /// empty the array of tonalities considered for pitch-spelling vectors.
    /// @see addTon
    void resetTons();
    
    /// add one tonality to the array of tonalities considered for
    /// pitch-spelling vectors.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of the tonality added.
    /// @see Ton
    void addTon(int ks, ModeName mode = ModeName::Major);

    /// add a tonality for pitch spelling.
    void addTon(const Ton& ton);
    
    /// compute the best pitch spelling for the input notes,
    /// using the algorithm named in this class.
    /// @return whether computation was succesfull.
    virtual bool spell() = 0;
    
    /// rewrite the passing notes in enumerator.
    /// @return how many notes have been rewritten.
    /// @see class RewritePassing
    size_t rewritePassing();
    
    /// estimated name for the note of given index, in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the enumerator of input notes.
    enum NoteName name(size_t i) const;

    /// estimated name for the note of given index, in -2..2.
    /// @param i index of note in the enumerator of input notes.
    enum Accid accidental(size_t i) const;

    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the enumerator of input notes.
    int octave(size_t i) const;

    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the enumerator of input notes.
    bool printed(size_t i) const;
    
    /// estimated global tonality.
    /// @warning spell() must have been called.
     virtual const Ton& global() const = 0;

    /// index of the estimated global tonality.
    /// @return the index of the estimated global tonality in the index of tons,
    /// in 0..index.size().
    /// @warning spell() must have been called.
    virtual size_t iglobal() const = 0;
    
    // estimated local tonality at note of given index.
    // @param i index of note in the list of input notes.
    // @warning spell() must have been called.
    // virtual const Ton& local(size_t i) const = 0;

protected: // data
        
    /// name of the algorithm implemented.
    const Algo _algo;

    /// enumerator of the input notes.
    PSRawEnum _enum;

    /// array of tonalities that shall be considered for pitch spelling.
    TonIndex _index;
    
    /// debug mode activated.
    bool _debug;

};

} // namespace pse

#endif /* Speller_hpp */

/// @}
