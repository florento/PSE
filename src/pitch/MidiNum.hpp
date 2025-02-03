//
//  MidiNum.hpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef MidiNum_hpp
#define MidiNum_hpp

#include <iostream>
#include <math.h>       /* floor */

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "PWO.hpp"


namespace pse {

/// @see https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
///
/// | MIDI | piano | organ | name0 | name1 | name2 | chroma |
/// |:----:|:-----:|:-----:|:------|:------|:------|:------:|
/// |   0  |       |       | Dbb-1 | C-1   | B#-2  |  0     |
/// |   1  |       |       | Db-1  | C#-1  |[B##-2]|  1     |
/// |   2  |       |       | Ebb-1 | D-1   | C##-1 |  2     |
/// |   3  |       |       |[Fbb-1]| Eb-1  | D#-1  |  3     |
/// |   4  |       |       | Fb-1  | E-1   | D##-1 |  4     |
/// |   5  |       |       | Gbb-1 | F-1   | E#-1  |  5     |
/// |   6  |       |       | Gb-1  | F#-1  |[E##-1]|  6     |
/// |   7  |       |       | Abb-1 | G-1   | F##-1 |  7     |
/// |   8  |       |       | Ab-1  | G#-1  |       |  8     |
/// |   9  |       |       | Bbb-1 | A-1   | G##-1 |  9     |
/// |  10  |       |       |[Cbb0] | Bb-1  | A#-1  | 10     |
/// |  11  |       |       | Cb0   | B-1   | A##-1 | 11     |
/// |  12  |       |       | Dbb0  | C0    | B#-1  |  0     |
/// |  13  |       |       | Db0   | C#0   |[B##-1]|  1     |
/// |  14  |       |       | Ebb0  | D0    | C##0  |  2     |
/// |  15  |       |       |[Fbb0] | Eb0   | D#0   |  3     |
/// |  16  |       |       | Fb0   | E0    | D##0  |  4     |
/// |  17  |       |       | Gbb0  | F0    | E#0   |  5     |
/// |  18  |       |       | Gb0   | F#0   |[E##0] |  6     |
/// |  19  |       |       | Abb0  | G0    | F##0  |  7     |
/// |  29  |       |       | Ab0   | G#0   |       |  8     |
/// |  21  |   1   |       | Bbb0  | A0    | G##0  |  9     |
/// |  22  |   2   |       |[Cbb1] | Bb0   | A#0   | 10     |
/// |  23  |   3   |       | Cb1   | B0    | A##0  | 11     |
/// |  24  |   4   |       | Dbb1  | C1    | B#0   |  0     |
/// |  25  |   5   |       | Db1   | C#1   |[B##0] |  1     |
/// |  26  |   6   |       | Ebb1  | D1    | C##1  |  2     |
/// |  27  |   7   |       |[Fbb1] | Eb1   | D#1   |  3     |
/// |  28  |   8   |       | Fb1   | E1    | D##1  |  4     |
/// |  29  |   9   |       | Gbb1  | F1    | E#1   |  5     |
/// |  30  |  10   |       | Gb1   | F#1   |[E##1] |  6     |
/// |  31  |  11   |       | Abb1  | G1    | F##1  |  7     |
/// |  32  |  12   |       | Ab1   | G#1   |       |  8     |
/// |  33  |  13   |       | Bbb1  | A1    | G##1  |  9     |
/// |  34  |  14   |       |[Cbb2] | Bb1   | A#1   | 10     |
/// |  35  |  15   |       | Cb2   | B1    | A##1  | 11     |
/// |  36  |  16   |   1   | Dbb2  | C2    | B#1   |  0     |
/// |  37  |  17   |   2   | Db2   | C#2   |[B##1] |  1     |
/// |  38  |  18   |   3   | Ebb2  | D2    | C##2  |  2     |
/// |  39  |  19   |   4   |[Fbb2] | Eb2   | D#2   |  3     |
/// |  40  |  20   |   5   | Fb2   | E2    | D##2  |  4     |
/// |  41  |  21   |   6   | Gbb2  | F2    | E#2   |  5     |
/// |  42  |  22   |   7   | Gb2   | F#2   |[E##2] |  6     |
/// |  43  |  23   |   8   | Abb2  | G2    | F##2  |  7     |
/// |  44  |  24   |   9   | Ab2   | G#2   |       |  8     |
/// |  45  |  25   |  10   | Bbb2  | A2    | G##2  |  9     |
/// |  46  |  26   |  11   |[Cbb3] | Bb2   | A#2   | 10     |
/// |  47  |  27   |  12   | Cb3   | B2    | A##2  | 11     |
/// |  48  |  28   |  13   | Dbb3  | C3    | B#2   |  0     |
/// |  49  |  29   |  14   | Db3   | C#3   |[B##2] |  1     |
/// |  50  |  30   |  15   | Ebb3  | D3    | C##3  |  2     |
/// |  51  |  31   |  16   |[Fbb3] | Eb3   | D#3   |  3     |
/// |  52  |  32   |  17   | Fb3   | E3    | D##3  |  4     |
/// |  53  |  33   |  18   | Gbb3  | F3    | E#3   |  5     |
/// |  54  |  34   |  19   | Gb3   | F#3   |[E##3] |  6     |
/// |  55  |  35   |  20   | Abb3  | G3    | F##3  |  7     |
/// |  56  |  36   |  21   | Ab3   | G#3   |       |  8     |
/// |  57  |  37   |  22   | Bbb3  | A3    | G##3  |  9     |
/// |  58  |  38   |  23   |[Cbb4] | Bb3   | A#3   | 10     |
/// |  59  |  39   |  24   | Cb4   | B3    | A##3  | 11     |
/// |  60  |  40   |  25   | Dbb4  | C4    | B#3   |  0     |
/// |  61  |  41   |  26   | Db4   | C#4   |[B##3] |  1     |
/// |  62  |  42   |  27   | Ebb4  | D4    | C##4  |  2     |
/// |  63  |  43   |  28   |[Fbb4] | Eb4   | D#4   |  3     |
/// |  64  |  44   |  29   | Fb4   | E4    | D##4  |  4     |
/// |  65  |  45   |  30   | Gbb4  | F4    | E#4   |  5     |
/// |  66  |  46   |  31   | Gb4   | F#4   |[E##4] |  6     |
/// |  67  |  47   |  32   | Abb4  | G4    | F##4  |  7     |
/// |  68  |  48   |  33   | Ab4   | G#4   |       |  8     |
/// |  69  |  49   |  34   | Bbb4  | A4    | G##4  |  9     |
/// |  70  |  50   |  35   |[Cbb5] | Bb4   | A#4   | 10     |
/// |  71  |  51   |  36   | Cb5   | B4    | A##4  | 11     |
/// |  72  |  52   |  37   | Dbb5  | C5    | B#4   |  0     |
/// |  73  |  53   |  38   | Db5   | C#5   |[B##4] |  1     |
/// |  74  |  54   |  39   | Ebb5  | D5    | C##5  |  2     |
/// |  75  |  55   |  40   |[Fbb5] | Eb5   | D#5   |  3     |
/// |  76  |  56   |  41   | Fb5   | E5    | D##5  |  4     |
/// |  77  |  57   |  42   | Gbb5  | F5    | E#5   |  5     |
/// |  78  |  58   |  43   | Gb5   | F#5   |[E##5] |  6     |
/// |  79  |  59   |  44   | Abb5  | G5    | F##5  |  7     |
/// |  80  |  60   |  45   | Ab5   | G#5   |       |  8     |
/// |  81  |  61   |  46   | Bbb5  | A5    | G##5  |  9     |
/// |  82  |  62   |  47   |[Cbb6] | Bb5   | A#5   | 10     |
/// |  83  |  63   |  48   | Cb6   | B5    | A##5  | 11     |
/// |  84  |  64   |  49   | Dbb6  | C6    | B#5   |  0     |
/// |  85  |  65   |  50   | Db6   | C#6   |[B##5] |  1     |
/// |  86  |  66   |  51   | Ebb6  | D6    | C##6  |  2     |
/// |  87  |  67   |  52   |[Fbb6] | Eb6   | D#6   |  3     |
/// |  88  |  68   |  53   | Fb6   | E6    | D##6  |  4     |
/// |  89  |  69   |  54   | Gbb6  | F6    | E#6   |  5     |
/// |  90  |  70   |  55   | Gb6   | F#6   |[E##6] |  6     |
/// |  91  |  71   |  56   | Abb6  | G6    | F##6  |  7     |
/// |  92  |  72   |  57   | Ab6   | G#6   |       |  8     |
/// |  93  |  73   |  58   | Bbb6  | A6    | G##6  |  9     |
/// |  94  |  74   |  59   |[Cbb7] | Bb6   | A#6   | 10     |
/// |  95  |  75   |  60   | Cb7   | B6    | A##6  | 11     |
/// |  96  |  76   |  61   | Dbb7  | C7    | B#6   |  0     |
/// |  97  |  77   |       | Db7   | C#7   |[B##6] |  1     |
/// |  98  |  78   |       | Ebb7  | D7    | C##7  |  2     |
/// |  99  |  79   |       |[Fbb7] | Eb7   | D#7   |  3     |
/// | 100  |  80   |       | Fb7   | E7    | D##7  |  4     |
/// | 101  |  81   |       | Gbb7  | F7    | E#7   |  5     |
/// | 102  |  82   |       | Gb7   | F#7   |[E##7] |  6     |
/// | 103  |  83   |       | Abb7  | G7    | F##7  |  7     |
/// | 104  |  84   |       | Ab7   | G#7   |       |  8     |
/// | 105  |  85   |       | Bbb7  | A7    | G##7  |  9     |
/// | 106  |  86   |       |[Cbb8] | Bb7   | A#7   | 10     |
/// | 107  |  87   |       | Cb8   | B7    | A##7  | 11     |
/// | 108  |  88   |       | Dbb8  | C8    | B#7   |  0     |
/// | 109  |       |       | Db8   | C#8   |[B##7] |  1     |
/// | 110  |       |       | Ebb8  | D8    | C##8  |  2     |
/// | 111  |       |       |[Fbb8] | Eb8   | D#8   |  3     |
/// | 112  |       |       | Fb8   | E8    | D##8  |  4     |
/// | 113  |       |       | Gbb8  | F8    | E#8   |  5     |
/// | 114  |       |       | Gb8   | F#8   |[E##8] |  6     |
/// | 115  |       |       | Abb8  | G8    | F##8  |  7     |
/// | 116  |       |       | Ab8   | G#8   |       |  8     |
/// | 117  |       |       | Bbb8  | A8    | G##8  |  9     |
/// | 118  |       |       |[Cbb9] | Bb8   | A#8   | 10     |
/// | 119  |       |       | Cb9   | B8    | A##8  | 11     |
/// | 120  |       |       | Dbb9  | C9    | B#8   |  0     |
/// | 121  |       |       | Db9   | C#9   |[B##8] |  1     |
/// | 122  |       |       | Ebb9  | D9    | C##9  |  2     |
/// | 123  |       |       |[Fbb9] | Eb9   | D#9   |  3     |
/// | 124  |       |       | Fb9   | E9    | D##9  |  4     |
/// | 125  |       |       | Gbb9  | F9    | E#9   |  5     |
/// | 126  |       |       | Gb9   | F#9   |[E##9] |  6     |
/// | 127  |       |       | Abb9  | G9    | F##9  |  7     |
/// | 128  |       |       | Ab9   | G#9   |       |  8     |


/// fake class. a namespace with a private part.
class MidiNum
{
    
public:
       
    /// accidental for the given pitch class and given name.
    /// @param c a pitch class in 0..11.
    /// @param n a note name, must not be Undef.
    /// @return the accidental corresponding to c and n, or Undef if there is none.
    static enum Accid accid(int c, const enum NoteName& n);
    
    /// octave number for a given MIDI key and note name.
    /// @param m midi number.
    /// @param n note name in 'A'..'G'.
    /// @return octave number, in -2..9, for the note of given midi key and name.
    static int midi_to_octave(unsigned int m, const enum NoteName& n);

    /// octave number for a given MIDI key and note name,
    /// and debug check accidental.
    /// @param m midi number
    /// @param n note name in 'A'..'G'.
    /// @param a accidental in [-2, 2], where 1.0 is a half tone.
    /// given only for debugging (asserts).
    /// @param debug debug mode: the accidental is controlled. other it is ignored.
    /// @return octave number, in -2..9, for the note of given midi key, name
    /// and accidental.
    static int midi_to_octave(unsigned int m,
                              const enum NoteName& n,
                              const enum Accid& a,
                              bool debug = true);

    /// pitch class, in 0..11, of the given midi number.
    /// @param m midi number
    /// @return m modulo 12 or 12 in case of error.
    static unsigned int pitchClass(unsigned int m);
    
    /// pitch class, in 0..11, of the given note name (with natural accid).
    /// pitch class, in 0..11, of the given note name (with natural accid).
    /// @param n a note name in A..G. must not be Undef.
    /// @return the pitch class corresponding to the note name (natural),
    /// or 12 in case of error.
    /// The classes of C, D, E, F, G, A, B are resp.  0, 2, 4, 5, 7, 9, 11.
    static unsigned int pitchClass(const enum NoteName& n);
    
    /// pitch class, in 0..11, of the given note name with given accidental.
    /// @param n a note name in A..G. must not be Undef.
    /// @param a accidental in [-2, 2], where 1 is a half tone.
    /// @return the pitch class corresponding to the note name and accid,
    /// or 12 in case of error.
    static unsigned int pitchClass(const enum NoteName& n, const enum Accid& a);
    
    /// midi value corresponding to the given note name.
    /// @param n a note name in A..G. must not be Undef.
    /// @param a accidental in [-2, 2], where 1 is a half tone.
    /// @param o octave number, in -2..9.
    static unsigned int to_midi(const enum NoteName& n,
                                const enum Accid& a,
                                int o);
    
private:
    
    /// abbreviations for accidental triple flat
    static const enum Accid _3F;
    
    /// abbreviations for accidental double flat
    static const enum Accid _2F;
    
    /// abbreviations for accidental single flat
    static const enum Accid _1F;
    
    /// abbreviations for accidental natural
    static const enum Accid _0N;
    
    /// abbreviations for accidental single flat
    static const enum Accid _1S;
    
    /// abbreviations for accidental double flat
    static const enum Accid _2S;
    
    /// abbreviations for accidental triple flat
    static const enum Accid _3S;
    
    /// abbreviations for accidental undef
    static const enum Accid __U;
    
    /// table of accidentals for pitch class and name.
    static const enum Accid ACCID[12][7];
    
    // table of pitch class for each note name in 0 (C) .. 7 (B)
    // static const int PC[7];
    
}; // end MidiNum
        
//} // namespace MidiNum


} // namespace pse

#endif /* MidiNum_hpp */

/// @}
