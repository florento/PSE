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

#include "pstrace.hpp"


namespace pse {

/// modes for scales
/// @see https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/mode
/// @see https://muted.io/scale-formulas-intervals/
enum class ModeName
{
    /// Major = Ionian
    Major,
    
    /// Mode of C = Major
    Ionian,

    /// Harmonic Minor
    Minor,
    
    /// Melodic Minor (melodic ascending)
    MinorMel,

    /// Major Blues = Major5 + flat 3d blue note
    MajorBlues,

    /// Minor Blues = Minor5 + flat 5th blue note
    MinorBlues,
        
    /// Mode of D
    Dorian,
    
    /// Mode of E
    Phrygian,
    
    /// Mode of F
    Lydian,
    
    /// Mode of G
    Mixolydian,
    
    /// Mode of A = Minor Natural
    Aeolian,

    /// natural Minor (melodic descending) = Aeolian
    MinorNat,
   
    /// Mode of B
    Locrian,
    
    /// Major Pentatonic
    /// = 4 fifths sarting from tonic
    /// = Major - 4th and 7th degrees
    Major5,

    /// Minor Pentatonic = Natural Minor - 2nd and 6th degrees
    Minor5,

    /// Augmented Scale = symmetrical augmented scale = two augmented triads
    Augmented,

    /// Diminished Scale = symmetric diminished scale = two diminished 7th chords
    Diminished,
    
    /// Diminished Scale Half-Whole
    DiminishedHW,
    
    /// Whole-ton Scale
    Whole,
    
    /// Harmonic Chromatic. all notes are repeated except tonic and dominant.
    /// @see https://www.dolmetsch.com/musictheory11.htm
    Chromatic,
    
    /// Unknown
    Undef
    
};

/// the given mode name is one of Ionian, Dorian, Phrygian, Lydian,
/// Mixolydian, Aeolian, Locrian
bool greek(const ModeName& m);

/// total ordering on mode names, induced by the order of definition.
/// can be irrelevant in some cases.
bool operator<(const ModeName& lhs, const ModeName& rhs);

/// negation of >.
bool operator<=(const ModeName& lhs, const ModeName& rhs);

/// inverse (commutation) of <.
bool operator>(const ModeName& lhs, const ModeName& rhs);

/// negation of <.
bool operator>=(const ModeName& lhs, const ModeName& rhs);

/// string of mode name.
std::string tostring(const ModeName& m);

std::ostream& operator<<(std::ostream& o, const ModeName& m);

} // end namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<enum pse::ModeName> : fmt::ostream_formatter {};

#endif /* ModeName_hpp */

/// @} // end group pse

