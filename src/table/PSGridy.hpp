//
//  PSGridy.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/01/2025.
//
/// @addtogroup pitch
/// @{

#ifndef PSGridy_hpp
#define PSGridy_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <vector>
#include <set>

#include "pstrace.hpp"
#include "utils.hpp"
#include "TonIndex.hpp"
#include "Cost.hpp"
#include "PSTable.hpp"
#include "PSVector.hpp"
#include "PSBag.hpp"
#include "PSGrid.hpp"

namespace pse {

/// Construction of a grid from a table with a greedy algorithm
/// selecting of the ton in each cell according to the spelling costs in table.
class PSGy : public PSG
{
    
public: // construction

    /// construction of grid from a table.
    /// @param tab pitch spelling table used to estimated the locals.
    /// @todo flag mask
    /// @todo flak atonal
    PSGy(const PST& tab);
    
    virtual ~PSGy();
    
private: // construction
    
    /// fill this grid of local tons.
    void init(const PST& tab); // std::vector<bool> mask
    
    /// fill the given set with the indexes of tonalities
    /// having a minimal cost in the bags of the given PSV
    /// (there can be several ties).
    /// @param vec vector of best spelling, one for each possible
    /// local tonality.
    /// @param ties vector initialy empty. will contain the index of tonalities
    /// with best cost in vec.
    /// @param d maximal ton distance for selection.
    /// @warning this function determines the best local tonalities
    /// according to the bags of the studied measure;
    /// However it has an important flaw : no tonal context is taken into
    /// account
    /// to determine the local tones.
    void extractBests(const PSV& vec, std::vector<size_t>& ties, double d=0);
    
    // OLD VERSION
    // select a local tonality for a measure,
    // by the search to tons close to the previous or the global one
    // and then choosing the one minimizing accidents.
    // @param vec vector of best spelling, one for each possible local tonality.
    // @param ig index of assumed global tonality.
    // @param iprev index of estimated local tonality for previous measure.
    // @param d ton distance value, for selection.
    // @todo TBR not used, replaced by extractBests + breakTie1
    // size_t estimateLocalLexico(const PSV& vec, size_t ig, size_t iprev,
    //                            unsigned int d);

protected: // construction
    
    /// select in the given set of candidates an unique index for local
    /// tonality,
    /// given an assumed global tonality and a previous local tonality.
    /// The selection criteria is the distance to previous local,
    /// then the distance to global.
    /// @param cands set of indices of tie candidate local tonalities,
    /// to be broken.
    /// @param vec vector of best spelling, one for each possible local
    /// tonality.
    /// @param ig index of assumed global tonality.
    /// @param iprev index of estimated local tonality for previous measure.
    size_t breakTieBest(const PSV& vec, const std::vector<size_t>& cands,
                        size_t ig, size_t iprev);
    
    void WARN_TIE(size_t barnb, const std::vector<size_t>& ties,
                  size_t selected = TonIndex::UNDEF) const;
    
};

} // namespace pse

#endif /* PSGridy_hpp */

/// @}
