//
//  Ton.hpp
//  qparse
//
//  Created by Florent Jacquemard on 03/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pse
/// @{


#ifndef Ton_hpp
#define Ton_hpp

#include <iostream>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Fifths.hpp"
#include "KeyFifth.hpp"


namespace pse {

// class AccidState;

/// A tonality is the combination of  by a Key Signature in -7..7 and a Mode.
/// | ks | Maj | Min  | sens (harm) |
/// |:--:|:---:|:----:|:------------|
/// | -7 | Cb  | Ab   | Gnat        |
/// | -6 | Gb  | Eb   | Dnat        |
/// | -5 | Db  | Bb   | Anat        |
/// | -4 | Ab  | F    | Enat        |
/// | -3 | Eb  | C    | Bnat        |
/// | -2 | Bb  | G    | F#          |
/// | -1 | F   | D    | C#          |
/// |  0 | C   | A    | G#          |
/// |  1 | G   | E    | D#          |
/// |  2 | D   | B    | A#          |
/// |  3 | A   | F#   | E#          |
/// |  4 | E   | C#   | B#          |
/// |  5 | B   | G#   | F##         |
/// |  6 | F#  | D#   | C##         |
/// |  7 | C#  | A#   | G##         |
/// |----|-----|------|-------------|
class Ton : public KeyFifth
{

    friend class AccidentState;
    
public:
    
    /// modes for scales
    /// @see https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/mode/
    enum class Mode
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
    
    /// main constructor
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of this tonality.
    Ton(int ks, Mode mode = Mode::Maj);

    /// upgrade of key signature.
    /// @param ks a key signature on which this tonality will be based.
    /// @param mode mode of this tonality.
    Ton(const KeyFifth& ks, Mode mode = Mode::Maj);

    /// copy constructor
    Ton(const Ton& ton);
    
    /// destructor
    ~Ton();
    
    /// assignement operator
    Ton& operator=(const Ton& rhs);

    bool operator==(const Ton& rhs) const;
    bool operator!=(const Ton& rhs) const;
    
    inline Mode mode() const { return _mode; }

    /// accidental in the key signature of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @return the number of accidents, in the key signature, for n, in -2..2.
    Accid accidKey(const NoteName& n) const;

    /// accidental in the scale of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @return the number of accidents, in scale, for n, in -2..2.
    Accid accidDia(const NoteName& n) const;

    /// a note is a lead in this Ton if
    /// it has an accidental in the scale different
    /// from its accidental in the key signature.
    /// For instance, the leading-tone (7th degree) in minor harmonic.
    /// It is equivalent to accidKey(n) != accidDia(n)
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return wether the note n is a lead in this Ton.
    bool lead(const NoteName& n) const;

    /// distance, in the array of fifths, between the note
    /// (given by name and accidental) and the tonic of this tonality.
    /// @param name a note name in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid alteration in -2..2 (-2 = double flats, 2 = double sharps).
    /// @todo TBR, unused.
    unsigned int dist(const NoteName& name, const Accid& accid) const;

    /// distance, in the array of fifths,
    /// between this tonality and another tonality.
    /// as defined by Diletski
    unsigned int distFifths(const Ton& rhs) const;

    /// Hamming distance between this tonality and another tonality.
    /// number of differently altered notes between them.
    unsigned int distHamming(const Ton& rhs) const;

    /// diatonic distance between this tonality and another tonality.
    /// sum of pointwise distances between alterations of notes between tonalities.
    /// @see accidDist
    unsigned int distDiatonic(const Ton& rhs) const;

    /// distance between this tonality and another tonality
    /// based on the table of J.G. Weber.
    /// @see Weber.hpp
    unsigned int distWeber(const Ton& rhs) const;


    // distance, in the array of fifths, between the note (given by name)
    // and the tonic of the given tonality.
    // @param name a note name in 0..6 (0 is 'C', 6 is 'B').
    // @param alt alteration in -2..2 (-2 = double flats, 2 = double sharps).
    // @param sig a key signature in -7..7.
    // static int dist(int name, int alt, int sig);

    static std::string tostring(const Mode& m);
    
    void print(std::ostream& o) const;
    
protected:

    /// mode of this tonality.
    Mode _mode;
       
private:
    
    /// abbreviations for accidentals
    static const Accid _2F;
    static const Accid _1F;
    static const Accid _0N;
    static const Accid _1S;
    static const Accid _2S;
    static const Accid __U;
    
    /// list of accidents in key signatures,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<Accid, 7>, 15> KEYS;

    /// lead node in minor harmonic tons
    /// one pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for each key signature in -7..7.
    static const std::array<int, 15> LEAD_HARM;

    /// list of accidents in minor harmonic mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<Accid, 7>, 15> MIN_HARM;

    /// list of accidents in minor melodic mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<Accid, 7>, 15> MIN_MEL;
    
    /// accidental in the key signature of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in the key signature, for n, in -2..2.
    /// @todo TBR
    Accid accidKey(int n) const;
    
    /// accidental in the scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in scale, for n, in -2..2.
    Accid accidDia(int n) const;
    
    
};

std::ostream& operator<<(std::ostream& o, const Ton::Mode& m);

std::ostream& operator<<(std::ostream& o, const Ton& ton);

} // end namespace pse
    
#endif /* Ton_hpp */

/// @} // end group pse


