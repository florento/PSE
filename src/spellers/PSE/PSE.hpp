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

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "ModeName.hpp"
#include "Ton.hpp"
#include "TonIndex.hpp"
#include "Cost.hpp"
#include "PSRawEnum.hpp"
#include "Speller2pass.hpp"
#include "PSTable.hpp"
// #include "PSGlobal.hpp"
#include "PSGrid.hpp"


namespace pse {


/// specific structure for the PSE algorithm.
class PSE : public Speller2Pass
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
        
    /// @return distance in the array of fifths between
    /// the estimated global tonality and
    /// a signature with no accidentals.
    inline int fifths() const { return global().fifths(); }
          
};


} // namespace pse

#endif /* PSE_hpp */

/// @}
