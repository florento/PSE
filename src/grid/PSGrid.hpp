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

#include "pstrace.hpp"
#include "utils.hpp"
#include "TonIndex.hpp"
#include "Cost.hpp"
#include "PSVector.hpp"
//#include "PSTable.hpp"

namespace pse {

class PST;

/// names of algorithms for the computation of the grid.
enum class GridAlgo
{
    /// Unknown
    Undef,

    /// Greedy algo with best costs
    Best,

    /// Greedy algo with ranks
    Rank,

    /// Best path search
    Exhaustive,

};

/// string of mode name.
std::string tostring(const GridAlgo& m);

std::ostream& operator<<(std::ostream& o, const GridAlgo& m);


/// Grid of estimated local tonalities, for each measure (X) and
/// each possible global tonality (Y) in a TonIndex.
/// This class only construct an empty grid, descendants shall construct
/// a concrete grid from a table.
/// @todo grid with a different TonIndex as the PST table
class PSG
{
    
public: // construction
        
    /// main constructor.
    /// @param tab pitch spelling table used to estimated the locals.
    // @param mask bitvector of global tonalities for which locals must
    // be estimated. must be of the same length as the Ton index of tab.
    // @todo suppr. mask. use the TonIndex of tab
    // (or an optional second TonIndex given)
    /// @todo flag mask
    /// @todo flak atonal
    PSG(const PST& tab);
    
    /// a grid cannot be copied
    PSG(const PSG& rhs) = delete;
    
    // constructor without mask.
    // @param tab pitch spelling table used to estimated the locals.
    // PSG(const PST& tab);
    
    virtual ~PSG();
    
    /// a grid cannot be copied
    PSG& operator=(const PSG& rhs) = delete;

public: // access
    
    /// number of columns in this grid, i.e. nb of measures spelled.
    size_t size() const;
    
    /// number of columns (PS Vectors) in this grid, i.e. nb of measures
    /// spelled.
    /// @see same as PSG::size()
    inline size_t measures() const { return size(); }

    /// vector of tonalities (row-index) associated to this grid.
    inline const TonIndex& index() const { return _index; }
    
    /// number of rows in this grid, i.e. nb of tons considered for spelling.
    inline size_t nbTons() const { return _index.size(); }
        
    /// one column of this table (corresponding to a measure).
    /// @param j measure number.
    const std::vector<size_t>& column(size_t j) const;
    
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
    
    /// estimated local tonality,
    /// for the given row number (assumed a global tonality)
    /// and column number (number of measure).
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j measure number (column number of the table used for
    /// initialization).
    /// @return the estimated local tonality assuming the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    const Ton& local(size_t i, size_t j) const;
    
protected: // data
    
    /// header of rows: array of tonalities (1 per row).
    const TonIndex& _index;
    
    /// columnns: one vector of index of local tons per measure.
    /// - the dimension of inner vectors (columns) is the size of TonIndex.
    ///   @see rowNb()
    /// - the dimension of outer vectors (rows) is the number of measures.
    ///   @see measures()
    std::vector<std::vector<size_t>> _content;
    
    // add one column
    // void init(const PSV& vec, std::vector<bool> mask);
             
protected: // construction

    // fill a grid (with the size of tab) with TonIndex::UNDEF
    void init_empty(const PST& tab);
    
};

} // namespace pse


/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<enum pse::GridAlgo> : fmt::ostream_formatter {};


#endif /* PSGrid_hpp */

/// @}
