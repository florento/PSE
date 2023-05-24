//
//  Cost.hpp
//  pse
//
//  Created by Florent on 16/05/2023.
//
/// @addtogroup pitch
/// @{

#ifndef Cost_hpp
#define Cost_hpp

#include <iostream>
#include <assert.h>

#include "trace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"

namespace pse {

class PSC1;

class PSC2;


/// abstract class defining the cost update interface.
class Cost
{
public:

    /// update this cost for doing a transition into the given config,
    /// from its previous config, in a given hypothetic global tonality.
    /// @see PSC.previous()
    /// @param gton conjectured main (global) tonality (key signature).
    virtual void update(const PSC1& c, const PSEnum& e,
                        const Ton& gton) = 0;

    /// update this cost for doing a transition into the given config,
    /// from its previous config,
    /// in given hypothetic global and local tonalities.
    /// @see PSC.previous()
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    virtual void update(const PSC1& c, const PSEnum& e,
                        const Ton& gton, const Ton& lton) = 0;
    
    /// update this cost for doing a transition into the given chord config,
    /// in a given hypothetic global tonality.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param gton conjectured main (global) tonality (key signature).
    virtual void update(const PSC2& c, const PSEnum& e,
                        const enum NoteName& name, const enum Accid& accid,
                        bool print, size_t nbocc,
                        const Ton& gton) = 0;
    
    /// update this cost for doing a transition into the given chord config,
    /// in given hypothetic global and local tonalities.
    /// @param name chosen name for the received pitch, in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid chosen alteration for the received pitch, in -2..2.
    /// @param print whether the accidental must be printed in score.
    /// @param nbocc nb of occurrence of the note (pitch class) in this chord.
    /// @param gton conjectured main (global) tonality (key signature).
    /// @param lton conjectured local tonality.
    virtual void update(const PSC2& c, const PSEnum& e,
                        const enum NoteName& name, const enum Accid& accid,
                        bool print, size_t nbocc,
                        const Ton& gton, const Ton& lton) = 0;
    
};

} // namespace pse

#endif /* Cost_hpp */

/// @}
