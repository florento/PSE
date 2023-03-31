//
//  TestMidiNum.cpp
//  testpse
//
//  Created by Florent Jacquemard on 31/03/2023.
//

#include "gtest/gtest.h"

#include "MidiNum.hpp"


// see table in MidiNum.hpp
TEST(MidiNum, midi_to_octave)
{
    EXPECT_EQ(pse::MidiNum::midi_to_octave(0, pse::NoteName::D), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(0, pse::NoteName::C), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(0, pse::NoteName::B), -2);
    
    EXPECT_EQ(pse::MidiNum::midi_to_octave(1, pse::NoteName::D), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(1, pse::NoteName::C), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(1, pse::NoteName::B), -2);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(2, pse::NoteName::E), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(2, pse::NoteName::D), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(2, pse::NoteName::C), -1);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(10, pse::NoteName::C), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(10, pse::NoteName::B), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(10, pse::NoteName::A), -1);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(11, pse::NoteName::C), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(11, pse::NoteName::B), -1);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(11, pse::NoteName::A), -1);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(12, pse::NoteName::D), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(12, pse::NoteName::C), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(12, pse::NoteName::B), -1);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(13, pse::NoteName::D), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(13, pse::NoteName::C), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(13, pse::NoteName::B), -1);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(14, pse::NoteName::E), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(14, pse::NoteName::D), 0);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(14, pse::NoteName::C), 0);
 
    EXPECT_EQ(pse::MidiNum::midi_to_octave(108, pse::NoteName::D), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(108, pse::NoteName::C), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(108, pse::NoteName::B), 7);
    
    EXPECT_EQ(pse::MidiNum::midi_to_octave(109, pse::NoteName::D), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(109, pse::NoteName::C), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(109, pse::NoteName::B), 7);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(110, pse::NoteName::E), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(110, pse::NoteName::D), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(110, pse::NoteName::C), 8);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(114, pse::NoteName::G), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(114, pse::NoteName::F), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(114, pse::NoteName::E), 8);
    
    EXPECT_EQ(pse::MidiNum::midi_to_octave(118, pse::NoteName::C), 9);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(118, pse::NoteName::B), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(118, pse::NoteName::A), 8);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(119, pse::NoteName::C), 9);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(119, pse::NoteName::B), 8);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(119, pse::NoteName::A), 8);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(120, pse::NoteName::D), 9);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(120, pse::NoteName::C), 9);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(120, pse::NoteName::B), 8);

    EXPECT_EQ(pse::MidiNum::midi_to_octave(121, pse::NoteName::D), 9);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(121, pse::NoteName::C), 9);
    EXPECT_EQ(pse::MidiNum::midi_to_octave(121, pse::NoteName::B), 8);

}


TEST(MidiNum, accid)
{
    //   0 = C
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::C), pse::Accid::Natural);
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::D), pse::Accid::DoubleFlat);
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::E), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::F), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::G), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::A), pse::Accid::TripleSharp);
    EXPECT_EQ(pse::MidiNum::accid(0, pse::NoteName::B), pse::Accid::Sharp);

    //  1 = C#, Db
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::C), pse::Accid::Sharp);
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::D), pse::Accid::Flat);
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::E), pse::Accid::TripleFlat);
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::F), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::G), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::A), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(1, pse::NoteName::B), pse::Accid::DoubleSharp);

    //  2 = D
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::C), pse::Accid::DoubleSharp);
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::D), pse::Accid::Natural);
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::E), pse::Accid::DoubleFlat);
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::F), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::G), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::A), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(2, pse::NoteName::B), pse::Accid::TripleSharp);

    //  3 = D#, Eb
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::C), pse::Accid::TripleSharp);
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::D), pse::Accid::Sharp);
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::E), pse::Accid::Flat);
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::F), pse::Accid::DoubleFlat);
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::G), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::A), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(3, pse::NoteName::B), pse::Accid::Undef);

    //  4 = E
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::C), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::D), pse::Accid::DoubleSharp);
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::E), pse::Accid::Natural);
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::F), pse::Accid::Flat);
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::G), pse::Accid::TripleFlat);
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::A), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(4, pse::NoteName::B), pse::Accid::Undef);

    //  5 = F
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::C), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::D), pse::Accid::TripleSharp);
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::E), pse::Accid::Sharp);
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::F), pse::Accid::Natural);
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::G), pse::Accid::DoubleFlat);
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::A), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(5, pse::NoteName::B), pse::Accid::Undef);

    //  6 = F#, Gb
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::C), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::D), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::E), pse::Accid::DoubleSharp);
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::F), pse::Accid::Sharp);
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::G), pse::Accid::Flat);
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::A), pse::Accid::TripleFlat);
    EXPECT_EQ(pse::MidiNum::accid(6, pse::NoteName::B), pse::Accid::Undef);

    //  7 = G
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::C), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::D), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::E), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::F), pse::Accid::DoubleSharp);
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::G), pse::Accid::Natural);
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::A), pse::Accid::DoubleFlat);
    EXPECT_EQ(pse::MidiNum::accid(7, pse::NoteName::B), pse::Accid::Undef);

    //  8 = G#, Ab
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::C), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::D), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::E), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::F), pse::Accid::TripleSharp);
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::G), pse::Accid::Sharp);
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::A), pse::Accid::Flat);
    EXPECT_EQ(pse::MidiNum::accid(8, pse::NoteName::B), pse::Accid::TripleFlat);

    //  9 = A
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::C), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::D), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::E), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::F), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::G), pse::Accid::DoubleSharp);
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::A), pse::Accid::Natural);
    EXPECT_EQ(pse::MidiNum::accid(9, pse::NoteName::B), pse::Accid::DoubleFlat);

    // 10 = A#, Bb
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::C), pse::Accid::DoubleFlat);
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::D), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::E), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::F), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::G), pse::Accid::TripleSharp);
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::A), pse::Accid::Sharp);
    EXPECT_EQ(pse::MidiNum::accid(10, pse::NoteName::B), pse::Accid::Flat);

    // 11 = B
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::C), pse::Accid::Flat);
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::D), pse::Accid::TripleFlat);
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::E), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::F), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::G), pse::Accid::Undef);
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::A), pse::Accid::DoubleSharp);
    EXPECT_EQ(pse::MidiNum::accid(11, pse::NoteName::B), pse::Accid::Natural);
}


TEST(MidiNum, pitchClass)
{
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::C), 0);
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::D), 2);
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::E), 4);
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::F), 5);
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::G), 7);
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::A), 9);
    EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::B), 11);
    // ERROR msg
    // EXPECT_EQ(pse::MidiNum::pitchClass(pse::NoteName::Undef), 12);
}

// test that to_midi is the inverse of the above tested
TEST(MidiNum, to_midi)
{
    for (int m = 0; m < 128; ++m)
    {
        // pitch class
        int c = m % 12;
        for (auto n : { pse::NoteName::C, pse::NoteName::D, pse::NoteName::E,
                        pse::NoteName::F, pse::NoteName::C, pse::NoteName::C,
                        pse::NoteName::B })
        {
            // has been tested
            enum pse::Accid a = pse::MidiNum::accid(c, n);
            if (a != pse::Accid::Undef)
            {
                // has been tested
                int o = pse::MidiNum::midi_to_octave(m, n);
                EXPECT_EQ(pse::MidiNum::to_midi(n, a, o), m);
            }
        }
    }
}



