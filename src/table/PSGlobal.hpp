//
//  PSGlobal.hpp
//  pse
//
//  Created by Florent Jacquemard on 25/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PSGlobal_hpp
#define PSGlobal_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <vector>
#include <set>

#include "trace.hpp"
#include "AlgoName.hpp"
#include "PSEnum.hpp"
#include "TonIndex.hpp"
//#include "PSCost.hpp"
#include "Cost.hpp"
//#include "PSTable.hpp"

namespace pse {

// [ ] centraliser _index dans Speller

class PST;

/// Structure for the estimation from a table
/// of a glabal tonality, or several candidates for glabal tonality.
class PSO
{
    
public:
    
    /// Empty or full store of candidate global tonalities in a given index.
    /// @param id array of possible global tonalities.
    /// @param dflag debug mode (for feeback)
    /// @param full flag, whether the constructed set of condidates is
    /// full (all tonalities of id) or empty.
    PSO(const TonIndex& id, double dflag = false, double full = false);

    /// Estimate and store the global tonality candidates for the given table.
    /// @param tab for the extraction of global tonality.
    /// The index of tonalities stored refer to the ton index of this table.
    /// @param d tolerance distance (in percent) for considering two
    /// cost approx. equal in the global ton search.
    /// @param dflag debug mode (for feeback)
    // @warning call eGlobals_eq and eGlobals_less variants to operator==
    // and operator< on cost.
    PSO(const PST& tab, double d = 0, bool dflag=false); // const TonIndex& i
    
    /// Refine a given store of global tonality candidates, given a new table.
    /// @param globals former list of global tonality conditates.
    /// @param tab table for the extraction of new global tonality condidates,
    /// amongsts the candidates of globals.
    /// Must refere to the same index of tonalities as globals.
    /// @param d tolerance distance (in percent) for considering two
    /// cost approx. equal in the global ton search.
    /// @param dflag debug mode (for feeback)
    PSO(const PSO& globals, const PST& tab, double d = 0, bool dflag=false);
    
    virtual ~PSO();
    
    // vector of tonalities used for pitch spelling.
    // inline const TonIndex& index() const { return _index; }
   
    // a set of candidate global tonalities is known.
    // estimateGlobals or setGlobal was called.
    // bool estimatedGlobals() const;
       
    /// number of candidates (ties) for the estimatation of the global tonality.
    /// @return 0 iff the estimation failed.
    size_t size() const;

    /// there are no candidates for the estimatation of the global tonality.
    bool empty() const;

    /// the given index of global tonality is one of the current candidates.
    bool contains(size_t i) const;
    
    /// vector of tonalities associated to the estimated global tonalities
    /// contained in this structure.
    /// it is the row-index of the table for the estimation
    inline const TonIndex& index() const { return _index; }

    /// estimated candidate global tonality.
    /// @param i candidate number, must be in 0..globalNb().
    const Ton& global(size_t i = 0) const;

    /// index of an estimated candidate global tonality.
    /// @param i candidate number, must be in 0..globalNb().
    /// @return index in in 0..index.size() of the ith global candidate.
    // @warning iglobal(0) = TonIndex::FAILED if the evaluation failed.
    size_t iglobal(size_t i = 0) const;
    
    /// Enharmonic tonality of the candidate of the given index.
    /// It is added to this list of candidates if necessary.
    /// @param i a candidate number, must be in 0..globalNb().
    /// @return the following index:
    /// - i if the candidate i has no enharmonics,
    /// - j != i if the candidate j is an enharmonic of the candidate i.
    ///   If the enharmonics of i is present in this candidate list,
    ///   then it is added.
    /// - UNDEF if if the candidate i has an enharmonic that is not
    ///   present in the index of tonalities considered.
    size_t enharmonic(size_t i);
    
    /// Add all enharmonic tonalities of candidates in this list,
    /// if they are not present.
    /// @return whether the completion was successful.
    bool completeEnharmonics();
    
    /// iterator pointing to the first index of an estimated candidate
    /// global tonality.
    std::vector<size_t>::const_iterator cbegin() const;
    
    /// iterator pointing to the past-the-end index of an estimated candidate
    /// global tonality.
    std::vector<size_t>::const_iterator cend() const;

    /// force a global tonality.
    /// @param ig index of global tonality in ton index.
    /// It must be in the index of tonalities used to create this structure
    /// (i.e. in 0..globalNb()).
    /// The given tonality index ig is added at the end of candidate list.
    void setGlobal(size_t ig);

    /// mask corresponding to the global tonality candidates in this structure.
    std::vector<bool> getMask() const;
    
    // empty the set of candidate global tonality.
    // void clear();
    
    // estimate the best global tonality for this table
    // (second step, after estimation local tonalities).
    // @return whether the estimation of the global tonality successed.
    // @warning estimLocals() must have been called.
    // bool estimateGlobal();
        
    // estimate the global tonality candidate for the given table.
    // The current list of global tonality candidates is cleared.
    // @param tab for the extraction of global tonality.
    // The index of tonalities stored refer to the ton index of this table.
    // @param d tolerance distance (in percent) for considering two
    // cost approx. equal in the global ton search.
    // @return whether the estimation of the global tonality successed.
    // @warning call eGlobals_eq and eGlobals_less variants to operator==
    // and operator<  on cost.
    // bool estimateGlobals(const PST& tab, double d=0);
    
    // global tonality is known.
    // estimateGlobal or setGlobal was called
    // bool estimatedGlobal() const;
    
    void print(std::ostream& o) const;
    
private:
    
    // header of rows: array of tonalities (1 per row).
    const TonIndex& _index;
    
    /// index of candidates for global tonality.
    /// empty if the estimation failed.
    // it contains one element TonIndex::FAILED.
    std::vector<size_t> _globals;
    
    // index of estimated global tonality.
    // - TonIndex::UNDEF if it was not estimated yet.
    // - TonIndex::FAILED if its estimation failed.
    // - an integer value between 0 and index.size() otherwise.
    // suoor. redundant / _globals
    // size_t _global;
  
    // the list of global candidates has been estimated,
    // i.e. estimateGlobals() was called.
    // bool _estimated_globals;
    
    /// debug mode.
    bool _debug;
     
private:
    
    void init(const PSO& globals, const PST& tab, double d);
    
    bool member(size_t ig) const;

    size_t find(size_t ig) const;

    // macro: cost equality for the estimation of global.
    // use if Costt.operator== is eq_lex.
    // @warning static choice
    // bool eGlobals_eq_lex(const PSCost& lhs, const PSCost& rhs) const;

    // macro: cost equality for the estimation of global.
    // use if Costt.operator== is eq_cumul.
    // @warning static choice
    // inline bool eGlobals_eq_cumul(const PSCost& lhs, const PSCost& rhs) const;
    
};


std::ostream& operator<<(std::ostream& o, const PSO& globals);



} // namespace pse

#endif /* PSGlobal_hpp */

/// @}
