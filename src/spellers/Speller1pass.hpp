//
//  Speller1pass.hpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//
/// @addtogroup pitch
/// @{


#ifndef Speller1pass_hpp
#define Speller1pass_hpp

#include <iostream>
#include <assert.h>
#include <memory>

#include "trace.hpp"
#include "PSTable.hpp"
#include "PSGlobal.hpp"
#include "PSGrid.hpp"
#include "Speller.hpp"

namespace pse {


/// speller in 1 passes, with 1 table.
class Speller1Pass : public Speller
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see PSTable
    Speller1Pass(const Algo& algo=Algo::Undef, size_t nbTons=0,
                 bool dflag=true);
    
    /// destructor
    virtual ~Speller1Pass();
    
    /// estimated local tonality for one bar.
    /// @param j bar number.
    /// @warning spell() must have been called.
    const Ton& local(size_t j) const;
    
    /// estimated local tonality at note of given index.
    /// @param i index of note in the enumerator of input notes.
    /// @warning spell() must have been called.
    const Ton& localNote(size_t i) const;
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    virtual void setGlobal(size_t i);
    
    /// number of candidates (ties) for the estimatation of the global tonality.
    virtual size_t globals() const;
    
    /// candidate global tonality for this table.
    /// @param i candidate number, must be in 0..globalCands().
    virtual const Ton& globalCand(size_t i) const;
    
    /// index of a candidate global tonality for this table, in 0..index.size().
    /// @param i candidate number, must be in 0..globalCands().
    /// @return the index of the global tonality candidate i,
    /// in the index of tons, in 0..index.size().
    virtual size_t iglobalCand(size_t i) const;
    
    /// index of the estimated global tonality.
    /// @return the index of the estimated global tonality in the index of tons,
    /// in 0..index.size().
    size_t iglobal() const { return iglobalCand(0); };
    
    /// estimated global tonality.
    /// @warning spell() must have been called.
    const Ton& global() const override { return globalCand(0); }
    
protected: // data
    
    /// First Pitch Spelling table.
    PST* _table0;  // std::shared_ptr<PST>
    
    /// First estimation of global tonality (on table0).
    PSO* _global0; // std::shared_ptr<PSO>
    
    /// Grid of loval tonalities (estimated on table0).
    PSG* _locals0; // std::shared_ptr<PSG>
    
    // forced global ton
    // std::unique_ptr<size_t> _global;
    
    /// undefined tonality, for errors.
    Ton* _uton; // std::shared_ptr<Ton>

protected:
    
    /// estimated local tonality for one candidate global tonality and one bar.
    /// @param i index of candidate global tonality.
    /// @param j bar number.
    const Ton& localCandBar(size_t i, size_t j) const;
   
    void setGlobal(size_t i, PSO* g); // std::shared_ptr<PSO>
    
    size_t globals(const PSO* g) const; // std::shared_ptr<PSO>
    
    const Ton& globalCand(size_t i, const PSO* g) const; // std::shared_ptr<PSO>
    
    size_t iglobalCand(size_t i, const PSO* g) const; // std::shared_ptr<PSO>
    
    /// compute the best pitch spelling for the input notes,
    /// using the algorithm named in this class.
    /// @return whether computation was succesfull.
    bool spell(const Cost& seed0);

};



} // namespace pse

#endif /* Speller1pass_hpp */

/// @}
