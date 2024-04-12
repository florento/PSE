//
//  CostType.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/04/2024.
//
/// @addtogroup pse
/// @{

#ifndef CostType_hpp
#define CostType_hpp

#include <iostream>
#include <string>

#include "trace.hpp"



namespace pse {


/// types of cost domains.
enum class CostType
{
    /// Unknown
    UNDEF,
    
    /// number of accidentals without discount for lead tons
    ACCID,

    /// number of accidentals with discount for lead tons
    ACCIDlead,

    /// number of accidentals and distance to local ton, sum of values.
    ADplus,

    /// number of accidentals and distance to local ton, lexicographic comparison values.
    ADlex

};


/// string of cost type.
std::string tostring(const CostType& m);

std::ostream& operator<<(std::ostream& o, const CostType& m);


} // end namespace pse

#endif /* CostType_hpp */

/// @} // end group pse

