//
//  Spelli.hpp
//  pse
//
//  Created by Florent Jacquemard on 23/01/2025.
//
/// @addtogroup pitch
/// @{


#ifndef Spelli_hpp
#define Spelli_hpp

#include <iostream>
#include <assert.h>
#include <memory>
#include <time.h>

#include "pstrace.hpp"
#include "PSRational.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
#include "PSRawEnum.hpp"
#include "AlgoName.hpp"
#include "CostType.hpp"
#include "CostA.hpp"
#include "CostADplus.hpp"
#include "CostADlex.hpp"


namespace pse {

/// base class for pitch-spelling functionalities,
/// containing the following paramters:
/// - one array of tonalities considered for pitch spelling.
/// - one debug flag.
/// - generic cost values.
class Spelli
{
    
public: // construction
    
    /// main constructor.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    Spelli(size_t nbTons=0, bool dflag=false);
    
    /// constructor with given TonIndex.
    /// @param id a Ton Index. must be closed.
    /// @param dflag debug mode.
    /// @see TonIndex for supported values for nbTons.
    Spelli(std::shared_ptr<TonIndex> id, bool dflag=false);

    /// destructor
    virtual ~Spelli();

public: // access array of tonalities (Ton index)
    
    /// array of tonalities considered for pitch spelling.
    /// @todo const ?
    TonIndex& index();
    
    /// number of tonalities considered for pitch spelling.
    /// It is the size of array of tonalities.
    size_t nbTons() const;

    /// tonality (for pitch spelling) of given index.
    /// @param i an index in the array of tonalities.
    /// must be smaller than nbtons().
    /// @see nbTons()
    const Ton& ton(size_t i) const;

public: // construct array of tonalities (Ton index)

    /// empty the array of tonalities considered for pitch-spelling vectors,
    /// and rebuild it with the given default number of tonalities.
    /// The array is unclosed.
    /// @param n number of default list of tonalities.
    /// @see constructor TonIndex
    /// @see addTon
    void resetTons(size_t n = 0);
    
    /// add one tonality to the array of tonalities considered for
    /// pitch-spelling vectors.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of the tonality added.
    /// @param global whether ton can be considered as a global tonality.
    /// @see Ton
    /// @warning with n=0, close() must be called aterwards.
    void addTon(int ks, ModeName mode = ModeName::Major, bool global=true);

    /// add a tonality for pitch spelling.
    /// @param ton the tonality to add to this speller.
    /// @param global whether ton can be considered as a global tonality.
    void addTon(const Ton& ton, bool global=true);
    
    /// for Python binder, same as addTon (overloaded).
    inline void addTon3(int ks, ModeName mode, bool global)
    { addTon(ks, mode, global); }
    
    /// close the array of tonalities and finish its initlialization.
    /// No ton can be added after closure.
    void closeTons();
    
    /// the array of tonalities is closed.
    bool closedTons() const;

    // switch the array of tonalities to tonal mode
    // for the conmputation of Weber distance at closing.
    // @warning the array of tonalities array must not be closed.
    // @todo TBR obsolete
    // void WeberTonal();

    // switch the array of tonalities to tonal mode
    // for the conmputation of Weber distance at closing.
    // @warning the array of tonalities array must not be closed.
    // @todo TBR obsolete
    // void WeberModal();

public: // results feedback : global ton estimation

    /// number of candidates estimated global tonality (ties).
    /// @return the size of _global or 0 if evalGlobal was not called.
    virtual size_t globals() const;
    
    /// n-best candidate global tonality.
    /// @param n number of candidate estimated global tonality,
    /// must be in 0..globals().
    /// @return the n-best estimated global tonality,
    /// or an undef ton if there is none.
    /// @warning evalGlobals must have been called.
    virtual const Ton& global(size_t n = 0) const;
    
    /// index of the n-best candidate global tonality.
    /// @param n number of candidate global tonality,
    /// must be in 0..globals().
    /// @return the index of the n-best global tonality
    /// in the index of tons, in 0..index.size(),
    /// or TonIndex::UNDEF in case of error.
    /// @warning spell() must have been called.
    virtual size_t iglobal(size_t n = 0) const;

public: // debug flag

        /// set debug mode (log messages for debugging)
        void debug(bool flag);

protected: // data
        
    /// array of tonalities that shall be considered for pitch spelling.
    std::shared_ptr<TonIndex> _index;
    
    /// @warning we compute one sample cost for cost type.

    // sample cost (zero) for the construction of tables.
    // @warning discount Obsolete for CostA
    // CostA _seedAdiscount;

    /// sample cost (zero) for the construction of tables.
    CostA _seedAnodiscount;

    /// sample cost (zero) for the construction of tables.
    CostADplus _seedADplus;

    /// sample cost (zero) for the construction of tables.
    CostADlex _seedADlex;

    /// debug mode activated.
    bool _debug;
    
    /// undefined tonality, for errors.
    Ton* _uton; // std::shared_ptr<Ton>
        
protected:
    
    /// construct a sample cost value (zero) for the construction of tables.
    /// @param ct type of cost domain.
    /// @return the cost value constructed.
    Cost& sampleCost(CostType ct);

    /// @param c code for cost type:
    /// - 00 costA with discount for lead tons
    /// - 01 costA without discount for lead tons
    /// - 02 costADplus
    /// - 03 costADlex
};
} // namespace pse

#endif /* Spelli_hpp */

/// @}
