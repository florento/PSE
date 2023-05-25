//
//  PSGrid.hpp
//  pse
//
//  Created by Florent Jacquemard on 24/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PSGrid_hpp
#define PSGrid_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <vector>
#include <set>

#include "trace.hpp"
#include "TonIndex.hpp"
#include "PSCost.hpp"
#include "PSBag.hpp"
#include "PSVector.hpp"
#include "PSTable.hpp"

namespace pse {

/// Grid of estimated local tonalities, for each measure (X) and
/// each possible global tonality (Y) in a TonIndex.
class PSG
{
public:
    
    /// main constructor.
    /// @param tab pitch spelling table used to estimated the locals.
    /// @param mask bitvector of global tonalities for which locals must
    /// be estimated. must be of the same length as the Ton index of tab.
    PSG(const PST& tab, std::vector<bool> mask);
    
    virtual ~PSG();
    
    /// vector of tonalities (row-index) associated to this table.
    inline const TonIndex& index() const { return _index; }
    
    /// number of rows in this table, i.e. nb of tons considered for spelling.
    inline size_t rowNb() const { return _index.size(); }
    
    /// number of columns in this table, i.e. nb of measures spelled.
    size_t size() const;
    
    /// number of columns (PS Vectors) in this table, i.e. nb of measures spelled.
    inline size_t columnNb() const { return size(); }
    
    /// index (in the TonIndex) of the estimated local tonality,
    /// for the given row number (assumed a global tonality)
    /// and column number (number of measure).
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j measure number (column number of the table used for
    /// initialization).
    /// @return the estimated local tonality assuming the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    size_t ilocal(size_t i, size_t j) const;
        
private: // data
    
    /// header of rows: array of tonalities (1 per row).
    const TonIndex& _index;
    
    /// columnns: one vector of bags of best paths (target configs) per measure.
    /// - the dimention of inner vectors (columns) is the size of TonIndex.
    ///   @see rowNb()
    /// - the dimention of outer vectors (rows) is the number of measures.
    ///   @see columnNb()
    std::vector<std::vector<size_t>> _content;
    
private:
    
    /// fill this table of local tons.
    void init(const PST& tab, std::vector<bool> mask);

    // add one column
    // void init(const PSV& vec, std::vector<bool> mask);

    /// fill the given set with tonality index with minimal cost in vec
    /// (there can be several tie).
    void extract_bests(const PSV& vec, std::set<size_t>& ties);
    
    /// select in the given set of candidates an index for local tonality,
    /// given an assumed global tonality and previous local tonality.
    size_t estimateLocal(size_t ig, size_t iprev, std::set<size_t>& cands);

    
};

} // namespace pse

#endif /* PSGrid_hpp */

/// @}
