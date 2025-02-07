//
//  PSGridr.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/01/2025.
//
/// @addtogroup pitch
/// @{

#ifndef PSGridr_hpp
#define PSGridr_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <vector>
#include <set>

#include "pstrace.hpp"
#include "utils.hpp"
#include "PSGridy.hpp"

namespace pse {

/// Construction of a grid from a table with a greedy algorithm
/// selecting of the ton in each cell according to the spelling costs in table.
class PSGr : public PSGy
{
public: // construction

    /// construction of grid from a table.
    /// @param tab pitch spelling table used to estimated the locals.
    /// @todo flag mask
    /// @todo flak atonal
    PSGr(const PST& tab);
    
    virtual ~PSGr();
    
private: // construction
    
    /// coefficients for the compution of means of ranks.
    /// 0. rank in column of spelling table.
    /// 1. rank for distance to previous in row.
    /// 2. rank for distance to global in row.
    static const std::array<size_t, 3> COEFF;
    
    /// fill this table of local tons.
    // @param flag whether the local estimation is done with rank means.
    void init(const PST& tab); 
    
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

    /// return index of first ton in cands with given ks,
    /// or TonIndex::UNDEF if not found.
    size_t findbyKS(int ks, const std::vector<size_t>& cands);

    /// select in the given set of candidates an unique index for local tonality,
    /// given an assumed global tonality and a previous local tonality.
    /// @param cands set of indices of tie candidate local tonalities,
    /// to be broken.
    /// @param vec vector of best spelling, one for each possible local tonality.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    size_t breakTieRank(const PSV& vec, const std::vector<size_t>& cands,
                     size_t ig, size_t iprev);

    // OLD VERSION
    // select a local tonality for a measure.
    // @param vec vector of best spelling, one for each possible local tonality.
    // @param ig index of assumed global tonality.
    // @param iprev index of estimated local tonality for previous measure.
    // @todo TBR not used, replaced by extractRank + breakTie2
    // size_t estimateLocalRank(const PSV& vec, size_t ig, size_t iprev);
        
};

} // namespace pse

#endif /* PSGridr_hpp */

/// @}
