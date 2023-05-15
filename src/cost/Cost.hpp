//
//  Cost.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
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

/// Abstract class for cost model used for for the ordering of configurations
/// in the the PS algorithm.
// see Note Spelling Conventions in Behind Bars (page 85)
struct PSCost
{
public:
    
    /// assignement operator
    PSCost& operator=(const PSCost& rhs);
    
    
    
    
    
};



} // namespace pse

#endif /* Cost_hpp */

/// @}
