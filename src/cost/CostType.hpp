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
    
    /// number of printed accidentals, without discount for lead tons
    ACCID,

    /// number of printed accidentals and tie breaking measures:
    /// - number of accidentals not present in the chromatic harmonic scale
    /// - number of accidentals with color different from the local ton,
    ///   or from the global ton if the local ton is unknown.
    /// - number of non-lead Cb B# E# Fb.
    /// - number of non-lead double sharps or double flats.
    ACCIDtb,

    /// number of printed accidentals and tie breaking measures,
    /// some TB measures are summed before comparison.
    ACCIDtbs,

    /// number of accidentals with discount for lead tons.
    /// obsolete.
    ACCIDlead,

    /// sum of number of accidentals and distance to local ton,
    /// and tie breaking measures.
    ADplus,

    /// sum of number of accidentals and distance to local ton, and tie
    /// breaking measures with some TB components summed before comparison.
    ADpluss,

    /// number of accidentals and distance to local ton
    /// compared lexicographicaly, and tie breaking measures.
    ADlex,

    /// number of accidentals and distance to local ton
    /// compared lexicographicaly, and tie breaking measures, with some
    /// TB components summed before comparison.
    ADlexs,

    /// sum of number of accidentals and distance to local ton,
    /// and then (lexicographically) the distance to local ton,
    /// and then tie breaking measures.
    ADplex,

    /// sum of number of accidentals and distance to local ton,
    /// and then (lexicographically) the distance to local ton,
    /// and then tie breaking measures.
    /// with some TB components summed before comparison.
    ADplexs,

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

