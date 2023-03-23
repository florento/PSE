//
//  ModeName.hpp
//  pse
//
//  Created by Florent on 23/03/2023.
//
/// @addtogroup pse
/// @{


#ifndef ModeName_hpp
#define ModeName_hpp

#include <iostream>
#include <string>

#include "trace.hpp"


namespace pse {

/// modes for scales
/// @see https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/mode/
enum class ModeName
{
    /// unknown
    Undef,

    /// major
    Maj,
    
    /// harmonic minor
    Min,

    /// natural minor (melodique descendant)
    MinNat,

    /// melodic minor (melodique ascendant)
    MinMel,

    /// mode of C
    Ionian,

    /// mode of D
    Dorian,

    /// mode of E
    Phrygian,
    
    /// mode of F
    Lydian,

    /// mode of G
    Mixolydian,

    /// mode of A
    Eolian,

    /// mode of B
    Locrian
};


/// string of mode name.
std::string tostring(const ModeName& m);

std::ostream& operator<<(std::ostream& o, const ModeName& m);


} // end namespace pse

#endif /* ModeName_hpp */

/// @} // end group pse

