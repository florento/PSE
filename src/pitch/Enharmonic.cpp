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
const PWO Enharmonics::get(int c, int i, bool cf, bool da)
{
    assert(0 <= c);
    assert(c <= 11);
    assert(0 <= i);
    assert(i <= 2);
    if (cf and da)
        return ENHARMONIC1b[c][i];
    else if (cf)
        return ENHARMONICb[c][i];
    else if (da)
        return ENHARMONIC1[c][i];
    else
        return ENHARMONIC[c][i];
}


// static
enum NoteName Enharmonics::name(int c, int i, bool cf, bool da)
{
    return get(c, i, cf, da).name;
}
  

// static
enum Accid Enharmonics::accid(int c, int i, bool cf, bool da)
{
    return get(c, i, cf, da).alteration;
}


// static abbreviations for note names
const enum NoteName Enharmonics::_C = NoteName::C;
const enum NoteName Enharmonics::_D = NoteName::D;
const enum NoteName Enharmonics::_E = NoteName::E;
const enum NoteName Enharmonics::_F = NoteName::F;
const enum NoteName Enharmonics::_G = NoteName::G;
const enum NoteName Enharmonics::_A = NoteName::A;
const enum NoteName Enharmonics::_B = NoteName::B;
const enum NoteName Enharmonics::_X = NoteName::Undef;

// static abbreviations for accidentals
const enum Accid Enharmonics::_2F = Accid::DoubleFlat;
const enum Accid Enharmonics::_1F = Accid::Flat;
const enum Accid Enharmonics::_0N = Accid::Natural;
const enum Accid Enharmonics::_1S = Accid::Sharp;
const enum Accid Enharmonics::_2S = Accid::DoubleSharp;
const enum Accid Enharmonics::_UA = Accid::Undef;

// static
const int Enharmonics::ENHARMONIC_UNDEF = 9;


// static
const PWO Enharmonics::CFLAT[4] =
{ {_C, _1F}, {_B, _1S}, {_F, _1F}, {_E, _1S} };


// static
bool Enharmonics::doublaccid(const enum Accid& a)
{
    return (a == _2F or a == _2S);
}


// static
const PWO Enharmonics::ENHARMONIC[12][3] =
{
    { {_D, _2F}, {_C, _0N}, {_B, _1S} }, //  0
    { {_D, _1F}, {_C, _1S}, {_B, _2S} }, //  1
    { {_E, _2F}, {_D, _0N}, {_C, _2S} }, //  2
    { {_F, _2F}, {_E, _1F}, {_D, _1S} }, //  3
    { {_F, _1F}, {_E, _0N}, {_D, _2S} }, //  4
    { {_G, _2F}, {_F, _0N}, {_E, _1S} }, //  5
    { {_G, _1F}, {_F, _1S}, {_E, _2S} }, //  6
    { {_A, _2F}, {_G, _0N}, {_F, _2S} }, //  7
    { {_A, _1F}, {_G, _1S}, {_X, _UA} }, //  8
    { {_B, _2F}, {_A, _0N}, {_G, _2S} }, //  9
    { {_C, _2F}, {_B, _1F}, {_A, _1S} }, // 10
    { {_C, _1F}, {_B, _0N}, {_A, _2S} }  // 11
};


// static
// short list without B#, Cb, E#, Fb
const PWO Enharmonics::ENHARMONICb[12][3] =
{
    { {_D, _2F}, {_C, _0N}, {_X, _UA} }, //  0  // _B, _1S
    { {_D, _1F}, {_C, _1S}, {_B, _2S} }, //  1  // _B, _2S
    { {_E, _2F}, {_D, _0N}, {_C, _2S} }, //  2
    { {_F, _2F}, {_E, _1F}, {_D, _1S} }, //  3  // _F, _2F
    { {_X, _UA}, {_E, _0N}, {_D, _2S} }, //  4  // _F, _1F
    { {_G, _2F}, {_F, _0N}, {_X, _UA} }, //  5  // _E, _1S
    { {_G, _1F}, {_F, _1S}, {_E, _2S} }, //  6  // _E, _2S
    { {_A, _2F}, {_G, _0N}, {_F, _2S} }, //  7
    { {_A, _1F}, {_G, _1S}, {_X, _UA} }, //  8
    { {_B, _2F}, {_A, _0N}, {_G, _2S} }, //  9
    { {_C, _2F}, {_B, _1F}, {_A, _1S} }, // 10  // _C, _2F
    { {_X, _UA}, {_B, _0N}, {_A, _2S} }  // 11  // _C, _1F
};


// static
// short list without double #, double b
const PWO Enharmonics::ENHARMONIC1[12][3] =
{
    { {_X, _UA}, {_C, _0N}, {_B, _1S} }, //  0
    { {_D, _1F}, {_C, _1S}, {_X, _UA} }, //  1
    { {_X, _UA}, {_D, _0N}, {_X, _UA} }, //  2
    { {_X, _UA}, {_E, _1F}, {_D, _1S} }, //  3
    { {_F, _1F}, {_E, _0N}, {_X, _UA} }, //  4
    { {_X, _UA}, {_F, _0N}, {_E, _1S} }, //  5
    { {_G, _1F}, {_F, _1S}, {_X, _UA} }, //  6
    { {_X, _UA}, {_G, _0N}, {_X, _UA} }, //  7
    { {_A, _1F}, {_G, _1S}, {_X, _UA} }, //  8
    { {_X, _UA}, {_A, _0N}, {_X, _UA} }, //  9
    { {_X, _UA}, {_B, _1F}, {_A, _1S} }, // 10
    { {_C, _1F}, {_B, _0N}, {_X, _UA} }  // 11
};


// static
// very short list without double #, double b
// and without B#, Cb, E#, Fb
const PWO Enharmonics::ENHARMONIC1b[12][3] =
{
    { {_X, _UA}, {_C, _0N}, {_X, _UA} }, //  0
    { {_D, _1F}, {_C, _1S}, {_X, _UA} }, //  1
    { {_X, _UA}, {_D, _0N}, {_X, _UA} }, //  2
    { {_X, _UA}, {_E, _1F}, {_D, _1S} }, //  3
    { {_X, _UA}, {_E, _0N}, {_X, _UA} }, //  4
    { {_X, _UA}, {_F, _0N}, {_X, _UA} }, //  5
    { {_G, _1F}, {_F, _1S}, {_X, _UA} }, //  6
    { {_X, _UA}, {_G, _0N}, {_X, _UA} }, //  7
    { {_A, _1F}, {_G, _1S}, {_X, _UA} }, //  8
    { {_X, _UA}, {_A, _0N}, {_X, _UA} }, //  9
    { {_X, _UA}, {_B, _1F}, {_A, _1S} }, // 10
    { {_X, _UA}, {_B, _0N}, {_X, _UA} }  // 11
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
