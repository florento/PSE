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
       
    /// rename all notes read by this speller,
    /// according to a given global tonality.
    /// @param n number of candidate estimated global tonality.
    /// must be in 0..globals().
    /// @return whether renaming succeded for all measures.
    virtual bool rename(size_t n=0) override;

    /// index (in the TonIndex) of the estimated local tonality
    /// for one assumed global tonality and one bar.
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j measure number.
    /// @return the index of the estimated local tonality,
    /// assuming the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    /// @warning spell() must have been called.
    size_t ilocal(size_t i, size_t j) const;

    /// estimated local tonality for one assumed global tonality and one bar.
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j measure number.
    /// @return the estimated local tonality assuming the global tonality i:
    /// - TonIndex::UNDEF if it was not estimated yet.
    /// - TonIndex::FAILED if its estimation failed.
    /// - an integer value between 0 and index.size() otherwise.
    /// @warning spell() must have been called.
    const Ton& local(size_t i, size_t j) const;
    
    /// estimated local tonality at note of given index, for one given
    /// assumed global tonality.
    /// @param i index in the TonIndex of an assumed global tonality.
    /// @param j index of note in the enumerator of input notes.
    /// @warning spell() must have been called.
    const Ton& localNote(size_t i, size_t j) const;
    
    /// force global tonality. it wont be estimated.
    /// @param i index of tonality set as global.
    virtual void setGlobal(size_t i);
    
    /// number of candidates (ties) for the estimatation of the global tonality.
    virtual size_t globals() const override;
    
    // candidate global tonality for this table.
    // @param i candidate number, must be in 0..globalCands().
    // virtual const Ton& globalCand(size_t i) const;
    
    // index of a candidate global tonality for this table, in 0..index.size().
    // @param i candidate number, must be in 0..globalCands().
    // @return the index of the global tonality candidate i,
    // in the index of tons, in 0..index.size().
    //virtual size_t iglobalCand(size_t i) const;
    
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
    
    // estimated local tonality for one candidate global tonality and one bar.
    // @param i index of candidate global tonality.
    // @param j bar number.
    // const Ton& localCandBar(size_t i, size_t j) const;
   
    void setGlobal(size_t i, PSO* g); // std::shared_ptr<PSO>
    
    /// number of global cands in g
    size_t globalCands(const PSO* g) const; // std::shared_ptr<PSO>
    
    /// global cand number i in g
    const Ton& globalCand(size_t i, const PSO* g) const; // std::shared_ptr<PSO>
    
    /// index of global cand number i in g
    size_t iglobalCand(size_t i, const PSO* g) const; // std::shared_ptr<PSO>

    /// compute the best pitch spelling for the input notes,
    /// using the algorithm named in this class.
    /// @param seed0 seed cost used to built the PS table
    /// @param diff0 approximation coeff (percent) to estimate the global ton(s).
    /// @param rename_flag whether the notes in enumerator must be renamed.
    /// @param rewrite_flag whether the passing note must be rewritten.
    /// @see PSEnum::rewritePassing()
    /// @return whether computation was succesfull.
    bool spell(const Cost& seed0, double diff0=0,
               bool rename_flag=false, bool rewrite_flag=false);
    
    /// rename all notes read by this speller,
    /// according to a given global tonality.
    /// @param table table for renaming.
    /// @param globals list of global tonality candidates.
    /// @param n number of candidate estimated global tonality, in g.
    /// @return whether renaming succeded for all measures.
    bool rename(PST* table, const PSO* globals, size_t n=0);
    
    
    
};



} // namespace pse

#endif /* Speller1pass_hpp */

/// @}
