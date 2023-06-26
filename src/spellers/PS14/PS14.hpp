//
//  PS14.hpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef PS14_hpp
#define PS14_hpp

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
#include "Speller2pass.hpp"
#include "PSTable.hpp"
#include "PSGlobal.hpp"
#include "PSGrid.hpp"

namespace pse {


/// specific structure for the PS14 algorithm.
class PS14 : public Speller2Pass
{
public:
    
    /// main constructor. initially empty list of notes to spell.
    /// @param nbTons use default list of tonalities (default: empty).
    /// @param dflag debug mode.
    /// @see PSTable
    PS14(size_t nbTons=0, bool dflag=true);
    
    /// destructor
    virtual ~PS14();
    
    // name of algorithm implemented
    // @return Algo::PS14 for this class.
    // virtual Algo algo() const override;
    
    /// compute the best pitch spelling for the input notes.
    /// @return whether computation was succesfull.
    bool spell() override;
    
    // Estimation of tonalities
        
    /// @return distance in the array of fifths between
    /// from estimated global tonality and
    /// a signature with no accidentals.
    inline int fifths(size_t n=0) const { return global(n).fifths(); }

};

} // namespace pse

#endif /* PS14_hpp */

/// @}
