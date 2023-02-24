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

    /// main constructor.
    /// @param e an enumerator of notes for transitions of configs.
    /// @warning the enumerator cannot be changed once the object created.
    PST(PSEnum& e);

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
       
    /// compute the columns (PS Vectors) of this table, fill with cost values.
    /// @return wether the computation was successful.
    bool init();
    
    /// number of columns (PS Vectors)  in this table (i.e. nb of measures).
    size_t size() const;
        
    /// access the ith column (PS vector) of this table.
    /// @param i column number. must be smaller than size().
    PSV& column(size_t i);
    
    /// number of rows (dimension of PS Vectors) in this table
    /// (i.e. nb of tonalities considered).
    size_t nbTons() const;

    /// Row header = tonality corresponding to the given row index.
    /// @param i an index in array of tonalities. must be smaller than NBTONS.
    const Ton& ton(size_t i) const;
    
    /// empty the list of row headers (tonalities).
    /// @see addTon
    void resetTons();

    ///add a tonality to the list of row headers.
    void addTon(const Ton ton);
    
    ///add a tonality to the list of row headers.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of this tonality.
    /// @see Ton
    void addTon(int ks, Ton::Mode mode = Ton::Mode::Maj);
    
    /// estimate the global tonality for this table.
    /// @return whether the estimation of the global tonality successed.
    bool estimateGlobal();

    /// estimated global tonality for this table, in 0..NBTONS.
    /// @warning estimGlobal() must have been called successfully.
    /// @todo change to const Ton& global(size_t i) const; (ith-best)
    const Ton& global() const;

    /// estimate a local tonality for each column of this table.
    /// @return whether estimation of the local tonalities successed.
    /// @warning estimGlobal() must have been called successfully.
    /// @todo change to estimateLocals(size_t i) const;
    /// (for ith-best global)
    /// or estimateLocals(Ton& global)
    bool estimateLocals();

    /// estimated local tonality for the ith column of this table,
    /// in 0..nbtons().
    /// @param i column number. must be smaller than size().
    /// @warning estimLocals() must have been called.
    /// @todo change to estimateLocals(size_t i, size_t j) const;
    /// (for jth-best global)
    /// or estimateLocals(size_t i, Ton& global)
    const Ton& local(size_t i) const;
    
    /// rename all notes read to build this PS table.
    /// estimateGlobal() and estimateLocals() are called
    /// if this was not done before.
    /// @return whether renaming succeded for all measures.
    bool rename();
    
    
private: // data

    /// vector of tonalities = headers of rows of this table
    std::vector<const Ton> _tons;
    /// @todo TonIndex

    /// enumerator of notes used to build this PS table.
    PSEnum& _enum;
    
    /// columnns:
    /// one vector of bags of best paths (target configs) per measure
    std::vector<std::unique_ptr<PSV>> _psvs;

    /// index of global tonality in 0..nbtons()
    size_t _global;
    /// @todo replace by vect<size_t> ordered list of best tons
    
    /// the global tonality has been estimated,
    /// i.e. estimateGlobal() was called.
    /// @todo TBR (replaced by _globbal != TonIndex::UNDEF)
    bool _estimated_global;

    /// the local tonalities have been estimated,
    /// i.e. estimateLocals() was called.
    /// @todo TBR
    bool _estimated_locals;
    
    /// intermediate vector of the sum of costs for each row
    /// every row of the table corresponds to a tonality.
    /// @todo change to vect<PSCost>
    std::vector<unsigned int> _rowcost;

    /// @todo
    /// second _rowcost : cost re-evaluated (complete cost)
    /// for the each row
    /// or UNDEF Cost if nont evaluated.
    
    /// flags : whether _rowcost was really estimated
    std::vector<bool> _frowcost;
    
private:
    
    /// @param bar measure number (first is 0)
    /// @param i0 index of first note of the measure (wrt the enumerator)
    /// @return the index of the first note of the next bar, or the total
    /// number of note (index of last note + 1) if end of part is reached.
    size_t bound_measure(size_t bar, size_t i0);

    /// cumul sum of every row.
    /// @warning: the table must not be empty
    void computeRowcost();
    
    /// accessor to the tabulated sum of costs for row (ton) of given index.
    unsigned int rowcost(size_t i) const;

    /// accessor to the flag for sum of costs for row (ton) of given index.
    bool frowcost(size_t i) const;

    /// index of the estimated global tonality for this table,
    /// in 0..nbtons().
    /// @warning estimGlobal() must have been called successfully.
    size_t iglobal() const;
    
    /// index of the estimated local tonality for the ith column of this table,
    /// in 0..nbtons().
    /// @param i column number. must be smaller than size().
    /// @warning estimLocals() must have been called.
    size_t ilocal(size_t i) const;

    /// debug: one row cost at least has been estimated.
    bool check_rowcost() const;

    /// debug: print the array of row costs.
    void dump_rowcost() const;

    /// debug: print the cottent of the table.
    void dump_table() const;
    
};


} // namespace pse

#endif /* PSTable_hpp */

/// @}
