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

#include "pstrace.hpp"
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
    Speller2Pass( size_t nbTons=0,
                 const Algo& algo=Algo::Undef, // TBR
                 bool dflag=true);
    
    /// destructor
    virtual ~Speller2Pass();
    
    /// rename all notes read by this speller,
    /// according to a given global tonality.
    /// @param n number of candidate estimated global tonality.
    /// must be in 0..globals().
    /// @return whether renaming succeded for all measures.
    bool rename(size_t n=0) override;

    /// rename all notes read by this speller, according to a given global tonality.
    /// @param n number of candidate estimated global tonality, in the
    /// first table of global candidates.
    /// @return whether renaming succeded for all measures.
    bool rename0(size_t n=0);
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    void setGlobal(size_t i) override;
    
    /// number of candidates (ties) for the estimation of the global tonality.
    size_t globals() const override;
    
    /// index of the n-best estimated global tonality.
    /// @param n number of candidate estimated global tonality,
    /// must be in 0..globals().
    /// @return the index of the n-best estimated global tonality
    /// in the index of tons, in 0..index.size()
    /// or TonIndex::UNDEF in case of error.
    /// @warning spell() must have been called.
    size_t iglobal(size_t n=0) const override;
    
    /// n-best estimated global tonality.
    /// @param n number of candidate estimated global tonality,
    /// must be in 0..globals().
    /// @return the n-best estimated global tonality.
    /// It is ton(iglobal(n)) or an undef ton in case of error.
    /// @warning spell() must have been called.
    const Ton& global(size_t n=0) const override;
    
    /// number of candidates (ties) for the estimation of the global tonality,
    /// after first pass.
    size_t globals0() const;
    
    const Ton& global0(size_t n=0) const;
    
    /// index of the n-best estimated global tonality,
    /// after first pass.
    /// @param n number of candidate estimated global tonality,
    /// must be in 0..globals().
    /// @return the index of the n-best estimated global tonality
    /// in the index of tons, in 0..index.size()
    /// or TonIndex::UNDEF in case of error.
    /// @warning spell() must have been called.
    size_t iglobal0(size_t n=0) const;
    
protected: // data
    
    /// Second Pitch Spelling table.
    PST* _table1; // std::shared_ptr<PST>
    
    /// Second estimation of global tonality (on table1).
    PSO* _global1; // std::shared_ptr<PSO>
    
    /// Time to build the second Pitch Spelling table.
    double _time_table1;

protected:
    
    /// compute the best pitch spelling for the input notes,
    /// using the algorithm named in this class.
    /// @param seed0 seed cost used to built the first PS table
    /// @param diff0 approximation coeff (percent) for the first estimatation of
    /// global ton(s). 100 for keeping all ton of index as candidates.
    // @param rewrite_flag0 whether the passing note must be rewritten
    // after first pass.
    /// @param seed1 seed cost used to built the second PS table
    /// @param diff1 approximation coeff (percent) for the second estimatation of
    /// global ton(s).
    /// @param rewrite_flag1 wether the passing note must be rewritten
    /// after second pass.
    /// @see PSEnum::rewritePassing()
    /// @return whether computation was succesfull.
    bool spell(const Cost& seed0, const Cost& seed1,
               double diff0=0, // bool rewrite_flag0=false,
               double diff1=0,
               bool rename_flag1=false,
               bool rewrite_flag1=false);

};



} // namespace pse

#endif /* Speller2pass_hpp */

/// @}
