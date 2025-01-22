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

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Accids.hpp"
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
/// @todo suppr. _chromatic (use chromaName)
class Ton : public KeyFifth
{

    friend class PSState;
    
public:
    
    /// undefined tonality.
    Ton();
    
    /// main constructor.
    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of this tonality. supported values:
    /// - Major, KS in -7..7.
    /// - Minor (harmonic minor), KS in -7..7.
    /// - MinorNat, KS in -7..7.
    /// - MinorMel, KS in -7..7.
    /// - Ionian = Major, KS in -7..7.
    /// - Dorian, KS in -7..7.
    /// - Phrygian, KS in -7..7.
    /// - Lydian, KS in -7..7.
    /// - Mixolydian, KS in -7..7.
    /// - Aeolian = MinorNat, KS in -7..7.
    /// - Locrian, KS in -7..7.
    /// - MajorBlues, KS in -5..6, -7=5, -6=6, 7=-5
    /// - MinorBlues, KS in -5..6, -7=5, -6=6, 7=-5
    /// - Augmented, KS in -5..6, -7=5, -6=6, 7=-5
    /// - Diminished, KS in -1..1, -7=5,
    ///   -6=6, -5=1, -4=-1, -3=0, -2=1, 2=-1, 3=0, 4=1, 5=-1, 6=0, 7=-5
    /// - DiminishedHW, KS in -1..1.
    ///   -7=5 -6=6, -5=1, -4=-1, -3=0, -2=1, 2=-1, 3=0, 4=1, 5=-1, 6=0, 7=-5
    /// - Whole, KS in 0..1.
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
    
    /// mode of this tonality.
    inline ModeName getMode() const { return _mode; }

    /// the mode of this tonality is major or minor harmonic.
    bool isMajorMinor() const;

    /// this tonality is undefined.
    bool undef() const;

    /// this tonality is defined.
    inline bool defined() const { return !undef(); }

    /// note name of this tonality.
    const enum NoteName getName() const;

    /// accidental name of this tonality.
    const enum Accid getAccidental() const;

    /// pitch class of this tonality.
    int getPitchClass() const;
    
    /// key signature truly reflecting the notes constituting the considered scale
    int getRealKS() const;

    /// this ton has the same accidents, in Key Signature or Scale,
    /// as the given ton.
    /// @param rhs other ton to be compered to.
    /// @param ks consider the accidents in Key Signature for equivalence,
    /// otherwise consider the accidents in scale.
    /// @see PSState
    bool equivalent(const Ton& rhs, bool ks) const;

    /// accidental in the key signature of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @return the number of accidents, in the key signature, for n, in -2..2.
    /// @todo mv KeySig::accid
    enum Accid accidKey(const enum NoteName& n) const;

    /// accidental in the key signature of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in the key signature, for n, in -2..2.
    /// @todo mv KeySig::accid
    enum Accid accidKey(int n) const;

    /// accidental in the scale of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @return the accidental(s), in scale, for note name n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    accids_t accidScale(const enum NoteName& n) const;

    /// accidental in the scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental(s), in scale, for note n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    accids_t accidScale(int n) const;

    /// accidental in the scale of this ton for a given pitch name.
    /// @param n an encapsulated note name
    /// @param mode a mode can be different from the one of this ton.
    /// @return the accidental(s), in scale, for note name n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    accids_t accidScale(const enum NoteName& n, ModeName mode) const;

    /// accidental in the diatonic scale of this ton for a given pitch name.
    /// @param n an encapsulated note name.
    /// @return the accidental, in scale, for note name n.
    /// @todo replaced by accidScale
    enum Accid accidDia(const enum NoteName& n) const;

    /// accidental in the diatonic scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return the accidental, in scale, for note n.
    /// @todo replaced by accidScale
    enum Accid accidDia(int n) const;

    /// accidental in the diatonic scale of this ton for a given pitch name.
    /// @param n an encapsulated note name.
    /// @param mode a mode can be different from the one of this ton.
    /// @return the accidental, in scale, for note n.
    /// @todo mode what for ?
    /// @todo replace by accidScale
    enum Accid accidDia(const enum NoteName& n, ModeName mode) const;
        
    /// note name in the diatonic scale of this ton for a given degree.
    /// @param d a degree in 0..6.
    /// @return the note name in scale, for degree d, in 'C'..'B'.
    /// @warning only for diatonic scales: the mode must be
    /// Major or Minor or MinorNat or MinorMel.
    /// @todo TBR unused?
    enum NoteName name(int d) const;

    /// accidental in the diatonic scale of this ton for a given degree.
    /// @param d a degree in 0..6.
    /// @return the number of accidents, in scale, for n, in -2..2.
    /// @warning only for distonic scales: the mode must be
    /// Major or Minor or MinorNat or MinorMel.
    /// @todo TBR OBSOLETE (not used?)
    enum Accid accidental(int d) const;
  
    /// a note is a lead in this Ton if
    /// it has an accidental in the scale different
    /// from its accidental in the key signature.
    /// For instance, the leading-tone (7th degree) in minor harmonic.
    /// It is equivalent to accidKey(n) != accidDia(n)
    /// @param n a pitch name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return wether the note n is a lead in this Ton.
    /// @warning only for diatonic scales: the mode must be
    /// Major or Minor or MinorNat or MinorMel.
    /// @todo TBR unused?
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

    /// distance between this tonality and another tonality
    /// based on the extension of the table of J.G. Weber.
    /// @see WeberBluesModal.hpp
    unsigned int distWeberBluesModal(const Ton& rhs) const;

    // distance, in the array of fifths, between the note (given by name)
    // and the tonic of the given tonality.
    // @param name a note name in 0..6 (0 is 'C', 6 is 'B').
    // @param alt alteration in -2..2 (-2 = double flats, 2 = double sharps).
    // @param sig a key signature in -7..7.
    // static int dist(int name, int alt, int sig);
    
    /// chromatic harmonic scale associated with this ton.
    /// the chromatic harmonic modal scale embeds the spellings of all the
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
    // const Scale& chromatic() const;
    const Ton& chromaton() const;
    
    /// note name corresponding to the given pitch class in the
    /// chromatic harmonic scale associated to this ton.
    /// @todo test in TestTon
    const enum NoteName chromaname(int pc) const;

    void print(std::ostream& o) const;
    
protected: // data

    /// mode of this tonality.
    ModeName _mode;
    
    // memoization of the chromatic harmonic scale associated with this ton.
    // std::shared_ptr<Scale> _chromatic;
    // const Scale _chromatic;
    
private: // convenience functions

    /// @param ks number of flats if negative int,
    /// or number of sharps if positive int. must be in -7..7.
    /// @param mode mode of this tonality.
    /// @return representative of ks for the given mode.
    int init_KSofMode(int ks, ModeName mode);
    
    /// number of the tonic of this ton in the array of fifths.
    int tonicFifth() const;
    
    /// accidental in the scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @param mode a mode can be different from the one of this ton.
    /// @return the accidental, in scale, for n, in -2..2.
    /// @todo TBR
    enum Accid accidDia(int n, ModeName mode) const;
        
    /// accidental in the scale of this ton for a given pitch name.
    /// @param n a note name, in 0..6 (0 is 'C', 6 is 'B').
    /// @param mode a mode can be different from the one of this ton.
    /// @return the accidental(s), in scale, for note n,
    /// can be a single accidental value, a pair of accidents or UNDEF.
    accids_t accidScale(int n, ModeName mode) const;
    
    /// @param a an encoding of accids. must be single.
    /// @return the unique accid value encoded in the encoding a.
    static enum Accid accidSingle(const accids_t a);
    
private: // tables
    
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
    static const accids_t _Ff;

    /// abbreviation for pair of accidentals: single flat
    static const accids_t _f_;

    /// abbreviation for pair of accidentals: flat or neutral
    static const accids_t _fn;

    /// abbreviation for pair of accidentals: single neutral
    static const accids_t _n_;

    /// abbreviation for pair of accidentals: neutral or sharp
    static const accids_t _ns;

    /// abbreviation for pair of accidentals: single sharp
    static const accids_t _s_;

    /// abbreviation for pair of accidentals: sharp or double sharp
    static const accids_t _sS;

    /// abbreviation for pair of accidentals: undef
    static const accids_t _UU;

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
    
    /// list of accidents (pairs) in the major blues scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<accids_t, 7>, 15> MAJ_BLUES;
    
    /// list of accidents (pairs) in the minor blues scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..7.
    static const std::array<std::array<accids_t, 7>, 15> MIN_BLUES;

    /// list of accidents (pairs) in whole tone scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every 2 key signatures 0, 1
    static const std::array<std::array<accids_t, 7>, 2> WHOLE_TONE;

    /// list of accidents (pairs) in the augmented scale
    /// (made of two augmented chords),
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every 12 key signatures in -5..6
    static const std::array<std::array<accids_t, 7>, 12> AUGMENTED;
    
    /// list of accidents (pairs) in the whole/half diminished scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every 3 key signatures in -1..1
    static const std::array<std::array<accids_t, 7>, 3> DIMINISHED_WH;

    /// list of accidents (pairs) in the half/whole diminished scale
    /// (dominant diminished scale),
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for 3 key signatures in -1..1
    static const std::array<std::array<accids_t, 7>, 3> DIMINISHED_HW;
    
    /// list of accidents (pairs) in the chromatic harmonic scale,
    /// for every pitch name in 0..6 (0 is 'C', 6 is 'B'),
    /// for every key signature in -7..10.
    static const std::array<std::array<accids_t, 7>, 18> CHROMATIC;
    
    /// static list of dist, in names, from tonic, according to the number of
    /// semitons (from tonic).
    static const std::array<int, 12> CHROMA_NAMES;

    /// static list of chromatic harmonic scales for each KS in -7..10
    static const std::array<Ton, 18> CHROMA_TONS;

};

std::ostream& operator<<(std::ostream& o, const Ton& ton);

} // end namespace pse
    
/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::Ton> : fmt::ostream_formatter {};

#endif /* Ton_hpp */

/// @} // end group pse


