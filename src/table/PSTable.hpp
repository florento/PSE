//
//  PSTable.hpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#ifndef PSTable_hpp
#define PSTable_hpp

#include <iostream>
#include <sstream>   // std::stringstream
#include <assert.h>
#include <memory>
#include <vector>

#include "pstrace.hpp"
#include "AlgoName.hpp"
#include "PSEnum.hpp"
#include "TonIndex.hpp"
#include "Cost.hpp"
#include "PSVector.hpp"
#include "PSGlobal.hpp" // globals
#include "PSGrid.hpp"   // locals


namespace pse {

class PSG;

// table: tonindex (for rows) x header of psvect
//
// tonindex = class with
// - a list of tons (mv _tons)
// size() (< const CEIL) CEIL = max = 200
// addTon()
// resetTons
// const undef() or UNDEF = ceil+1
// const failed() or FAILED = ceil+2
//
// the TonIndex of the table is passed (ref) to every columnn
// (repl. PSV::_tons)


/// A PS Table is a list of PS Vectors, one for each measure.
/// @todo suppr. algo (useless)
class PST
{
    
public: // construction
   
    /// main constructor.
    /// @param a name of pitch-spelling algorithm implemented with this table.
    /// @param seed cost value of specialized type used to create a null cost
    /// of the same type.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param i array of tonalities = header of rows of this table.
    /// @param tonal mode: tonal or modal, for the construction
    /// of initial state. default = modal.
    /// @param dflag debug mode (display table during construction).
    /// @warning the enumerator cannot be changed once the object created.
    PST(const Algo& a, const Cost& seed, const TonIndex& i, PSEnum& e,
        bool tonal, bool dflag=false);

    // main constructor.
    // @param e an enumerator of notes for transitions of configs.
    // @param n0 index of the first note to read in enumerator.
    // @param n1 index of the note after the last note to read in enumerator.
    // must be superior of equal to n0.
    // the sequence is empty iff if n0 == n1.
    // PST(PSEnum& e, size_t n0, size_t n1);
    
    // main constructor.
    // @param e an enumerator of notes for transitions of configs.
    // @param n0 index of the first note to read in enumerator.
    // The enumeration starts at n0 and stops
    // when there are no more notes to read in e.
    // PST(PSEnum& e, size_t n0);
    
    
    /// rebuid a table with the same index as the given table,
    /// and the new given seed and given grid of local tonalities.
    /// @param a name of pitch-spelling algorithm implemented with this table.
    /// @param seed cost value of specialized type used to create a null cost
    /// of the same type.
    /// @param e an enumerator of notes for transitions of configs.
    /// Its dimensions must be the same as tab.
    /// @param locals table of local tonalities for tab.
    /// Its dimensions must be the same as tab.
    /// @param tonal mode: tonal or modal, for the construction
    /// of initial state. default = tonal.
    /// @param dflag debug mode (display table during construction).
    PST(const Algo& a, const Cost& seed, const TonIndex& index, PSEnum& e,
        const PSG& locals, bool tonal, bool dflag=false);

    /// rebuid a table with the same algo and index as the given table,
    /// and the new given seed and given grid of local tonalities.
    /// @param seed cost value of specialized type used to create a null cost
    /// of the same type.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param locals table of local tonalities for tab.
    /// Its dimensions must be the same as tab.
    /// @param tonal mode: tonal or modal, for the construction
    /// of initial state. default = tonal.
    /// @param dflag debug mode (display table during construction).
    // PST(const Cost& seed, const TonIndex& index, PSEnum& e,
    //    const PSG& locals, bool tonal, bool dflag=false);
    
    /// a table cannot be copied
    PST(const PST& rhs) = delete;

    /// destructor.
    virtual ~PST();
    
    /// a table cannot be copied
    PST& operator=(const PST& rhs) = delete;
      
public: // access
    
    /// enumerator of input notes used to build this table.
    inline PSEnum& enumerator() const { return _enum; }
    
    /// vector of tonalities associated to this table.
    /// it is the row-index of this table.
    inline const TonIndex& index() const { return _index; }
    
    /// number of rows in this table, i.e. nb of tons considered for spelling.
    inline size_t rowNb() const { return _index.size(); }

    /// tonality associated to the row of given index in this table.
    /// @param i the index of a row (ie a candidate tonality).
    /// @return the tonality associated to index i in the tonality array.
    const Ton& rowHeader(size_t i) const;

    /// sum of the costs of all cells in row of given index in this table.
    /// @param i the index of a row (ie a candidate tonality).
    /// must be smaller than rowNb().
    /// @return the sum of the costs of all cells in row i
    /// or an indeterminate cost value if the computation failed.
    const Cost& rowCost(size_t i) const;
    
    /// number of columns (PS Vectors) in this table,
    /// i.e. nb of measures spelled.
    size_t size() const;

    /// number of columns (PS Vectors) in this table,
    /// i.e. nb of measures spelled.
    inline size_t measures() const { return size(); }

    /// the enumerator of input notes used to construict the column of given
    /// index in this table.
    /// @param j the index of a column (ie a bar number).
    /// must be smaller than measures().
    const PSEnum& columnHeader(size_t j) const;

    /// access a column (PS vector) of this table.
    /// @param j column number (number of bar). must be smaller than measures().
    PSV& column(size_t j) const;
        
    /// rename all notes read to build this PS table, according to a given
    /// global tonality.
    /// @param ig index of cestimated global tonality = row index.
    /// @return whether renaming succeded for all measures.
    bool rename(size_t ig);

public: // debug

    /// the table content has been correctly initialized.
    bool status() const;
    
    /// debug: print the array of row costs.
    void dump_rowcost() const;
    
    /// debug: print the content of the table in cvs format
    /// in the output stream.
    void print(std::ostream& o) const;

    /// print the content of the table as debug messages
    /// in the output stream.
    void dump_table(std::ostream& o) const;

private: // data

    /// name  of the pitch spelling algorithm implemented with this table.
    const Algo _algo;
    
    /// enumerator of notes used to build this PS table.
    PSEnum& _enum;
    
    /// header of rows: array of tonalities (1 per row).
    const TonIndex& _index;

    /// columnns: one vector of bags of best paths (target configs) per measure.
    std::vector<std::unique_ptr<PSV>> _psvs;

    /// auxiliary vector of the sum of costs for each row
    /// every row of the table corresponds to a tonality in index.
    std::vector<std::shared_ptr<Cost>> _rowcost;

    // second _rowcost : cost re-evaluated (complete cost)
    // for the each row
    // or UNDEF Cost if nont evaluated.
    // @todo local variable?

    // flags : whether _rowcost was really estimated
    // @todo TBR
    // std::vector<bool> _frowcost;
    
    
    // index of candidates best global tonality.
    // if empty and _estimated_globals == false: it was not estimated yet.
    // if empty and _estimated_globals == true: it was estimated but estimation failed.
    // @todo ordered list?
    // if it is empty and _global == UNDEF, then estimateGlobals was not called
    // if it is empty and _global == FAILED, then extimation failed.
    // std::vector<size_t> _globals;

    // the list of global candidates has been estimated,
    // i.e. estimateGlobals() was called.
    // bool _estimated_globals;
    
    // index of estimated global tonality.
    // - TonIndex::UNDEF if it was not estimated yet.
    // - TonIndex::FAILED if its estimation failed.
    // - an integer value between 0 and index.size() otherwise.
    // size_t _global;
    
    // the local tonalities have been estimated,
    // i.e. estimateLocals() was called.
    // @todo TBR. replace by psvs[i].local[j] == UNDEF (or FAIL)
    // bool _estimated_locals;
    
    /// debug mode.
    bool _debug;
    
    
private:

    /// compute the columns (PS Vectors) of this table,
    /// filling cells with cost values.
    /// @param seed cost value of specialized type used to create a null cost
    /// of the same type.
    /// @param locals table of local tonalities for tab.
    /// Ignored if empty, otherwise, its dimensions must be the same as the
    /// ton index and the columns of this table .
    /// @param tonal mode: tonal or modal, for the construction
    /// of initial state.
    /// @return wether the computation was successful.
    bool init_psvs(const Cost& seed, const PSG& locals, bool tonal=false);

    /// compute the columns (PS Vectors) of this table,
    /// filling cells with cost values.
    /// @param tab PS table with same algo, ton index, and enumerator as
    /// this table (and former pitch spelling content).
    /// @param seed cost value of specialized type used to create a null cost
    /// of the same type.
    /// @param locals table of local tonalities for tab.
    /// Its dimensions must be the same as the index of this table and
    /// the columns of tab.
    /// @param tonal mode: tonal or modal, for the construction
    /// of initial state.
    /// @return wether the computation was successful.
    // bool init_psvs(const PST& tab, const Cost& seed,
    //                const PSG& locals, bool tonal=true);
    
    // initialise the vector of row costs as null vector.
    // void init_rowcosts(const Cost& seed);
    
    /// fill the the vector of row costs.
    /// @param globals compute the row costs only for candidate global
    /// tonalities in the index of this table.
    void compute_rowcosts(const Cost& seed, bool globals=false);

    // fill the the vector of row costs for given global tonalities.
    // @param globals candidate global tonalities.
    // Its dimensions must be the same as the index of this table and
    // the columns of tab.
    // @todo remove globals, replaced by global flag in ton index.
    // void compute_rowcosts(const Cost& seed, const PSO& globals);
        
    /// @param bar measure number (first is 0)
    /// @param i0 index of first note of the measure (wrt the enumerator)
    /// @return the index of the first note of the next bar, or the total
    /// number of note (index of last note + 1) if end of part is reached.
    size_t bound_measure(size_t bar, size_t i0);
  
};


} // namespace pse

#endif /* PSTable_hpp */

/// @}



// force a global tonality.
// @param ig index of global tonality.
// void setGlobal(size_t ig);

// estimate the global tonality candidate for this table (first step).
// @return whether the estimation of the global tonality successed.
// @warning call eGlobals_eq and eGlobals_less variants to operator==
// and operator<  on cost.
// bool estimateGlobals();

// a set of candidate global tonalities is known.
// estimateGlobals or setGlobal was called.
// bool estimatedGlobals() const;

// number of candidates (ties) for the estimatation of the global tonality.
// @warning estimateGlobals() must have been called successfully.
// size_t globalCands() const;

// candidate global tonality for this table.
// @param i candidate number, must be in 0..globalCands().
// @warning estimateGlobal() must have been called successfully.
// @warning estimateGlobal() must have been called successfully.
// const Ton& globalCand(size_t i) const;

// index of a candidate global tonality for this table, in 0..index.size().
// @warning estimGlobal() must have been called successfully.
// size_t iglobalCand(size_t i) const;
    
// estimated local tonality for one candidate global tonality and one bar.
// @param i row index = index of candidate global tonality.
// must be smaller than index.size().
// @param j column index = bar number. must be smaller than enum.size().
// @warning estimLocals() must have been called.
// const Ton& local(size_t i, size_t j) const;

// index of the estimated local tonality for one candidate global tonality
// and one bar.
// @param i row index = index of candidate global tonality.
// must be smaller than size().
// @param j column index = bar number. must be smaller than index.size().
// @warning estimLocals() must have been called.
// size_t ilocal(size_t i, size_t j) const;
    
// estimate the best global tonality for this table
// (second step, after estimation local tonalities).
// @return whether the estimation of the global tonality successed.
// @warning estimLocals() must have been called.
// bool estimateGlobal();

// global tonality is known.
// estimateGlobal or setGlobal was called
// bool estimatedGlobal() const;

// estimated global tonality for this table, in 0..index.size().
// @warning estimGlobal() must have been called successfully.
// @todo change to const Ton& global(size_t i) const; (ith-best)
// const Ton& global() const;

// index of the estimated global tonality for this table,
// in 0..index.size().
// @warning estimGlobal() must have been called successfully.
// size_t iglobal() const;

// cumulative sum of cost of one row.
// @param step number of step (category of psb (is psv) to sum): 0 or 1.
// @param i row number, in the index of tons of this table.
// PSCost rowCost(size_t step, size_t i);

// accessor to the tabulated sum of costs for row (ton) of given index.
//unsigned int rowcost(size_t i) const;

// accessor to the flag for sum of costs for row (ton) of given index.
// bool frowcost(size_t i) const;

/// debug: one row cost at least has been estimated.
/// if one psb is empty, the whole column is empty
/// hence if a whole row could no be estimated, all columns are empty
/// hence the table is empty.
// bool check_rowcost(const std::vector<PSCost>& rc) const;


// estimate local tonalities in every column,
// for each potential global tonality.
// bool init_locals();

// estimate a local tonality for each column of this table,
// for all candidate global tonalities in _globals.
// @return whether estimation of the local tonalities successed.
// @warning estimGlobals() must have been called successfully.
// bool init_locals_globals();

// estimate a local tonality for each column of this table,
// for an assumed global tonality ig.
// @param ig the index of a candidate global tonality.
// @return whether estimation of the local tonalities successed.
// @warning estimGlobal() must have been called successfully.
// bool init_locals(size_t ig);

// local tonalities are known.
// estimateLocals was called.
// bool estimatedLocals() const;

// macro: cost equality for the estimation of global.
// use if Costt.operator== is eq_lex.
// @warning static choice
// bool eGlobals_eq_lex(const PSCost& lhs, const PSCost& rhs) const;

// macro: cost equality for the estimation of global.
// use if Costt.operator== is eq_cumul.
// @warning static choice
// inline bool eGlobals_eq_cumul(const PSCost& lhs, const PSCost& rhs) const;

// macro: cost equality for the estimation of global
// @warning static choice
// inline bool eGlobals_neq(const PSCost& lhs, const PSCost& rhs) const
// { return (lhs != rhs); }
// { return lhs.neq_approx(rhs, _enum.length()); }

// macro: cost ordering for the estimation of global
// @warning static choice
// inline bool eGlobals_less(const PSCost& lhs, const PSCost& rhs) const
// { return (lhs < rhs); }
// { return lhs.less_approx(rhs, _enum.length()); }
