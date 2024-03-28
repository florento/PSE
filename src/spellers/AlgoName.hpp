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

#include "trace.hpp"


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

#endif /* AlgoName_hpp */

/// @} // end group pse


