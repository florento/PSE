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
#include <assert.h>
#include <memory>
#include <vector>

#include "trace.hpp"
#include "PSEnum.hpp"
#include "TonIndex.hpp"
#include "PSVector.hpp"

namespace pse {



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
class PST
{
    
public:

    /// header of rows: vector of tonalities (1 per row).
    TonIndex index;
    
    /// @todo param step number (default 0)
    /// main constructor.
    /// @param e an enumerator of notes for transitions of configs.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode (display table during construction).)
    /// @see TonIndex for supported values.
    /// @warning the enumerator cannot be changed once the object created.
    PST(PSEnum& e, size_t nbTons=0, bool dflag=false);

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

    virtual ~PST();
          
    /// number of columns (PS Vectors) in this table, i.e. nb of measures spelled.
    size_t size() const;
    
    /// number of rows in this table, i.e. nb of tons considered for spelling.
    inline size_t nbTons() const { return index.size(); }

    /// tonality associated to the ith row of this table.
    inline const Ton& ton(size_t i) const { return index.ton(i); }

    /// access the ith column (PS vector) of this table.
    /// @param i column number. must be smaller than size().
    PSV& column(size_t i);
    
    /// @todo param step number (default 0)
    /// compute the columns (PS Vectors) of this table, fill with cost values.
    /// @return wether the computation was successful.
    bool init();
    
    /// force a global tonality.
    /// @param ig index of global tonality.
    void setGlobal(size_t ig);

    /// estimate the global tonality candidate for this table (first step).
    /// @return whether the estimation of the global tonality successed.
    /// @warning call eGlobals_eq and eGlobals_less variants to operator==
    /// and operator<  on cost.
    bool estimateGlobals();
    
    /// a set of candidate global tonalities is known.
    /// estimateGlobals or setGlobal was called.
    bool estimatedGlobals() const;

    /// number of candidates (ties) for the estimatation of the global tonality.
    /// @warning estimateGlobals() must have been called successfully.
    size_t globalCands() const;

    /// candidate global tonality for this table.
    /// @param i candidate number, must be in 0..globalCands().
    /// @warning estimateGlobal() must have been called successfully.
    /// @warning estimateGlobal() must have been called successfully.
    const Ton& globalCand(size_t i) const;
    
    /// index of a candidate global tonality for this table, in 0..index.size().
    /// @warning estimGlobal() must have been called successfully.
    size_t iglobalCand(size_t i) const;
        
    /// estimate a local tonality for each column of this table,
    /// for all candidate global tonalities in _globals.
    /// @return whether estimation of the local tonalities successed.
    /// @warning estimGlobals() must have been called successfully.
    bool estimateLocals();
    
    /// local tonalities are known.
    /// estimateLocals was called.
    bool estimatedLocals() const;

    /// estimate a local tonality for each column of this table,
    /// for an assumed global tonality ig.
    /// @param ig the index of a candidate global tonality.
    /// @return whether estimation of the local tonalities successed.
    /// @warning estimGlobal() must have been called successfully.
    bool estimateLocals(size_t ig);

    /// estimated local tonality for one candidate global tonality and one bar.
    /// @param i row index = index of candidate global tonality.
    /// must be smaller than index.size().
    /// @param j column index = bar number. must be smaller than index.size().
    /// @warning estimLocals() must have been called.
    const Ton& local(size_t i, size_t j) const;

    /// index of the estimated local tonality for one candidate global tonality
    /// and one bar.
    /// @param i row index = index of candidate global tonality.
    /// must be smaller than size().
    /// @param j column index = bar number. must be smaller than index.size().
    /// @warning estimLocals() must have been called.
    size_t ilocal(size_t i, size_t j) const;
        
    /// estimate the best global tonality for this table
    /// (second step, after estimation local tonalities).
    /// @return whether the estimation of the global tonality successed.
    /// @warning estimLocals() must have been called.
    bool estimateGlobal();
    
    /// global tonality is known.
    /// estimateGlobal or setGlobal was called
    bool estimatedGlobal() const;

    /// estimated global tonality for this table, in 0..index.size().
    /// @warning estimGlobal() must have been called successfully.
    /// @todo change to const Ton& global(size_t i) const; (ith-best)
    const Ton& global() const;
    
    /// index of the estimated global tonality for this table,
    /// in 0..index.size().
    /// @warning estimGlobal() must have been called successfully.
    size_t iglobal() const;
    
    /// rename all notes read to build this PS table.
    /// estimateGlobal() and estimateLocals() are called
    /// if this was not done before.
    /// @return whether renaming succeded for all measures.
    /// @warning Global() must have been called.
    bool rename();
    
private: // data

    // vector of tonalities = headers of rows of this table
    // std::vector<const Ton> _tons;
    // @todo replace by TonIndex

    /// enumerator of notes used to build this PS table.
    PSEnum& _enum;
    
    /// columnns: one vector of bags of best paths (target configs) per measure
    std::vector<std::unique_ptr<PSV>> _psvs;

    /// index of candidates best global tonality.
    /// if empty and _estimated_globals == false: it was not estimated yet.
    /// if empty and _estimated_globals == true: it was estimated but estimation failed.
    /// @todo ordered list?
    /// if it is empty and _global == UNDEF, then estimateGlobals was not called
    /// if it is empty and _global == FAILED, then extimation failed.
    std::vector<size_t> _globals;

    // the list of global candidates has been estimated,
    // i.e. estimateGlobals() was called.
    // bool _estimated_globals;
    
    /// index of estimated global tonality.
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    size_t _global;
    
    /// the local tonalities have been estimated,
    /// i.e. estimateLocals() was called.
    /// @todo TBR. replace by psvs[i].local[j] == UNDEF (or FAIL)
    bool _estimated_locals;
    
    /// debug mode.
    bool _debug;
    
    // intermediate vector of the sum of costs for each row
    // every row of the table corresponds to a tonality.
    // @todo change to vect<PSCost>
    // @todo local variable?
    // std::vector<unsigned int> _rowcost;

    // second _rowcost : cost re-evaluated (complete cost)
    // for the each row
    // or UNDEF Cost if nont evaluated.
    // @todo local variable?

    // flags : whether _rowcost was really estimated
    // @todo TBR
    // std::vector<bool> _frowcost;
    
private:
    
    /// macro: cost equality for the estimation of global.
    /// use if Cost.operator== is eq_lex.
    /// @warning static choice
    bool eGlobals_eq_lex(const PSCost& lhs, const PSCost& rhs) const;

    /// macro: cost equality for the estimation of global.
    /// use if Cost.operator== is eq_cumul.
    /// @warning static choice
    inline bool eGlobals_eq_cumul(const PSCost& lhs, const PSCost& rhs) const;

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
    
    /// @param bar measure number (first is 0)
    /// @param i0 index of first note of the measure (wrt the enumerator)
    /// @return the index of the first note of the next bar, or the total
    /// number of note (index of last note + 1) if end of part is reached.
    size_t bound_measure(size_t bar, size_t i0);
  
    /// cumulative sum of cost of one row.
    /// @param step number of step (category of psb (is psv) to sum): 0 or 1.
    /// @param i row number (index of ton)
    PSCost rowCost(size_t step, size_t i);
    
    // accessor to the tabulated sum of costs for row (ton) of given index.
    //unsigned int rowcost(size_t i) const;

    // accessor to the flag for sum of costs for row (ton) of given index.
    // bool frowcost(size_t i) const;

    /// debug: one row cost at least has been estimated.
    /// if one psb is empty, the whole column is empty
    /// hence if a whole row could no be estimated, all columns are empty
    /// hence the table is empty.
    // bool check_rowcost(const std::vector<PSCost>& rc) const;

    /// debug: print the array of row costs.
    void dump_rowcost(const std::vector<PSCost>& rc) const;
   
    /// debug: print the cottent of the table.
    void dump_table(const std::vector<PSCost>& rc) const;
    
};


} // namespace pse

#endif /* PSTable_hpp */

/// @}
