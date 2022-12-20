//
//  Enharmonic.hpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef Enharmonic_hpp
#define Enharmonic_hpp

#include <iostream>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "PWO.hpp"  // pitch without octave

namespace pse {


/// for each pitch class (MIDI pitch modulo 12),
/// there are 2 or 3 enharmonics, described in the following table:
/// | pitch  |       |    |       |
/// |:------:|:------|:---|:------|
/// |   0    |  Dbb  | C  |  B#   |
/// |   1    |  Db   | C# | [B##] |
/// |   2    |  Ebb  | D  |  C##  |
/// |   3    | [Fbb] | Eb |  D#   |
/// |   4    |  Fb   | E  |  D##  |
/// |   5    |  Gbb  | F  |  E#   |
/// |   6    |  Gb   | F# | [E##] |
/// |   7    |  Abb  | G  |  F##  |
/// |   8    |  Ab   | G# |       |
/// |   9    |  Bbb  | A  |  G##  |
/// |  10    | [Cbb] | Bb |  A#   |
/// |  11    |  Cb   | B  |  A##  |
class Enharmonics
{
public:
    
    /// name of the ith entry for pitch a class.
    /// @param c a pitch class in 0..11
    /// @param i an index of enharmonics in 0..2
    /// @return a note name in 'A' .. 'G'
    /// or 'X' (undef) is there is no enharmonic of index i.
    static NoteName name(int c, int i);
      
    /// accidental of the ith entry for pitch a class.
    /// @param c a pitch class in 0..11
    /// @param i an index of enharmonics in 0..2
    /// @return an alteration number in -2..2.
    static Accid accid(int c, int i);
    
private:

    /// abbreviations for note names
    static const NoteName _C;
    static const NoteName _D;
    static const NoteName _E;
    static const NoteName _F;
    static const NoteName _G;
    static const NoteName _A;
    static const NoteName _B;
    static const NoteName _X;

    /// abbreviations for accidentals
    static const Accid _2F;
    static const Accid _1F;
    static const Accid _0N;
    static const Accid _1S;
    static const Accid _2S;
    static const Accid _UA;

    static const int ENHARMONIC_UNDEF;

    /// The table ENHARMONIC contains for each pitch class in 0..11
    /// three synonyms in 0..2 presented with 2 values :
    /// - name in 0..6 for C, D, E, F, G, A, B (see table NAME) and
    /// - accidental in -2..2
    /// In the cases where there are only 2 synonyms (e.g. pitch = 8),
    /// both components are set to ENHARMONIC_UNDEF.
    static const PWO ENHARMONIC[12][3];
    
    /// short list for ENHARMONIC
    static const PWO ENHARMONICSHORT[12][3];
    
};
    
// same as ENHARMONIC with flats first
// static const int ENHARMONICF[12][3][2];


} // namespace pse

#endif /* Enharmonic_hpp */

/// @}


