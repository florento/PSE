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
#include <memory>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "AccidPair.hpp"
#include "ModeName.hpp"
#include "Fifths.hpp"
#include "KeyFifth.hpp"
#include "Scale.hpp"


namespace pse {

// class PSState;

/// A tonality is the combination of a Key Signature in -7..7 and a diatonic Mode.
/// It defines a diatonic scale:
/// 7 notes of distinct names and with or without accident.
///
/// | ks | Maj | Min  | sens (harm) |
/// |:--:|:---:|:----:|:-----------:|
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

    friend class PSState;
    
public:
    
    /// undefined tonality.
    Ton();
    
    /// main constructor.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of this tonality.
    Ton(int ks, ModeName mode = ModeName::Major);

    /// upgrade of key signature.
    /// @param ks a key signature on which this tonality will be based.
    /// @param mode mode of this tonality.
    Ton(const KeyFifth& ks, ModeName mode = ModeName::Major);

    /// copy constructor
    Ton(const Ton& ton);
    
    /// destructor
    ~Ton();
    
    /// assignement operator
    Ton& operator=(const Ton& rhs);

    bool operator==(const Ton& rhs) const;
    bool operator!=(const Ton& rhs) const;
    
    inline ModeName getMode() const { return _mode; }

    /// note name of this tonality.
    const enum NoteName getName() const;

    /// accidental name of this tonality.
    const enum Accid getAccidental() const;

    /// pitch class of this tonality.
    int getPitchClass() const;

    /// this tonality is undefined.
    bool undef() const;

    /// this tonality is defined.
    inline bool defined() const { return !undef(); }

    /// accidental in the key signature of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @return the number of accidents, in the key signature, for n, in -2..2.
    enum Accid accidKey(const enum NoteName& n) const;

    /// accidental in the key signature of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in the key signature, for n, in -2..2.
    /// @todo TBR
    enum Accid accidKey(int n) const;
    
    /// accidental in the diatonic scale of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @return the number of accidents, in scale, for degree d, in -2..2.
    enum Accid accidDia(const enum NoteName& n) const;

    /// accidental in the diatonic scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in scale, for n, in -2..2.
    enum Accid accidDia(int n) const;

    /// accidental in the diatonic scale of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @param mode a mode can be different from the one of this ton
    /// @todo mode what for ?
    /// @return the number of accidents, in scale, for degree d, in -2..2.
    enum Accid accidDia(const enum NoteName& n, ModeName mode) const;
        
    /// note name in the diatonic scale of this ton for a given degree.
    /// @param d a degree in 0..6.
    /// @return the note name in scale, for degree d, in 'C'..'B'.
    /// @warning only for distonic scales: the mode must be
    /// Major or Minor or MinorNat or MinorMel.
    enum NoteName name(int d) const;

    /// accidental in the diatonic scale of this ton for a given degree.
    /// @param d a degree in 0..6.
    /// @return the number of accidents, in scale, for n, in -2..2.
    /// @warning only for distonic scales: the mode must be
    /// Major or Minor or MinorNat or MinorMel.
    /// @todo TBR not used?
    enum Accid accidental(int d) const;
  
    /// a note is a lead in this Ton if
    /// it has an accidental in the scale different
    /// from its accidental in the key signature.
    /// For instance, the leading-tone (7th degree) in minor harmonic.
    /// It is equivalent to accidKey(n) != accidDia(n)
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return wether the note n is a lead in this Ton.
    bool lead(const enum NoteName& n) const;

    /// distance, in the array of fifths, between the note
    /// (given by name and accidental) and the tonic of this tonality.
    /// @param name a note name in 0..6 (0 is 'C', 6 is 'B').
    /// @param accid alteration in -2..2 (-2 = double flats, 2 = double sharps).
    /// @todo TBR, unused.
    unsigned int dist(const enum NoteName& name, const enum Accid& accid) const;

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
    
    /// distance between this tonality and another tonality
    /// based on the extension of the table of J.G. Weber.
    /// @see WeberModal.hpp
    unsigned int distWeberModal(const Ton& rhs) const;

    // distance, in the array of fifths, between the note (given by name)
    // and the tonic of the given tonality.
    // @param name a note name in 0..6 (0 is 'C', 6 is 'B').
    // @param alt alteration in -2..2 (-2 = double flats, 2 = double sharps).
    // @param sig a key signature in -7..7.
    // static int dist(int name, int alt, int sig);
    
    /// chromatic harmonic scale associated with this ton.
    /// the chromatic harmonic modal scale embeds the epsllings of all the
    /// traditional seven-tone modes that contain a perfect fifth
    /// above the tonic note, including:
    /// - Harmonic Minor
    /// - Melodic Minor,
    /// - Ionian mode,
    /// - Dorian mode,
    /// - Phrygian mode,
    /// - Lydian mode,
    /// - Mixolydian mode,
    /// - Aeolian mode.
    /// @see https://www.jomarpress.com/nagel/articles/ChromaticModal.html
    const Scale& chromatic() const;

    void print(std::ostream& o) const;
    
protected: // data

    /// mode of this tonality.
    ModeName _mode;
    
    /// memoization of the chromatic harmonic scale associated with this ton.
    // std::shared_ptr<Scale> _chromatic;
    const Scale _chromatic;

private:
    
    /// abbreviation for accidentals: double flat
    static const enum Accid _F;
    
    /// abbreviation for accidentals: flat
    static const enum Accid _f;
    
    /// abbreviation for accidentals: neutral
    static const enum Accid _n;
    
    /// abbreviation for accidentals: sharp
    static const enum Accid _s;
    
    /// abbreviation for accidentals: double sharp
    static const enum Accid _S;
    
    /// abbreviation for accidentals: undef;
    static const enum Accid _U;

    /// abbreviation for pair of accidentals: double flat or flat
    static const AccidPair _Ff;

    /// abbreviation for pair of accidentals: single flat
    static const AccidPair _f_;

    /// abbreviation for pair of accidentals: flat or neutral
    static const AccidPair _fn;

    /// abbreviation for pair of accidentals: single neutral
    static const AccidPair _n_;

    /// abbreviation for pair of accidentals: neutral or sharp
    static const AccidPair _ns;

    /// abbreviation for pair of accidentals: single sharp
    static const AccidPair _s_;

    /// abbreviation for pair of accidentals: sharp or double sharp
    static const AccidPair _sS;

    /// abbreviation for pair of accidentals: undef
    static const AccidPair _UU;

    /// list of accidents in key signatures,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> MAJOR;

    /// lead node in minor harmonic tons
    /// one pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for each key signature in -7..7.
    static const std::array<int, 15> LEAD_HARM;

    /// list of accidents in minor harmonic mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_HARM;

    /// list of accidents in minor melodic mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> MIN_MEL;
    
    /// list of accidents in dorian mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> DORIAN;

    /// list of accidents in phrygian mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> PHRYGIAN;

    /// list of accidents in lydian mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> LYDIAN;

    /// list of accidents in mixolydian mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> MIXOLYDIAN;

    /// list of accidents in locrian mode,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<enum Accid, 7>, 15> LOCRIAN;
    
    /// list of accidents (pairs) in major blues scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<AccidPair, 7>, 15> MAJ_BLUES;
    
    /// list of accidents (pairs) in minor blues scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<AccidPair, 7>, 15> MIN_BLUES;
    
    /// number of the tonic of this ton in the array of fifths.
    int tonic() const;
    
    /// accidental in the scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in scale, for n, in -2..2.
    enum Accid accidDia(int n, ModeName mode) const;
        
};

std::ostream& operator<<(std::ostream& o, const Ton& ton);

} // end namespace pse
    
#endif /* Ton_hpp */

/// @} // end group pse


