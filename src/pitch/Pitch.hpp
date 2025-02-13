//
//  Pitch.hpp
//  qparse
//
//  Created by Florent Jacquemard on 10/05/17.
//  Copyright © 2017 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef Pitch_hpp
#define Pitch_hpp

#include <stdio.h>
#include <assert.h>
#include <vector>

#include "pstrace.hpp"
#include "NoteName.hpp"    // in PSE
#include "Accid.hpp"  // in PSE
#include "MidiNum.hpp"     // in PSE (for def midi_to_octave)
#include "PWO.hpp"         // in PSE (pitch without octave)


namespace pse {


/// @brief internal representation of a pitch value.
/// it can have:
/// - a MIDI or MIDIcent value
/// - a MIDI value and a name/accidental/octave
/// - none of them (undef pitch)
/// can be undef value
/// @todo extend conversions to MIDIcent (import OM)
class Pitch : public PWO
{
    
public: // constants

    /// MIDI is a MIDI key value in 0..127
    /// MIDICENT is a value in 0..12700 for mico-tonality
    enum class PitchUnit { MIDI, MIDICENT };
    
    /// code for undefuned MIDI value
    static const unsigned int  UNDEF_MIDICENT;
    
    /// @todo TBR (mv to NoteName.hpp)
    static const enum NoteName UNDEF_NOTE_NAME;

    /// @todo TBR (mv to Accid.hpp)
    static const enum Accid    UNDEF_ALTERATION;

    /// code for undefuned octave value
    static const int           UNDEF_OCTAVE;

public: // construction

    /// @brief undef pitch value.
    Pitch();
    
    /// @brief construct pitch from name+alteration+octave.
    /// @param name note name in 'A'..'G'.
    /// @param accid in [-2, 2] where 1.0 is half tone
    /// @param oct in -10..10
    /// @see table pse::Pitch::NAME
    Pitch(const enum NoteName& name,
          const enum Accid& accid,
          int oct = 0);

    /// @brief construct note from MIDI pitch
    /// @param pitch in 0..127
    /// @param unit for pitch : MIDI or MIDICENT
    /// @param named whether a default name must be computed for this pitch.
    Pitch(unsigned int pitch,
          PitchUnit unit = PitchUnit::MIDI,
          bool named = true);
    
    /// @brief copy constructor.
    Pitch(const Pitch& rhs);
    
    /// destructor
    virtual ~Pitch() {}
    
    /// assignment operator
    Pitch& operator=(const Pitch& rhs);
    
    /// equality
    virtual bool equal(const Pitch& rhs) const;

    bool less(const Pitch& rhs) const;

    /// the MIDI value is undefined.
    virtual bool undef() const;
    virtual bool unpitched() const { return undef(); }

    /// the name/alteration/octave values are set.
    bool named() const;
    
    /// set the given name/alteration/octave values.
    /// @param n note name in 'A'..'G'.
    /// @param a accidental in [-2, 2] where 1 is one half tone
    /// @param o octave number, in -10..10
    /// @param altprint whether the accidental must be printed.
    /// @warning the triplet n, a, o must correspond to the midi value
    /// of this pitch.
    void rename(const enum NoteName& n, const enum Accid& a, int o,
                bool altprint);
    //void rename(char n, float alt, int oct, bool altprint);

    /// set the given name value and the alteration and octave accordingly.
    /// @param n a note name in A..G. must be defined.
    /// @warning the name n must be a possible name for the current midi value
    /// of this pitch.
    /// @warning the alt-print flag is set arbitrarily to true.
    void rename(const enum NoteName& n);
    
    /// @brief value in MIDIcent.
    unsigned int midicent() const { return _midi; };
    
    /// @brief value in MIDI.
    unsigned int midi() const { return (_midi/100); };
    
    /// @param a alteration in -2..2
    /// @todo TBR replaced by with Accid
    static std::string alt_to_string(float a);
    
    virtual void print(std::ostream& o) const;

    friend std::ostream& operator<<(std::ostream& o, const Pitch& p);

public: // data

    // name and alteration are in PWO
    
    /// @brief octave in -10..10.
    int octave;

    /// @brief whether the alteration must be printed or not (engraving info).
    /// @warning this value can be set by a pitch spelling algorithm
    bool altprint;
 
private: // data

    /// @brief value in MIDI cent.
    unsigned int _midi;

    /// conversion of pitch names from 0 to 6 into char 'A'..'G' (0 is 'C').
    /// @todo TBR replaced by with NoteName
    static const char NAME[7];

    /// @brief tables for conversion from MIDI values into note names.
    static const char NAMEFLAT[12];
    static const char NAMESHARP[12];
    static const int  FLAT[12];
    static const int  SHARP[12];

private: // non unique
    
    /// @todo TBR replaced by with NoteName
    // static char midi_to_name(unsigned int);
    static enum NoteName midi_to_name(unsigned int m);

    /// @todo TBR replaced by with Accid
    // static float midi_to_alt(unsigned int);
    static enum Accid midi_to_alt(unsigned int m);

public:  // unique

    // @param m midi number
    // @todo TBR (incorrect)
    // static int midi_to_octave(unsigned int m);

    /// @param m midicent number
    /// @todo TBR (incorrect)
    static int midicent_to_octave(unsigned int m);

    // @param m midi number
    // @param n note name in 'A'..'G'.
    // @brief a accidental in [-2, 2], where 1.0 is a half tone.
    // @return octave number, in -2..9, for the note of given midi nb, name
    // and accidental.
    // @todo mv to MidiNum
    // static int midi_to_octave(unsigned int m,
    //                           const enum NoteName& n,
    //                           const enum Accid& a);

    /// midi cent value corresponding to the given note name.
    /// @todo mv to MidiNum
    static unsigned int to_midi(const enum NoteName& name,
                                const enum Accid& accid,
                                int octave);
    //static unsigned int to_midi(char, float, int);

    /// pitch class, in 0..11, of the given note name.
    /// @todo TBR mv to NoteName
    static unsigned int pitchClass(const enum NoteName& name);
    
    /// encoding of given note name in 0..6 (0 is 'C', 6 is 'B').
    /// @param n note name in 'A'..'G'.
    /// @return int encoding in 0..6 (0 is 'C', 6 is 'B').
    /// @todo TBR, use NoteName
    static int name_to_int(char n);

    /// note name correspoding to given encoding.
    /// @return int encoding of name, in 0..6 (0 is 'C', 6 is 'B').
    /// @return corresponding note name in 'A'..'G'.
    /// @todo TBR, use NoteName
    static char name_of_int(int c);

}; // end class Pitch


// boolean operators
inline bool operator==(const Pitch& lhs, const Pitch& rhs)
{
    return lhs.equal(rhs);
}

inline bool operator!=(const Pitch& lhs, const Pitch& rhs)
{
    return !operator==(lhs,rhs);
}

inline bool operator<(const Pitch& lhs, const Pitch& rhs)
{
    return (lhs.less(rhs));
}

inline bool operator>(const Pitch& lhs, const Pitch& rhs)
{
    return  operator<(rhs,lhs);
}

inline bool operator<=(const Pitch& lhs, const Pitch& rhs)
{
    return !operator> (lhs, rhs);
}

inline bool operator>=(const Pitch& lhs, const Pitch& rhs)
{
    return !operator< (lhs,rhs);    
}

} // namespace pse

/// fmt v10 and above requires `fmt::formatter<T>` extends `fmt::ostream_formatter`.
/// @see: https://github.com/fmtlib/fmt/issues/3318
template<> struct fmt::formatter<pse::Pitch> : fmt::ostream_formatter {};

#endif /* Pitch_hpp */

/// @}

