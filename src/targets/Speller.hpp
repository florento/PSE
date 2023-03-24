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

/// wrapper in top of PSRawEnum
/// interface to pybind
class Speller
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see PSTable
    Speller(size_t nbTons=0, bool dflag=true);

    // Speller for the given input notes.
    // @param ln Python list of Music 21 Note objects (references).
    // @param lb Python list of bar numbers (ints).
    // @warning both list must be of same length.
    // Speller(py::list& ln, const py::list& lb);

    // Speller for a list of notes with midi pitch and bar number.
    // Speller(const std::vector<int>& notes, const std::vector<int>& barnum);

    /// destructor
    virtual ~Speller();

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
    
    /// number of tonalities considered for pitch spelling.
    size_t nbTons() const;

    /// tonality (for pitch spelling) of given index.
    /// @param i an index in array of tonalities. must be smaller than nbtons().
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
        
    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    bool spell();
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    void setGlobal(size_t i);

    /// number of candidates (ties) for the estimatation of the global tonality.
    size_t globalCands() const;
    
    /// candidate global tonality for this table.
    /// @param i candidate number, must be in 0..globalCands().
    const Ton& globalCand(size_t i) const;

    /// index of a candidate global tonality for this table, in 0..index.size().
    size_t iglobalCand(size_t i) const;
 
    /// estimated local tonality for one candidate global tonality and one bar.
    const Ton& local(size_t i, size_t j) const; // { return _table.local(i, j); }
    
    /// estimated global tonality
    /// @warning respell() must have been called.
    const Ton& global() const;
    
    /// index of the estimated global tonality.
    size_t iglobal() const;
    
    /// @return distance in the array of fifths between
    /// from estimated global tonality and
    /// a signature with no accidentals.
    inline int fifths() const { return global().fifths(); }

    /// estimated name for the note of given index in the best path,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the list of input notes.
    inline pse::NoteName name(size_t i) const { return _enum.name(i); }

    /// estimated name for the note of given index in the best path, in -2..2.
    /// @param i index of note in the list of input notes.
    inline pse::Accid accidental(size_t i) const {  return _enum.accidental(i); }

    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the list of input notes.
    inline int octave(size_t i) const {  return _enum.octave(i); }

    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed or not.
    /// @param i index of note in the list of input notes.
    inline bool printed(size_t i) const {  return _enum.printed(i); }

private: // data
        
    /// enumerator of the input notes
    pse::PSRawEnum _enum;

    /// Pitch Spelling table
    pse::PST _table;
    
    // evaluated global tonality
    //pse::Ton _global;
    
    /// debug mode activated
    bool _debug;
    
private:
        
    // default array of tonalities considered for Pitch Spelling.
    // @todo mv to TonIndex
    // static const std::vector<const Ton> TONS;

    // smaller table of all tonalities considered for PS.
    // @todo mv to TonIndex
    // static const std::vector<const Ton> TONS26;

    // table of state vectors associated to tonalities in TON.
    // static std::array<const PSState, NBTONS> ASTATES;

    // table of joker state vectors associated to tonalities in TON.
    // static std::array<const PSState, NBTONS> AJOKER;

};

} // namespace pse

#endif /* Speller_hpp */

/// @}
