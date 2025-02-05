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

#include "pstrace.hpp"



namespace pse {


/// types of cost domains.
enum class CostType
{
    /// Unknown
    UNDEF,
    
    /// number of accidentals, without discount for lead tons
    ACCID,

    /// number of accidentals and tie breaking measures
    /// - number of accidentals not present in the chromatic harmonic scale
    /// - cumulated number of accidentals with color different from global ton
    /// - cumulated number of printed and non lead Cb B# E# Fb.
    ACCIDtb,

    /// number of accidentals with discount for lead tons
    ACCIDlead,

    /// number of accidentals and distance to local ton, sum of values.
    ADplus,

    /// number of accidentals and distance to local ton,
    /// lexicographic comparison values.
    ADlex

};

/// string of cost type.
std::string tostring(const CostType& m);

std::ostream& operator<<(std::ostream& o, const CostType& m);

} // end namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostType> : fmt::ostream_formatter {};

#endif /* CostType_hpp */

/// @} // end group pse

