//
//  Scale.hpp
//  pse
//
//  Created by Florent on 24/03/2023.
//

#ifndef Scale_hpp
#define Scale_hpp

#include <iostream>
#include <vector>

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "MidiNum.hpp"
//#include "Fifths.hpp"
#include "KeyFifth.hpp"
#include "ModeName.hpp"
#include "Mode.hpp"
//#include "Ton.hpp"

namespace pse {

class Ton;

/// Major and harmonic minor scales for each pitch class
/// with enharmonics
///
/// | pc | maj          | min          |
/// |:--:|:------------:|:------------:|
/// |  0 | C(0)         | C(-3)        |
/// |  1 | C#(7) Db(-5) | C#(4)        |
/// |  2 | D(2)         | D(-1)        |
/// |  3 | Eb(-3)       | Eb(-6) D#(6) |
/// |  4 | E(4)         | E(1)         |
/// |  5 | F(-1)        | F(-4)        |
/// |  6 | F#(6) Gb(-6) | F#(3)        |
/// |  7 | G(1)         | G(-2)        |
/// |  8 | Ab(-4)       | Ab(-7) G#(5) |
/// |  9 | A(3)         | A(0)         |
/// | 10 | Bb(-2)       | Bb(-5) A#(7) |
/// | 11 | B(5) Cb(-7)  | B(2)         |


/// a scale is defined by a mode, a pitch class and the name of tonic.
/// It has a certain number of degrees (its size()),
/// and for each degree,
/// - one note name,
/// - one accidental,
/// - one pitch class (which depends on the name and accid).
///
class Scale  // public Mode
{
public:

    /// undefined scale.
    Scale();
    
    /// main constructor.
    /// @param mode mode of this scale
    /// @param pc pitch class of tonic, in 0..11
    /// @param name name of tonic, in A..G
    Scale(const ModeName& mode, int pc, const enum NoteName& name);

    /// constructor from mode.
    /// @param mode mode of this scale
    /// @param pc pitch class of tonic, in 0..11
    /// @param name name of tonic, in A..G
    Scale(const Mode& mode, int pc, const enum NoteName& name);

    /// constructor of a scale associated to a given ton.
    /// @param ton a tonality, i.e. a diatonic scale
    /// (defined by key signature and mode). Its mode must be diatonic.
    Scale(const Ton& ton);

    /// chromatic harmonic scale associated to a given ton.
    /// @param ton a tonality, i.e. a diatonic scale
    /// (defined by key signature and mode). Its mode must be diatonic.
    /// @param mode must be ModeName::Chromatic.
    Scale(const Ton& ton, const ModeName& mode);

    /// number of degrees of this scale.
    size_t size() const;

    /// this scale is undefined.
    bool undef() const;
    
    /// note name for given degree.
    /// @param d degree in this scale, in 0..12.
    /// @return the note name for the given degree, in A..G.
    /// @warning the first degree is 0 (not 1).
    enum NoteName name(size_t d) const;

    /// accidental name for given degree.
    /// @param d degree in this scale, in 0..12.
    /// @return the accidentals for the given degree, in -2..2.
    /// @warning the first degree is 0 (not 1).
    enum Accid accid(size_t d) const;

    /// pitch class for given degree
    /// @param d degree in this scale, in 0..12.
    /// @return number of pitch class in 0..11.
    /// @warning the first degree is 0 (not 1).
    int pitchClass(size_t d) const;

    /// this scale contains the note of given name and accidental.
    /// @param n note name in 'A'..'G'.
    /// @brief a accidental in [-2, 2], where 1.0 is a half tone.
    bool contains(const enum NoteName& n, const enum Accid& a) const;

    /// distance to C maj in the line of fifths
    /// @return the key signature in -7..7
    int fifths() const;

    /// key signature for diatonic scales (0 for others).
    /// @return the key signature in -7..7
    KeyFifth keySignature() const;

protected: //data

    /// mode of scale
    ModeName _mode;
    
    /// pitch class of every degree
    std::vector<int> _pcs;
   
    /// name of every degree
    std::vector<enum NoteName> _names;

    /// accidental of every degree
    std::vector<enum Accid> _accids;

    /// key signature for diatonic scales (0 for others)
    KeyFifth _ks;

    // void init_ks();

protected:
    
    /// mode is major diatonic.
    static bool major(const Mode& mode);

    /// mode is minor diatonic
    /// (harmonic minor or melodic minor or natural minor).
    static bool minor(const Mode& mode);

    /// mode is diatonic.
    static bool diatonic(const Mode& mode);

};

std::ostream& operator<<(std::ostream& o, const Scale& s);

} // end namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::Scale> : fmt::ostream_formatter {};

#endif /* Scale_hpp */

/// @} // end group pse
