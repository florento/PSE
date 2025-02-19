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

    /// number of accidentals, with equality to one near.
    ACCIDa,

    /// number of accidentals and tie breaking measures:
    /// - number of accidentals not present in the chromatic harmonic scale
    /// - number of accidentals with color different from global ton
    /// - number of printed and non lead Cb B# E# Fb.
    ACCIDtb,

    /// number of accidentals and tie breaking measures,
    /// equality is number of accidentals to one near
    /// and component-wise TB equality.
    ACCIDtba,

    /// number of accidentals and tie breaking measures,
    /// equality is equality of number of accidentals
    /// and tb equality where some TB components are summed.
    ACCIDtbs,

    /// number of accidentals and tie breaking measures,
    /// equality is number of accidentals to one near
    /// and tb equality where some TB components are summed.
    ACCIDtbas,

    /// number of accidentals with discount for lead tons.
    /// obsolete.
    ACCIDlead,

    /// number of accidentals and distance to local ton, sum of values.
    ADplus,

    /// number of accidentals and distance to local ton, sum of values.
    /// equality is equality of sum to one near
    /// and component-wise TB equality.
    ADplusa,

    /// number of accidentals and distance to local ton, sum of values.
    /// equality is equality of sum
    /// and tb equality where some TB components are summed.
    ADpluss,

    /// number of accidentals and distance to local ton, sum of values.
    /// equality is equality of sum to one near
    /// and tb equality where some TB components are summed.
    ADplusas,

    /// number of accidentals and distance to local ton compared lexicographicaly.
    ADlex,

    /// number of accidentals and distance to local ton compared lexicographicaly.
    ADlexa,

    /// number of accidentals and distance to local ton compared lexicographicaly.
    ADlexs,

    /// number of accidentals and distance to local ton compared lexicographicaly.
    ADlexas

};

class Cost;

/// zero cost of given type.
std::unique_ptr<Cost> unique_zero(const CostType& ct);

/// string of cost type.
std::string tostring(const CostType& ct);

std::ostream& operator<<(std::ostream& o, const CostType& m);

} // end namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::CostType> : fmt::ostream_formatter {};

#endif /* CostType_hpp */

/// @} // end group pse

