//
//  PSE.hpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PSE_hpp
#define PSE_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
#include "Cost.hpp"
#include "PSRawEnum.hpp"
#include "Speller.hpp"
#include "PSTable.hpp"
#include "PSGlobal.hpp"
#include "PSGrid.hpp"


namespace pse {


/// specific structure for the PSE algorithm.
class PSE : public Speller
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see PSTable
    PSE(size_t nbTons=0, bool dflag=true);
    
    /// destructor
    virtual ~PSE();
    
    // name of algorithm implemented
    // @return Algo::PSE for this class.
    // virtual Algo algo() const override;
    
    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    bool spell() override;
    
    // Estimation of tonalities
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    void setGlobal(size_t i);
    
    /// number of candidates (ties) for the estimatation of the global tonality.
    size_t globals() const;
    
    /// candidate global tonality for this table.
    /// @param i candidate number, must be in 0..globalCands().
    const Ton& globalCand(size_t i) const;
    
    /// index of a candidate global tonality for this table, in 0..index.size().
    /// @param i candidate number, must be in 0..globalCands().
    /// @return the index of the global tonality candidate i,
    /// in the index of tons, in 0..index.size().
    size_t iglobalCand(size_t i) const;
    
    /// index of the estimated global tonality.
    /// @return the index of the estimated global tonality in the index of tons,
    /// in 0..index.size().
    size_t iglobal() const { return iglobalCand(0); };
    
    /// estimated global tonality.
    /// @warning spell() must have been called.
    const Ton& global() const override { return globalCand(0); }
    
    /// @return distance in the array of fifths between
    /// from estimated global tonality and
    /// a signature with no accidentals.
    inline int fifths() const { return global().fifths(); }
    
    /// estimated local tonality for one bar.
    /// @param j bar number.
    /// @warning spell() must have been called.
    const Ton& local(size_t j) const;
    
    /// estimated local tonality at note of given index.
    /// @param i index of note in the enumerator of input notes.
    /// @warning spell() must have been called.
    const Ton& localNote(size_t i) const;
    
    
private: // data
    
    /// First Pitch Spelling table.
    std::unique_ptr<PST> _table0;
    
    /// First estimation of global tonality (on table0).
    std::unique_ptr<PSO> _global0;
    
    /// Grid of loval tonalities (estimated on table0).
    std::unique_ptr<PSG> _locals0;
    
    /// Second Pitch Spelling table.
    std::unique_ptr<PST> _table1;
    
    /// Second estimation of global tonality (on table1).
    std::unique_ptr<PSO> _global1;
    
    // forced global ton
    // std::unique_ptr<size_t> _global;
   
    /// estimated local tonality for one candidate global tonality and one bar.
    /// @param i index of candidate global tonality.
    /// @param j bar number.
    const Ton& localCandBar(size_t i, size_t j) const;

};


} // namespace pse

#endif /* PSE_hpp */

/// @}
