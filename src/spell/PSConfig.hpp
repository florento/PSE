//
//  PSConfig.hpp
//  qparse
//
//  Created by Florent Jacquemard on 14/03/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PSConfig_hpp
#define PSConfig_hpp


#include <iostream>
#include <assert.h>
#include <array>
#include <vector>

#include "trace.hpp"
#include "Accidental.hpp"
#include "PSConfig0.hpp"
#include "PSEnum.hpp"


namespace pse {


/// abstract class for extending PSConfig0 as target of transition.
/// Configurations of descendants of this class are always non-initial
/// in a best path solution for pitch spelling.
class PSC : public PSC0
{
    
public:

    /// target PS config for a transition from given (previous) config.
    /// @param c previous config (origin), to be updated with the transition.
    PSC(const PSC0* c);

    /// copy constructor
    PSC(const PSC& c);
    
    virtual ~PSC();
    
    /// assignement operator
    PSC& operator=(const PSC& rhs);

    /// configs have the same list of accidentals
    bool operator==(const PSC& rhs) const;
    
    /// configs have different list of accidentals
    bool operator!=(const PSC& rhs) const;
    
    /// always false: PSC are non-initial configurations in a best path.
    virtual bool initial() const;
    
    /// previous config in shortest path to this config,
    /// or null if there are none.
    /// every config has at most one predecessor.
    virtual const PSC0* previous() const;
    
    // update the cumulated distance for this Config,
    // with the distance (in the array of fifths) to the given tonality.
    // @param ton a tonality.
    // @todo obsolete, TBR
    // void updateDist(const Ton& ton);
    // void succ(PSEnum& _enum, const Ton& ton, PSCQueue& q) const;
    // void succ(PSEnum& _enum, const Ton& ton, const Ton& lton,
    //           PSCQueue& q) const;
    
protected:
        
    /// previous config in shortest path to this config.
    const PSC0* _pred;
    //std::shared_ptr<const PSC0> _pred;
    
};


} // namespace pse

#endif /* PSConfig_hpp */

/// @}
