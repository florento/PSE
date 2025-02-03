//
//  Spiral.hpp
//  squant
//
//  Created by Florent Jacquemard on 04/07/2018.
//  Copyright © 2018 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#ifndef Spiral_hpp
#define Spiral_hpp

#include <stdio.h>
#include <cmath> // = <math.h>

#include "NoteName.hpp"
#include "Accid.hpp"
#include "Pitch.hpp"

namespace pse {

/// @brief Elaine Chew's spiral of fifths.
/// for pitch spelling.
struct SpiralPoint
{
public:
    double x;
    double y;
    double z;
    
    SpiralPoint(double, double, double);
    
    SpiralPoint(const SpiralPoint& rhs);
    
    SpiralPoint& operator= (const SpiralPoint&);
    
    /// @return wether coordinate are not NAN. */
    bool isnormal() const;
    
    void operator+=(const SpiralPoint& rhs);
    void operator-=(const SpiralPoint& rhs);

    void operator*=(double a);
    
    friend inline bool operator==(const SpiralPoint&, const SpiralPoint&);
    friend inline bool operator!=(const SpiralPoint&, const SpiralPoint&);
    
    /// @return Euclidian distance to given point.
    double distance(const SpiralPoint& rhs) const;

    friend inline std::ostream& operator<<(std::ostream& o, 
                                           const SpiralPoint& rhs);
};


inline bool operator==(const SpiralPoint& lhs, const SpiralPoint& rhs)
{
    return ((lhs.x == rhs.x) &&
            (lhs.y == rhs.y) &&
            (lhs.z == rhs.z));
}


inline bool operator!=(const SpiralPoint& lhs, const SpiralPoint& rhs)
{
    return !operator==(lhs,rhs);
}


inline std::ostream& operator<<(std::ostream& o, const SpiralPoint& rhs)
{
    o << "(";
    o << rhs.x;
    o << ", ";
    o << rhs.y;
    o << ", ";
    o << rhs.z;
    o << ")";
    return o;
}


struct SpiralName : public SpiralPoint
{
public:
    static const int UNDEF_NOTE_INDEX;
    
    /// @brief z distance between two successive points of the spiral (one fifth apart).
    static const double h;
    
    /// @brief radius of the cylinder in which the spiral is embedded.
    static const double r;

    /// @brief note name.
    /// 'A' to 'G' or Pitch::UNDEF_NOTE_NAME
    /// @see same has in class Pitch
    char name;
    
    /// @brief note alteration.
    ///
    /// in [-2.0, 2.0] where 1.0 is half tone or Accid::Undef
    /// same has in class Pitch.
    float alteration;
    
    /// @brief position in the line of fifths relative to C
    ///
    /// - C has index 0 and index increases in the direction of sharps:
    /// - G has index 1,  D  has index 2, F# has index 6...
    /// - F has index -1, Bb has index -2...
    ///
    /// values between -15 (Fbb) and 19 (B##)
    ///
    /// TBC: it is redundant with name and alteration
    /// maybe should replace them?
    int index;
    
    SpiralName():
    SpiralPoint(NAN, NAN, NAN),
    name('X'),
    alteration(9),
    index(UNDEF_NOTE_INDEX)
    {}
    
    /// @brief notename object from name, alteration and index.
    /// @param n must be between 'A' and 'G'
    /// @param alt must be between -2.0 and 2.0
    /// @param id must be between -15 and 19
    SpiralName(char n, float alt, int id);
    
    // @brief notename from key signature.
    // (number of sharps - positive or number of flats - negative).
    //SpiralName(int tonality);

    SpiralName(const SpiralName& rhs);
    
    SpiralName& operator=(const SpiralName& rhs);
    
    inline bool unknown() const { return (index == UNDEF_NOTE_INDEX); };
    
    static const SpiralName synonyms[12][3];

    /// @brief ref to a SpiralName in table synonyms.
    static const SpiralName& ofkey(int k);
    
    /// @brief note name (ref in table synonyms) corresponding to given midi pitch
    /// and closest to given point.
    /// @param p point in spiral
    /// @param pitch must be in 0..128
    static const SpiralName& closest(unsigned int pitch, const SpiralPoint& p);

    friend inline bool operator==(const SpiralName&, const SpiralName&);
    friend inline bool operator!=(const SpiralName&, const SpiralName&);

    friend inline std::ostream& operator<<(std::ostream& o, 
                                           const SpiralName& rhs);

};


inline bool operator==(const SpiralName& lhs, const SpiralName& rhs)
{
    return ((((const SpiralPoint&) lhs) == ((const SpiralPoint&) rhs)) &&
            (lhs.name == rhs.name) &&
            (lhs.alteration == rhs.alteration) &&
            (lhs.index == rhs.index));
}


inline bool operator!=(const SpiralName& lhs, const SpiralName& rhs)
{
    return !operator==(lhs,rhs);
}

inline std::ostream& operator<<(std::ostream& o, const SpiralName& p)
{
    if (p.name == 'X') o << "x";
    else o << p.name;
    if ( p.alteration == 9) o << "";
    else if ( p.alteration == 0.0) o << "";
    else if ( p.alteration == 1.0) o << "#";
    else if ( p.alteration == 2.0) o << "##";
    else if ( p.alteration == -1.0) o << "b";
    else if ( p.alteration == -2.0) o << "bb";
    o << " " << (SpiralPoint&) p;
    return o;
}

namespace Spiral
{
   
    const SpiralName& ofkey(int k);
    
    /// @brief note name corresponding to the given midi pitch and closest to given point.
    /// @param p point in spiral
    /// @param pitch must be in 0..128
const pse::NoteName& closest(unsigned int pitch, const SpiralPoint& p);
}

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::SpiralPoint> : fmt::ostream_formatter {};
template<> struct fmt::formatter<pse::SpiralName> : fmt::ostream_formatter {};


#endif /* Spiral_hpp */

/// @}
