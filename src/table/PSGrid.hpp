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
#include "PSBag.hpp"
#include "PSVector.hpp"
//#include "PSTable.hpp"

namespace pse {

class PST;

/// Grid of estimated local tonalities, for each measure (X) and
/// each possible global tonality (Y) in a TonIndex.
/// @todo grid with a different TonIndex as the PST table
class PSG
{
public:
        
    /// main constructor.
    /// @param tab pitch spelling table used to estimated the locals.
    /// @param mask bitvector of global tonalities for which locals must
    /// be estimated. must be of the same length as the Ton index of tab.
    PSG(const PST& tab, std::vector<bool> mask);
    
    /// constructor without mask.
    /// @param tab pitch spelling table used to estimated the locals.
    PSG(const PST& tab);
    
    virtual ~PSG();
    
    /// vector of tonalities (row-index) associated to this table.
    inline const TonIndex& index() const { return _index; }
    
    /// number of rows in this table, i.e. nb of tons considered for spelling.
    inline size_t rowNb() const { return _index.size(); }
    
    /// number of columns in this table, i.e. nb of measures spelled.
    size_t size() const;
    
    /// number of columns (PS Vectors) in this table, i.e. nb of measures spelled.
    inline size_t columnNb() const { return size(); }
    
    /// one column of this table (corresponding to a measure).
    const std::vector<size_t>& column(size_t i) const;
    
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
    
private: // data
    
    /// header of rows: array of tonalities (1 per row).
    const TonIndex& _index;
    
    /// columnns: one vector of index of local tons per measure.
    /// - the dimention of inner vectors (columns) is the size of TonIndex.
    ///   @see rowNb()
    /// - the dimention of outer vectors (rows) is the number of measures.
    ///   @see columnNb()
    std::vector<std::vector<size_t>> _content;
    
private:
    
    /// fill this table of local tons.
    /// @param flag whether the local estimation is done with rank means.
    void init(const PST& tab, std::vector<bool> mask, bool flag=false);

    // add one column
    // void init(const PSV& vec, std::vector<bool> mask);
    
    /// fill the given set with the indexes of tonalities
    /// having a minimal cost in the bags of the given PSV
    /// (there can be several ties).
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ties vector initialy empty. will contain the index of tonalities
    /// with best cost in vec.
    /// @param d maximal ton distance for selection.
    /// @warning this function determines the best local tonalities
    /// according to the bags of the studied measure;
    /// However it has an important flaw : no tonal context is taken into account
    /// to determine the local tones.
    void extractBests(const PSV& vec, std::vector<size_t>& ties, double d=0);
    
    /// select a local tonality for a measure,
    /// by computing the mean of ranks of tonalities
    /// - by cost of best spellings (for this bar)
    /// - by distance to previous local iprev
    /// - by distance to global ig
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ties vector initialy empty. will contain the index of tie
    /// candidates best ton in vec.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    void extractRank(const PSV& vec, std::vector<size_t>& ties,
                     size_t ig, size_t iprev);
    
    /// select in the given set of candidates an unique index for local tonality,
    /// given an assumed global tonality and a previous local tonality.
    /// The selection criteria is the distance to previous local,
    /// then the distance to global.
    /// @param cands set of indices of tie candidate local tonalities,
    /// to be broken.
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    size_t breakTie1(std::vector<size_t>& cands, const PSV& vec,
                     size_t ig, size_t iprev);
    
    /// select in the given set of candidates an unique index for local tonality,
    /// given an assumed global tonality and a previous local tonality.
    /// @param cands set of indices of tie candidate local tonalities,
    /// to be broken.
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    size_t breakTie2(std::vector<size_t>& cands, const PSV& vec,
                     size_t ig, size_t iprev);
        
    /// select a local tonality for a measure.
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    /// @todo TBR not used, replaced by extractRank + breakTie2
    size_t estimateLocalRank(const PSV& vec, size_t ig, size_t iprev);
    
    /// select a local tonality for a measure,
    /// by the search to tons close to the previous or the global one
    /// and then choosing the one minimizing accidents.
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    /// @param d ton distance value, for selection.
    /// @todo TBR not used, replaced by extractBests + breakTie1
    size_t estimateLocalLexico(const PSV& vec, size_t ig, size_t iprev,
                               unsigned int d);
    
    /// return index of first ton in cands with given ks,
    /// or TonIndex::UNDEF if not found.
    size_t findbyKS(int ks, const std::vector<size_t>& cands);
    
    void WARN_TIE(size_t barnb, const std::vector<size_t>& ties,
                  size_t selected = TonIndex::UNDEF) const;
};

} // namespace pse

#endif /* PSGrid_hpp */

/// @}
