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

#include "trace.hpp"
#include "Rational.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
#include "PSRawEnum.hpp"
#include "PSTable.hpp"
#include "AlgoName.hpp"
#include "CostType.hpp"
#include "CostA.hpp"
#include "CostADplus.hpp"
#include "CostADlex.hpp"



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
    
    /// set debug mode (log messages for debugging)
    void debug(bool flag);

    /// number of input notes in the enumerator of notes to spell.
    size_t size() const;

    
    //
    // notes
    //
    
    /// empty the list of notes in the enumerator of this speller.
    /// @param i0 new index of the first note accessible by this enumerator.
    /// @param i1 new index of the note after the last note accessible by this
    /// enumerator. optional (can be ommited for open PS Enum).
    /// if given it must be larger than or equal to first.
    void reset(size_t i0, size_t i1 = PSEnum::ID_INF);
    
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


    //
    // array of tonalities
    //
    
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
    /// @param global whether ton can be considered as a global tonality.
    /// @see Ton
    void addTon(int ks, ModeName mode = ModeName::Major, bool global=true);

    /// for Python binder, same as addTon (overloaded).
    inline void addTon3(int ks, ModeName mode, bool global)
    { addTon(ks, mode, global); }
    
    /// add a tonality for pitch spelling.
    /// @param ton the tonality to add to this speller.
    /// @param global whether ton can be considered as a global tonality.
    void addTon(const Ton& ton, bool global=true);
    
    /// switch the array of tonalities to tonal mode
    /// for the conmputation of Weber distance at closing.
    /// @warning the array of tonalities array must not be closed.
    void WeberTonal();

    /// switch the array of tonalities to tonal mode
    /// for the conmputation of Weber distance at closing.
    /// @warning the array of tonalities array must not be closed.
    void WeberModal();

    /// close the array of tonalities and finish its initlialization.
    /// No ton can be added after closure.
    void closeTons(bool tonal_flag);
    
    /// the array of tonalities is closed.
    bool closedTons() const;
       
    
    //
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
    
    //
    // spelling
    //

    /// construct the first spelling table.
    /// @param type of cost domain.
    /// @param tonal tonal or modal construction of initial state in each cell.
    /// @param chromatic whether transitions for best-path comp. are deterministic.
    /// @return whether computation was succesfull.
    /// @warning if the table exists it is overwritten.
    /// @see sampleCost
    bool evalTable(CostType ctype, bool tonal=true, bool chromatic=false);

    /// construct a second spelling table, using
    /// - a first spelling table (use the same index)
    /// - the array of global candidates if it exists
    ///   (otherwise, we consider all tonalities)
    /// - the grid of local tonalities
    /// @param ctype type of cost domain.
    /// @param tonal tonal or modal construction of initial state in each cell.
    /// @param chromatic whether transitions for best-path comp. are deterministic.
    /// @return whether computation was succesfull.
    /// @warning the first spelling table must have been constructed.
    /// @warning the first spelling table is deleted.
    bool revalTable(CostType ctype, bool tonal=true, bool chromatic=false);

    /// construct the grid of local tons (1 ton for each initial ton and measure)
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
    /// if true, one global sbuarray must have been computed.
    /// @return whether computation was succesfull.
    /// @warning a table must have been evaluated.
    bool evalGlobal(double d, bool refine=false);
    
    /// monolithic spelling function.
    /// should not be called for this speller.
    /// @return whether computation was succesfull.
    virtual bool spell();

    /// rename all notes read by this speller, according to a given global tonality.
    /// @param i index of tonality in the index.
    /// @return whether renaming succeded for all measures.
    virtual bool rename(size_t i);
    
    /// rewrite the passing notes in enumerator.
    /// @return how many notes have been rewritten.
    /// @warning must be called after rename.
    /// @see class RewritePassing
    size_t rewritePassing();
    
    
    //
    // results feedback
    //
    
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
    
    /// number of candidates estimated global tonality (ties).
    /// @return the size of _global or 0 if evalGlobal was not called.
    virtual size_t globals() const;
    
    /// n-best estimated global tonality.
    /// @param n number of candidate estimated global tonality,
    /// must be in 0..globals().
    /// @return the n-best estimated global tonality.
    /// It is ton(iglobal(n)) or an undef ton in case of error.
    /// @warning evalGlobal must have been called.
    virtual const Ton& global(size_t n = 0) const;

    /// index of the n-best estimated global tonality.
    /// @param n number of candidate estimated global tonality,
    /// must be in 0..globals().
    /// @return the index of the n-best estimated global tonality
    /// in the index of tons, in 0..index.size()
    /// or TonIndex::UNDEF in case of error.
    /// @warning spell() must have been called.
    virtual size_t iglobal(size_t n = 0) const;
        
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

    /// enumerator of the input notes.
    PSRawEnum _enum;

    /// array of tonalities that shall be considered for pitch spelling.
    TonIndex _index;
    
    /// @warning we compute one sample cost for cost type.

    // sample cost (zero) for the construction of tables.
    // @warning discount Obsolete for CostA
    // CostA _seedAdiscount;

    /// sample cost (zero) for the construction of tables.
    CostA _seedAnodiscount;

    /// sample cost (zero) for the construction of tables.
    CostADplus _seedADplus;

    /// sample cost (zero) for the construction of tables.
    CostADlex _seedADlex;

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

    /// grid of local tons (1 ton for each initial ton and measure)
    PSG* _grid; // std::shared_ptr<PSG>

    /// subarray of tons selected as candidate global tonality.
    PSO* _global;
    
    /// debug mode activated.
    bool _debug;
    
    /// undefined tonality, for errors.
    Ton* _uton; // std::shared_ptr<Ton>
    
protected:
    
    /// time lapsed, in milli seconds, since the given start date.
    static double duration(clock_t start);
    
    
private:
    
    /// construct a sample cost value (zero) for the construction of tables.
    /// @param ct type of cost domain.
    /// @return the cost value constructed.
    Cost& sampleCost(CostType ct);

    /// @param c code for cost type:
    /// - 00 costA with discount for lead tons
    /// - 01 costA without discount for lead tons
    /// - 02 costADplus
    /// - 03 costADlex

};

} // namespace pse

#endif /* Speller_hpp */

/// @}
