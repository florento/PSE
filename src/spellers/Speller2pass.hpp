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
    

protected: // data
    
    /// Second Pitch Spelling table.
    PST* _table1; // std::shared_ptr<PST>
    
    /// Second estimation of global tonality (on table1).
    PSO* _global1; // std::shared_ptr<PSO>

protected:
    
    /// compute the best pitch spelling for the input notes,
    /// using the algorithm named in this class.
    /// @param seed0 seed cost used to built the first PS table
    /// @param diff0 approximation coeff (percent) for the first estimatation of
    /// global ton(s).
    /// @param rewrite_flag0 wether the passing note must be rewritten
    /// after first pass.
    /// @param seed1 seed cost used to built the second PS table
    /// @param diff1 approximation coeff (percent) for the second estimatation of
    /// global ton(s).
    /// @param rewrite_flag1 wether the passing note must be rewritten
    /// after second pass.
    /// @see PSEnum::rewritePassing()
    /// @return whether computation was succesfull.
    bool spell(const Cost& seed0, const Cost& seed1,
               double diff0=0, bool rewrite_flag0=false,
               double diff1=0, bool rewrite_flag1=false);

};



} // namespace pse

#endif /* Speller2pass_hpp */

/// @}
