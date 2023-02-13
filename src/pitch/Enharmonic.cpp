//
//  Enharmonic.cpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#include "Enharmonic.hpp"



namespace pse {

// static
NoteName Enharmonics::name(int c, int i)
{
    assert(0 <= c);
    assert(c <= 11);
    assert(0 <= i);
    assert(i <= 2);
    return ENHARMONIC[c][i].name;
}
  

// static
Accid Enharmonics::accid(int c, int i)
{
    assert(0 <= c);
    assert(c <= 11);
    assert(0 <= i);
    assert(i <= 2);
    return ENHARMONIC[c][i].alteration;
}


// static abbreviations for note names
const NoteName Enharmonics::_C = NoteName::C;
const NoteName Enharmonics::_D = NoteName::D;
const NoteName Enharmonics::_E = NoteName::E;
const NoteName Enharmonics::_F = NoteName::F;
const NoteName Enharmonics::_G = NoteName::G;
const NoteName Enharmonics::_A = NoteName::A;
const NoteName Enharmonics::_B = NoteName::B;
const NoteName Enharmonics::_X = NoteName::Undef;

// static abbreviations for accidentals
const Accid Enharmonics::_2F = Accid::DoubleFlat;
const Accid Enharmonics::_1F = Accid::Flat;
const Accid Enharmonics::_0N = Accid::Natural;
const Accid Enharmonics::_1S = Accid::Sharp;
const Accid Enharmonics::_2S = Accid::DoubleSharp;
const Accid Enharmonics::_UA = Accid::Undef;

// static
const int Enharmonics::ENHARMONIC_UNDEF = 9;

// static
const PWO Enharmonics::ENHARMONIC[12][3] =
{
    { {_D, _2F}, {_C, _0N}, {_B,_1S} }, //  0
    { {_D, _1F}, {_C, _1S}, {_B,_2S} }, //  1
    { {_E, _2F}, {_D, _0N}, {_C,_2S} }, //  2
    { {_F, _2F}, {_E, _1F}, {_D,_1S} }, //  3
    { {_F, _1F}, {_E, _0N}, {_D,_2S} }, //  4
    { {_G, _2F}, {_F, _0N}, {_E,_1S} }, //  5
    { {_G, _1F}, {_F, _1S}, {_E,_2S} }, //  6
    { {_A, _2F}, {_G, _0N}, {_F,_2S} }, //  7
    { {_A, _1F}, {_G, _1S}, {_X,_UA} }, //  8
    { {_B, _2F}, {_A, _0N}, {_G,_2S} }, //  9
    { {_C, _2F}, {_B, _1F}, {_A,_1S} }, // 10
    { {_C, _1F}, {_B, _0N}, {_A,_2S} }  // 11
};


// static
// short list without B##, Fbb, E##, Cbb
const PWO Enharmonics::ENHARMONICSHORT[12][3] =
{
    { {_D, _2F}, {_C, _0N}, {_B,_1S} }, //  0
    { {_D, _1F}, {_C, _1S}, {_X,_UA} }, //  1
    { {_E, _2F}, {_D, _0N}, {_C,_2S} }, //  2
    { {_X, _UA}, {_E, _1F}, {_D,_1S} }, //  3
    { {_F, _1F}, {_E, _0N}, {_D,_2S} }, //  4
    { {_G, _2F}, {_F, _0N}, {_E,_1S} }, //  5
    { {_G, _1F}, {_F, _1S}, {_X,_UA} }, //  6
    { {_A, _2F}, {_G, _0N}, {_F,_2S} }, //  7
    { {_A, _1F}, {_G, _1S}, {_X,_UA} }, //  8
    { {_B, _2F}, {_A, _0N}, {_G,_2S} }, //  9
    { {_C, _2F}, {_B, _1F}, {_A,_1S} }, // 10
    { {_C, _1F}, {_B, _0N}, {_A,_2S} }  // 11
};


// second version with different order (useless)
//const PWO ENHARMONICF[12][3] =
//{
//    { {_B,_1S}, {_C, _0N}, { 1, _2F} }, //  0
//    { {_C,_1S}, {_D, _1F}, { 6, _2S} }, //  1
//    { {_C,_2S}, {_D, _0N}, { 2, _2F} }, //  2
//    { {_D,_1S}, {_E, _1F}, { 3, _2F} }, //  3
//    { {_D,_2S}, {_E, _0N}, { 3, _1F} }, //  4
//    { {_E,_1S}, {_F, _0N}, { 4, _2F} }, //  5
//    { {_E,_2S}, {_F, _1S}, { 4, _1F} }, //  6
//    { {_F,_2S}, {_G, _0N}, { 5, _2F} }, //  7
//    { {_G,_1S}, {_A, _1F}, {-1, _0N} }, //  8
//    { {_G,_2S}, {_A, _0N}, { 6, _2F} }, //  9
//    { {_A,_1S}, {_B, _1F}, { 0, _2F} }, // 10
//    { {_A,_2S}, {_B, _0N}, { 0, _1F} }  // 11
//};

} // end namespace pse

/// @}
