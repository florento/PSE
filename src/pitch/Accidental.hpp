//
//  Accidental.hpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef Accidental_hpp
#define Accidental_hpp

#include <iostream>
//#include <ostream>
#include <assert.h>
#include <string>

#include "trace.hpp"
//#include "Rational.hpp"


namespace pse {

/// Accidental values
/// @see https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/accidental-value/
/// @todo complete for micro tonality
enum class Accid
{
    /// +1 ton 1/2
    TripleSharp,
    
    /// +1 ton
    DoubleSharp,
    
    /// +3/4 ton
    ThreeQuartersSharp,
    
    /// +1/2 ton
    Sharp,
    
    /// +1/4 ton
    QuarterSharp,
    
    /// +0 ton
    Natural,
    
    /// -1/4 ton
    QuarterFlat,
    
    /// -1/2 ton
    Flat,
    
    /// -3/4 ton
    ThreeQuartersFlat,
    
    /// -1 ton
    DoubleFlat,
    
    /// -1 ton 1/2
    TripleFlat,
    
    /// undefined accident (ignore)
    Undef
};

/// this accidental is not undef.
bool defined(const Accid& a);

/// distance between two accidentals, in 1/2 tons.
/// @param lhs a defined accidental.
/// @param rhs a defined accidental.
unsigned int accidDist(const Accid& lhs, const Accid& rhs);
   
/// distance between two accidentals.
/// @param lhs a defined accidental.
/// @param rhs a defined accidental.
unsigned int accidDistSigned(const Accid& lhs, const Accid& rhs);

/// cast accidental value.
/// only for internal use.
int toint(const Accid& a);

/// accidental is one or more flats
bool flat(const Accid& a);

/// accidental is a natural
bool natural(const Accid& a);

/// accidental is one or more sharps
bool sharp(const Accid& a);

/// constructor of accidental value.
/// only for internal use.
/// @param a an integral number of 1/2 tons,
/// positive for sharps, negative for flat.
/// must be between -3 and 3 (included, otherwise, undef.
Accid accidofint(int a);

/// cast accidental value for printing.
std::string tostring(const Accid& a);

/// print accidental value.
std::ostream& operator<<(std::ostream& o, const Accid& c);

} // namespace pse

#endif /* Accidental_hpp */

/// @}
