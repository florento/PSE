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
#include "PSTable.hpp"
#include "PSGrid.hpp"


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
    /// @param e_aux optional auxiliary enumerator of alternative input notes,
    /// for the construction of the first table only.
    /// @param algo name of the algorithm implemented in speller class.
    /// obsolete. not used anymore.
    /// @param nbtons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    /// @warning this speller cannot be reused for other notes than those
    /// in the given enumerator.
    Speller(PSEnum* e, PSEnum* e_aux = nullptr,
            size_t nbtons=0,
            const Algo& algo=Algo::Undef, // TBR
            bool dflag=false);
    
    /// speller with given (fixed) enumerator of notes to spell
    /// and given TonIndex.
    /// @param e external exnumerator of the notes to spell.
    /// must not be null.
    /// @param id a Ton Index. must be closed.
    /// @param e_aux optional auxiliary enumerator of alternative input notes,
    /// for the construction of the first table only.
    /// @param algo name of the algorithm implemented in speller class.
    /// obsolete. not used anymore.
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    /// @warning this speller cannot be reused for other notes than those
    /// in the given enumerator.
    Speller(PSEnum* e,
            std::shared_ptr<TonIndex> id,
            PSEnum* e_aux = nullptr,
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
    
public: // access
    
    /// name of algorithm implemented.
    /// @return Algo::Undef by default.
    const Algo& algo() const { return _algo; }

    /// size of the enumerator.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    virtual size_t size(bool aux=false) const;
    
    // the initial state for construction of tables will be tonal.
    // @see PSState
    // void setTonal();
    
    // the initial state for construction of tables will be tonal.
    // @see PSState
    // void setModal();
    
    // wether the transitions for construction of tables shall be deterministic.
    // void setChromatic(bool flag);

    /// this speller has an optional auxiliary enumerator.
    bool hasAuxEnumerator() const;

protected: // parameters and auxiliary enumerator
    
    /// add the given speller an optional auxiliary enumerator.
    /// @param aux an enumerator. must not be NULL.
    /// @return whether the operation was succesful.
    /// @warning fors nothing if there is alreay an auxiliary enumerator.
    bool setAuxEnumerator(PSEnum* aux);
    
public: // spelling : computation of tables and grid
    
    /// construct the first spelling table.
    /// @param ctype type of cost domain.
    /// @param tonal tonal or modal construction of the initial state
    /// in each cell.
    /// @param octave mode for the state transitions: repeat accidents
    /// at different octaves, or reason modulo 12.
    /// @param chromatic whether transitions for best-path computation
    /// are deterministic or exhaustive.
    /// @param aux wether we use the auxilliary enumerator for the evaluation.
    /// in that case it must be set.
    /// @return whether computation was succesfull.
    /// @warning if the table exists it is overwritten.
    /// @see sampleCost
    /// @see PSState for the construction of the initial state
    /// and tonal/modal flag.
    bool evalTable(CostType ctype,
                   bool tonal=true, bool octave=false,
                   bool chromatic=false, bool aux=false);
    
    /// construct a second spelling table, using
    /// - a first spelling table (use the same index)
    /// - the array of global candidates if it exists
    ///   (otherwise, we consider all tonalities)
    /// - the grid of local tonalities
    /// @param ctype type of cost domain.
    /// @param tonal tonal or modal construction of initial state in each cell.
    /// @param octave mode for the state transitions: repeat accidents
    /// at different octaves, or reason modulo 12.
    /// @param chromatic whether transitions for best-path computation
    /// are deterministic.
    /// @param aux wether we use the auxilliary enumerator for the evaluation.
    /// in that case it must be set.
    /// @return whether computation was succesfull.
    /// @warning the first spelling table must have been constructed.
    /// @warning the first spelling table is deleted.
    /// @see PSState for the construction of the initial state
    /// and tonal/modal flag
    bool revalTable(CostType ctype,
                    bool tonal=true, bool octave=false,
                    bool chromatic=false, bool aux=false);
    
    /// construct the grid of local tons
    /// (1 ton for each initial ton and measure) using
    /// - a spelling table.
    /// - the array of global candidates if it exists
    ///   (otherwise, we consider all tonalities).
    /// @param algo algorithm for the computation of the grid.
    /// possible values are GridAlgo::Best, GridAlgo::Rank,
    /// GridAlgo::Exhaustive.
    /// @return whether computation was succesfull.
    bool evalGrid(const GridAlgo& algo = GridAlgo::Rank);
    
    /// compute the subarray of tons selected as candidate global tonality,
    /// using the evaluated table.
    /// @param d tolerance distance (in percent) for considering two
    /// cost approximatively equal in the global ton search.
    /// @param refine whether we restart from the current global subarray
    /// and refine, or from scratch.
    /// if true, one global subarray must have been computed.
    /// @return the number of tonalities selected by this function.
    /// @warning a table must have been evaluated.
    size_t selectGlobals(double d=0, bool refine=true);

    /// select a unique index on ton in the global tons,
    /// mark all others as non global.
    /// @return whether the operation was successful.
    /// if it was, there is exactly one global ton,
    /// otherwise there is zero.
    bool selectGlobal();

    /// monolithic spelling function.
    /// should not be called for this speller.
    /// @return whether computation was succesfull.
    virtual bool spell();
    
    /// rename the notes of the enumerator of the current spell table,
    /// according to this table, for a given tonality.
    /// @param i index of tonality in the index.
    /// @return whether renaming succeded for all measures.
    virtual bool rename(size_t i);
    
    /// rewrite the passing notes in enumerator.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    /// @return how many notes have been rewritten.
    /// @warning must be called after rename.
    /// @see class RewritePassing
    size_t rewritePassing(bool aux=false);

    /// clear the current spelling table.
    void resetTable();

    /// clear the current grid.
    void resetGrid();
    
public: // results feedback : notes
    
    /// nb of measures processed by this speller.
    /// it is the number of measure in the enumarator.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    size_t measures(bool aux=false) const;
    
    /// estimated name for the note of given index,
    /// in 0..6 (0 is 'C', 6 is 'B').
    /// @param i index of note in the enumerator of input notes.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    enum NoteName name(size_t i, bool aux=false) const;
    
    /// estimated name for the note of given index, in -2..2.
    /// @param i index of note in the enumerator of input notes.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    enum Accid accidental(size_t i, bool aux=false) const;
    
    /// estimated octave for the note of given index in the best path, in -2..9.
    /// @param i index of note in the enumerator of input notes.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    int octave(size_t i, bool aux=false) const;
    
    /// estimated print flag for the note of given index in the best path.
    /// This flags says wether the accidental of the note must be printed
    /// or not.
    /// @param i index of note in the enumerator of input notes.
    /// @param aux wether we consider the auxilliary enumerator.
    /// in that case it must be set.
    bool printed(size_t i, bool aux=false) const;

public: // results feedback : grid

    /// the grid of local tonalities has been computed.
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
    /// @param j index of note in the enumerator of input notes
    /// used for the computation of the grid.
    ///
    /// @warning evalGrid() must have been called.
    virtual const Ton& localNote(size_t i, size_t j) const;

public: // debug
    
    void printGrid(std::ostream& o) const;

    void printGrid_pybind() const;

    void writeGrid(const std::string& filename) const;

    void printTable(std::ostream& o) const;

    void printTable_pybind() const;

    void writeTable(const std::string& filename) const;

protected: // data
    
    /// name of the algorithm implemented.
    /// @todo TBR obsolete
    const Algo _algo;
    
    /// raw enumerator allocated internally for this speller, or
    /// null for a speller build on top of a given external enum,
    // std::unique_ptr<PSEnum> _penum;
    
    /// enumerator of the input notes.
    PSEnum* _enum;

    /// auxiliary enumerator of alternative input notes,
    /// for the construction of the first table only.
    PSEnum* _enum_aux;
    
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
    
    // sub-array of tons selected as candidate global tonality.
    // contains a ton index.
    // @todo TBR. replaced by global tags in ton index.
    // PSO* _global;
    
protected: // debug
    
    /// time lapsed, in milli seconds, since the given start date.
    static double duration(clock_t start);

    /// access the internal note enumerator.
    /// @param aux whether we want the auxiliary enumerator.
    /// in this case it must be set.
    PSEnum& enumerator(bool aux=false) const;
    
};

} // namespace pse

#endif /* Speller_hpp */

/// @}
