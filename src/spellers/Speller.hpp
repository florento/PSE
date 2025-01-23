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
#include <time.h>

#include "Spelli.hpp"


// TODO
// - const ln & lb

namespace pse {

/// class wrapping main pitch-spelling functionalities
/// i.e. interface between pitch spelling algorithm and structures.
/// It is associated with a fixed note enumerator,
/// giving acces to the notes to be spelled.
/// The enumerator can be external, given at initialization,
/// or an internal raw enumerator (PSRawEnum) feed manually.
/// @see PSEnum
/// @see PSRawEnum
class Speller : public Spelli
{

public: // construction
    
    /// speller with given (fixed) enumerator of notes to spell.
    /// @param e external exnumerator of the notes to spell.
    /// must not be null.
    /// @param algo name of the algorithm implemented in speller class.
    /// obsolete. not used anymore.
    /// @param nbtons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    /// @warning this speller cannot be reused for other notes than those
    /// in the given enumerator.
    Speller(PSEnum* e,
            size_t nbtons=0,
            const Algo& algo=Algo::Undef, // TBR
            bool dflag=false);
    
    /// speller with given (fixed) enumerator of notes to spell
    /// and given TonIndex.
    /// @param e external exnumerator of the notes to spell.
    /// must not be null.
    /// @param algo name of the algorithm implemented in speller class.
    /// obsolete. not used anymore.
    /// @param id a Ton Index. must be closed.
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    /// @warning this speller cannot be reused for other notes than those
    /// in the given enumerator.
    Speller(PSEnum* e,
            std::shared_ptr<TonIndex> id,
            const Algo& algo=Algo::Undef,  // TBR
            bool dflag=false);
    
    /// copy constructor forbidden.
    /// makes a deep copy of the note enumerator.
    Speller(const Speller& rhs) = delete;
    
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
    
    // prepare spelling
    //
    
    // the initial state for construction of tables will be tonal.
    // @see PSState
    // void setTonal();
    
    // the initial state for construction of tables will be tonal.
    // @see PSState
    // void setModal();
    
    // wether the transitions for construction of tables shall be deterministic.
    // void setChromatic(bool flag);
    
public: // spelling : computation of tables and grid
    
    /// construct the first spelling table.
    /// @param ctype type of cost domain.
    /// @param tonal tonal or modal construction of the initial state
    /// in each cell.
    /// @param chromatic whether transitions for best-path computation
    /// are deterministic or exhaustive.
    /// @return whether computation was succesfull.
    /// @warning if the table exists it is overwritten.
    /// @see sampleCost
    /// @see PSState for the construction of the initial state
    /// and tonal/modal flag.
    bool evalTable(CostType ctype, bool tonal=true, bool chromatic=false);
    
    /// construct a second spelling table, using
    /// - a first spelling table (use the same index)
    /// - the array of global candidates if it exists
    ///   (otherwise, we consider all tonalities)
    /// - the grid of local tonalities
    /// @param ctype type of cost domain.
    /// @param tonal tonal or modal construction of initial state in each cell.
    /// @param chromatic whether transitions for best-path computation
    /// are deterministic.
    /// @return whether computation was succesfull.
    /// @warning the first spelling table must have been constructed.
    /// @warning the first spelling table is deleted.
    /// @see PSState for the construction of the initial state
    /// and tonal/modal flag
    bool revalTable(CostType ctype, bool tonal=true, bool chromatic=false);
    
    /// construct the grid of local tons (1 ton for each initial ton and measure) using
    /// - a spelling table.
    /// - the array of global candidates if it exists
    ///   (otherwise, we consider all tonalities).
    /// @return whether computation was succesfull.
    bool evalGrid();
    
    /// compute the subarray of tons selected as candidate global tonality,
    /// using the evaluated table.
    /// @param d tolerance distance (in percent) for considering two
    /// cost approximatively equal in the global ton search.
    /// @param refine whether we restart from the current global subarray
    /// and refine, or from scratch.
    /// if true, one global subarray must have been computed.
    /// @return whether computation was succesfull.
    /// @warning a table must have been evaluated.
    bool selectGlobals(double d=0, bool refine=false);

    /// select a unique index on ton in the global tons,
    /// mark all others as non global.
    /// @return whether the opration was successful.
    /// if it was, there is exactly one global ton,
    /// otherwise there is zero.
    bool selectGlobal();

    /// monolithic spelling function.
    /// should not be called for this speller.
    /// @return whether computation was succesfull.
    virtual bool spell();
    
    /// rename all notes read by this speller,
    /// according to a given global tonality.
    /// @param i index of tonality in the index.
    /// @return whether renaming succeded for all measures.
    virtual bool rename(size_t i);
    
    /// rewrite the passing notes in enumerator.
    /// @return how many notes have been rewritten.
    /// @warning must be called after rename.
    /// @see class RewritePassing
    size_t rewritePassing();
    
public: // results feedback : notes
    
    /// estimated name for the note of given index,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the enumerator of input notes.
    enum NoteName name(size_t i) const;
    
    /// estimated name for the note of given index, in -2..2.
    /// @param i index of note in the enumerator of input notes.
    enum Accid accidental(size_t i) const;
    
    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the enumerator of input notes.
    int octave(size_t i) const;
    
    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed
    /// or not.
    /// @param i index of note in the enumerator of input notes.
    bool printed(size_t i) const;

public: // results feedback : grid

    /// the grid of local tonalities has been computaed.
    /// i.e. evalGrid() has been called.
    virtual bool locals() const;
    
    /// index (in the TonIndex) of the estimated local tonality
    /// for one assumed global tonality and one bar.
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j measure number.
    /// @return the index of the estimated local tonality,
    /// assuming the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    /// @warning spell() must have been called.
    virtual size_t ilocal(size_t i, size_t j) const;
    
    /// estimated local tonality for one assumed global tonality and one bar.
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j measure number.
    /// @return the estimated local tonality assuming the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    /// @warning evalGrid() must have been called.
    virtual const Ton& local(size_t i, size_t j) const;
    
    /// estimated local tonality at note of given index, for one given
    /// assumed global tonality.
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j index of note in the enumerator of input notes.
    /// @warning evalGrid() must have been called.
    virtual const Ton& localNote(size_t i, size_t j) const;
    
protected: // data
    
    /// name of the algorithm implemented.
    /// @todo TBR obsolete
    const Algo _algo;
    
    /// raw enumerator allocated internally for this speller, or
    /// null for a speller build on top of a given external enum,
    // std::unique_ptr<PSEnum> _penum;
    
    /// enumerator of the input notes.
    PSEnum* _enum;
    // PSRawEnum _enum;
    
    // tonal or modal construction of initial state for construction of tables.
    // - 0 for constructor default (unset in this speller).
    // - 1 for tonal.
    // - 2 for modal.
    // int _initial_state;
    
    // deterministic transitions for construction of tables
    // bool _chromatic;
    
    /// spelling table.
    /// contains the best spelling paths for each ton and measure.
    PST* _table;
    
    // second spelling table.
    // for 2 steps procedure.
    // PST* _table_post;
    
    /// grid of local tons: 1 ton for each candidate initial ton and measure.
    PSG* _grid; // std::shared_ptr<PSG>
    
    /// sub-array of tons selected as candidate global tonality.
    /// contains a ton index.
    /// @todo TBR. replaced by global tags in ton index.
    PSO* _global;
    
protected: // debug
    
    /// time lapsed, in milli seconds, since the given start date.
    static double duration(clock_t start);
    
};







} // namespace pse

#endif /* Speller_hpp */

/// @}
