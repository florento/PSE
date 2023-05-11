//
//  Fifths.cpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


//#include "Pitch.hpp"
#include "Fifths.hpp"


namespace pse {

// static
enum NoteName Fifths::name(int i)
{
    assert(-15 <= i);
    assert(i <= 19);

    return FIFTHS[i+15].name;
}


// static
enum Accid Fifths::accid(int i)
{
    assert(-15 <= i);
    assert(i <= 19);

    return FIFTHS[i+15].alteration;
    //assert(-2 <= a);
    //assert(a <= 2);
    //return accidofint(a);
}


// static
int Fifths::pc(int i)
{
    assert(-15 <= i);
    assert(i <= 19);

    return PCFIFTHS[i+15];
}


// static
int Fifths::index(const enum NoteName& name, const enum Accid& accid)
{
    int mod = 11;
    for (int i = 0; i < 7; ++i)
    {
        if (MODI[i] == name)
        {
            mod = i;
            break;
        }
    }
    assert(0 <= mod);
    assert(mod <= 6);
    int a = toint(accid);
    assert(-2 <= a);
    assert(a <= 2);
    int div = a+2;
    return ((div * 7) + mod) - 15;
}


// static
const enum NoteName Fifths::MODI[7] = { _F, _C, _G, _D, _A, _E, _B };

//const char Fifths::MODC[7] = { 'F', 'C', 'G', 'D', 'A', 'E', 'B' };

// abbreviations for note names
const enum NoteName Fifths::_C = NoteName::C;
const enum NoteName Fifths::_D = NoteName::D;
const enum NoteName Fifths::_E = NoteName::E;
const enum NoteName Fifths::_F = NoteName::F;
const enum NoteName Fifths::_G = NoteName::G;
const enum NoteName Fifths::_A = NoteName::A;
const enum NoteName Fifths::_B = NoteName::B;
const enum NoteName Fifths::_X = NoteName::Undef;

// abbreviations for accidentals
const enum Accid Fifths::_2F = Accid::DoubleFlat;
const enum Accid Fifths::_1F = Accid::Flat;
const enum Accid Fifths::_0N = Accid::Natural;
const enum Accid Fifths::_1S = Accid::Sharp;
const enum Accid Fifths::_2S = Accid::DoubleSharp;

const PWO Fifths::FIFTHS[35] =
{
    {_F, _2F}, // -15  // Fbb
    {_C, _2F}, // -14  // Cbb
    {_G, _2F}, // -13  // Gbb
    {_D, _2F}, // -12  // Dbb
    {_A, _2F}, // -11  // Abb
    {_E, _2F}, // -10  // Ebb
    {_B, _2F}, //  -9  // Bbb
    {_F, _1F}, //  -8  // Fb
    {_C, _1F}, //  -7  // Cb
    {_G, _1F}, //  -6  // Gb
    {_D, _1F}, //  -5  // Db
    {_A, _1F}, //  -4  // Ab
    {_E, _1F}, //  -3  // Eb
    {_B, _1F}, //  -2  // Bb
    {_F, _0N}, //  -1  // F
    {_C, _0N}, //   0  // C
    {_G, _0N}, //   1  // G
    {_D, _0N}, //   2  // D
    {_A, _0N}, //   3  // A
    {_E, _0N}, //   4  // E
    {_B, _0N}, //   5  // B
    {_F, _1S}, //   6  // F#
    {_C, _1S}, //   7  // C#
    {_G, _1S}, //   8  // G#
    {_D, _1S}, //   9  // D#
    {_A, _1S}, //  10  // A#
    {_E, _1S}, //  11  // E#
    {_B, _1S}, //  12  // B#
    {_F, _2S}, //  13  // F##
    {_C, _2S}, //  14  // C##
    {_G, _2S}, //  15  // G##
    {_D, _2S}, //  16  // D##
    {_A, _2S}, //  17  // A##
    {_E, _2S}, //  18  // E##
    {_B, _2S}  //  19  // B##
};

const int Fifths::PCFIFTHS[35] =
{
     3, // -15  // Fbb
    10, // -14  // Cbb
     5, // -13  // Gbb
     0, // -12  // Dbb
     7, // -11  // Abb
     2, // -10  // Ebb
     9, //  -9  // Bbb
     4, //  -8  // Fb
    11, //  -7  // Cb
     6, //  -6  // Gb
     1, //  -5  // Db
     8, //  -4  // Ab
     3, //  -3  // Eb
    10, //  -2  // Bb
     5, //  -1  // F
     0, //   0  // C
     7, //   1  // G
     2, //   2  // D
     9, //   3  // A
     4, //   4  // E
    11, //   5  // B
     6, //   6  // F#
     1, //   7  // C#
     8, //   8  // G#
     3, //   9  // D#
    10, //  10  // A#
     5, //  11  // E#
     0, //  12  // B#
     7, //  13  // F##
     2, //  14  // C##
     9, //  15  // G##
     4, //  16  // D##
    11, //  17  // A##
     6, //  18  // E##
     1  //  19  // B##
};



// static
//int Fifths::iname(int i)
//{
//    assert(-15 <= i);
//    assert(i <= 19);
//
//    int n = FIFTHS[i+15][0];
//    assert(0 <= n);
//    assert(n <= 6);
//    return n;
//}


// static
//const int Fifths::FIFTHS[35][2] =
//{
//    {3, -2}, // -15  // Fbb
//    {0, -2}, // -14  // Cbb
//    {4, -2}, // -13  // Gbb
//    {1, -2}, // -12  // Dbb
//    {5, -2}, // -11  // Abb
//    {2, -2}, // -10  // Ebb
//    {6, -2}, //  -9  // Bbb
//    {3, -1}, //  -8  // Fb
//    {0, -1}, //  -7  // Cb
//    {4, -1}, //  -6  // Gb
//    {1, -1}, //  -5  // Db
//    {5, -1}, //  -4  // Ab
//    {2, -1}, //  -3  // Eb
//    {6, -1}, //  -2  // Bb
//    {3,  0}, //  -1  // F
//    {0,  0}, //   0  // C
//    {4,  0}, //   1  // G
//    {1,  0}, //   2  // D
//    {5,  0}, //   3  // A
//    {2,  0}, //   4  // E
//    {6,  0}, //   5  // B
//    {3,  1}, //   6  // F#
//    {0,  1}, //   7  // C#
//    {4,  1}, //   8  // G#
//    {1,  1}, //   9  // D#
//    {5,  1}, //  10  // A#
//    {2,  1}, //  11  // E#
//    {6,  1}, //  12  // B#
//    {3,  2}, //  13  // F##
//    {0,  2}, //  14  // C##
//    {4,  2}, //  15  // G##
//    {1,  2}, //  16  // D##
//    {5,  2}, //  17  // A##
//    {2,  2}, //  18  // E##
//    {6,  2}, //  19  // B##
//};


//const std::map<std::pair<int,int>, int> FIFTHS =
//{
//    {{3, -2}, -15},  // Fbb
//    {{0, -2}, -14},  // Cbb
//    {{4, -2}, -13},  // Gbb
//    {{1, -2}, -12},  // Dbb
//    {{5, -2}, -11},  // Abb
//    {{2, -2}, -10},  // Ebb
//    {{6, -2},  -9},  // Bbb
//    {{3, -1},  -8},  // Fb
//    {{0, -1},  -7 }, // Cb
//    {{4, -1},  -6 }, // Gb
//    {{1, -1},  -5 }, // Db
//    {{5, -1},  -4 }, // Ab
//    {{2, -1},  -3 }, // Eb
//    {{6, -1},  -2 }, // Bb
//    {{3,  0},  -1 }, // F
//    {{0,  0},   0 }, // C
//    {{4,  0},   1},  // G
//    {{1,  0},   2},  // D
//    {{5,  0},   3},  // A
//    {{2,  0},   4},  // E
//    {{6,  0},   5},  // B
//    {{3,  1},   6},  // F#
//    {{0,  1},   7},  // C#
//    {{4,  1},   8},  // G#
//    {{1,  1},   9},  // D#
//    {{5,  1},  10},  // A#
//    {{2,  1},  11},  // E#
//    {{6,  1},  12},  // B#
//    {{3,  2},  13},  // F##
//    {{0,  2},  14},  // C##
//    {{4,  2},  15},  // G##
//    {{1,  2},  16},  // D##
//    {{5,  2},  17},  // A##
//    {{2,  2},  18},  // E##
//    {{6,  2},  19}   // B##
//};


// static
//int Fifths::index(char name, int alt)
//{
//    assert('A' <= name);
//    assert(name <= 'G');
//    int mod = 11;
//    for (int i = 0; i < 7; ++i)
//    {
//        if (MODC[i] == name)
//        {
//            mod = i;
//            break;
//        }
//    }
//    assert(0 <= mod);
//    assert(mod <= 6);
//    assert(-2 <= alt);
//    assert(alt <= 2);
//    int div = alt+2;
//
//    return ((div * 7) + mod) - 15;
//}


} // end namespace pse

/// @}
