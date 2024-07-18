//
//  AlgoName.hpp
//  pse
//
//  Created by Florent on 11/05/2023.
//
/// @addtogroup pse
/// @{

#ifndef AlgoName_hpp
#define AlgoName_hpp

#include <iostream>
#include <string>

#include "pstrace.hpp"


namespace pse {


/// names of  pitch spelling algorithms implemented.
enum class Algo
{
    /// Unknown
    Undef,
    
    /// exhaustive PSE, explore all paths for the construction of tables.
    PSE,

    /// deterministic restriction os PSE
    /// (paths conform to chromanic harmonic scale).
    PSD,

    /// Meredith's PS13
    PS13,
    
    /// rewriting of passing notes
    RewritePassing
        
};

/// string of algo name.
std::string tostring(const Algo& a);

std::ostream& operator<<(std::ostream& o, const Algo& m);

} // end namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::Algo> : fmt::ostream_formatter {};

#endif /* AlgoName_hpp */

/// @} // end group pse


