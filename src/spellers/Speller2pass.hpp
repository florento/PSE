//
//  Speller2pass.hpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef Speller2pass_hpp
#define Speller2pass_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "Speller1pass.hpp"

namespace pse {

/// speller in 2 passes, with 2 tables.
class Speller2Pass : public Speller1Pass
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see PSTable
    Speller2Pass(const Algo& algo=Algo::Undef, size_t nbTons=0,
                 bool dflag=true);
    
    /// destructor
    virtual ~Speller2Pass();
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    void setGlobal(size_t i) override;
    
    /// number of candidates (ties) for the estimatation of the global tonality.
    size_t globals() const override;
    
    /// candidate global tonality for this table.
    /// @param i candidate number, must be in 0..globalCands().
    const Ton& globalCand(size_t i) const override;
    
    /// index of a candidate global tonality for this table, in 0..index.size().
    /// @param i candidate number, must be in 0..globalCands().
    /// @return the index of the global tonality candidate i,
    /// in the index of tons, in 0..index.size().
    size_t iglobalCand(size_t i) const override;
    

protected:
    
    /// Second Pitch Spelling table.
    std::shared_ptr<PST> _table1;
    
    /// Second estimation of global tonality (on table1).
    std::shared_ptr<PSO> _global1;

};



} // namespace pse

#endif /* Speller2pass_hpp */

/// @}
