//
//  KeyFifth.hpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pse
/// @{


#ifndef KeyFifth_hpp
#define KeyFifth_hpp

#include <iostream>

#include "pstrace.hpp"
#include "Fifths.hpp"


namespace pse {


/// @brief Key signature represented as an integer in -7..7
/// = the number of sharps (if positive) or flats (if negative).
/// = distance in the line of fifths to the signature with no accidentals.
///
/// Representation of key signatures:
/// | major | minor | key sig |
/// |:-----:|:-----:|:-------:|
/// |  Cb   |  Ab   |   -7    |
/// |  Gb   |  Eb   |   -6    |
/// |  Db   |  Bb   |   -5    |
/// |  Ab   |  F    |   -4    |
/// |  Eb   |  C    |   -3    |
/// |  Bb   |  G    |   -2    |
/// |  F    |  D    |   -1    |
/// |  C    |  A    |    0    |
/// |  G    |  E    |    1    |
/// |  D    |  B    |    2    |
/// |  A    |  F#   |    3    |
/// |  E    |  C#   |    4    |
/// |  B    |  G#   |    5    |
/// |  F#   |  D#   |    6    |
/// |  C#   |  A#   |    7    |
/// @todo rename KeySig
class KeyFifth
{

public:

    static const int UNDEF = 999;

    /// main constructor
    /// @param signature number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// distance in the array of fifths from a signature with no accidentals.
    KeyFifth(int signature);
    
    /// key signature constructor for a given pitch class in 0..11 and a mode.
    /// @param c a pitch class, in 0..11
    /// @param major true for major key, false for minor key.
    /// @see key(int, bool)
    /// pitch classes:
    /// |:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
    /// | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  |
    /// | C   | C#  | D   | D#  | E   | F   | F#  | G   | G#  | A   | A#  | B   |
    /// | B#  | Db  |     | Eb  | Fb   | E# | Gb  |     | Ab  |     | Bb  | Cb  |
    ///
    /// ambiguities:
    /// - for  1, major, we get -5 (Db M), it could be  7 (C# M)
    /// - for  6, major, we get  6 (F# M), it could be -6 (Gb M)
    /// - for 11, major, we get  5 (B  M), it could be -7 (Cb M)
    /// 
    /// - for 10, minor, we get -5 (Bb m), it could be  7 (A# m)
    /// - for  3, minor, we get  6 (D# m), it could be -6 (Eb m)
    /// - for  8, minor, we get  5 (G# m), it could be -7 (Ab m)
    /// @todo RM obsolete (not used)
    KeyFifth(int c, bool major);

    /// copy constructor
    KeyFifth(const KeyFifth& k);

    /// assignement operator
    KeyFifth& operator=(const KeyFifth& rhs);

    bool operator==(const KeyFifth& rhs) const;
    bool operator!=(const KeyFifth& rhs) const;

    /// @return distance in the array of fifths from a signature 
    /// with no accidentals.
    inline int fifths() const { return _sig; };
    
    /// number of sharp symbols to denote this key signature.
    size_t sharps() const;

    /// number of flat symbols to denote this key signature.
    size_t flats() const;
    
    /// key signature, in -7..7, corresponding to a given pitch class in 0..11.
    /// ambiguities:
    /// - for  1, major, we get -5 (Db M), it could be  7 (C# M)
    /// - for  6, major, we get  6 (F# M), it could be -6 (Gb M)
    /// - for 11, major, we get  5 (B  M), it could be -7 (Cb M)
    /// - for 10, minor, we get -5 (Bb m), it could be  7 (A# m)
    /// - for  3, minor, we get  6 (D# m), it could be -6 (Eb m)
    /// - for  8, minor, we get  5 (G# m), it could be -7 (Ab m)
    /// @param c a pitch class, in 0..11
    /// @param major true for major key, false for minor key.
    /// @todo altkey for ambiguities ?
    static int key(int c, bool major);
    
    void print(std::ostream& o) const;
        
protected:
    
    /// the number of fifths distance
    /// from a signature with no accidentals.
    int _sig;
    
};

std::ostream& operator<<(std::ostream&, const KeyFifth&);

} // end namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::KeyFifth> : fmt::ostream_formatter {};

#endif /* KeyFifth_hpp */

/// @} // end group pse
