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

#include "pstrace.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "PWO.hpp"  // pitch without octave

namespace pse {


/// for each pitch class (MIDI key modulo 12),
/// there are 2 or 3 enharmonics, described in the following table:
/// | pitch  | name1 | name2 | name3 |
/// |:------:|:------|:------|:------|
/// |   0    |  Dbb  |  C    |  B#   |
/// |   1    |  Db   |  C#   | [B##] |
/// |   2    |  Ebb  |  D    |  C##  |
/// |   3    | [Fbb] |  Eb   |  D#   |
/// |   4    |  Fb   |  E    |  D##  |
/// |   5    |  Gbb  |  F    |  E#   |
/// |   6    |  Gb   |  F#   | [E##] |
/// |   7    |  Abb  |  G    |  F##  |
/// |   8    |  Ab   |  G#   |       |
/// |   9    |  Bbb  |  A    |  G##  |
/// |  10    | [Cbb] |  Bb   |  A#   |
/// |  11    |  Cb   |  B    |  A##  |
/// @see NoteName for the encoding of note names
/// @see Accid for the encoding of accidentals
/// @see MidiNum for a complete table
class Enharmonics
{
public:
    
    /// name of the ith entry for pitch a class.
    /// @param c a pitch class in 0..11
    /// @param i an index of enharmonics in 0..2
    /// @param cf whether Cb, B#, Fb, E# are forbidden.
    /// @param da whether double flats ans sharps are forbidden.
    /// @return a note name in 'A' .. 'G'
    /// or Undef is there is no enharmonic of index i.
    static enum NoteName name(int c, int i, bool cf=false, bool da=false);
      
    /// accidental of the ith entry for pitch a class.
    /// @param c a pitch class in 0..11
    /// @param i an index of enharmonics in 0..2
    /// @param cf whether Cb, B#, Fb, E# are forbidden.
    /// @param da whether double flats ans sharps are forbidden.
    /// @return an alteration number in -2..2
    /// or Undef is there is no enharmonic of index i.
    static enum Accid accid(int c, int i, bool cf=false, bool da=false);
    
private:

    /// abbreviations for note names
    static const enum NoteName _C;
    static const enum NoteName _D;
    static const enum NoteName _E;
    static const enum NoteName _F;
    static const enum NoteName _G;
    static const enum NoteName _A;
    static const enum NoteName _B;
    static const enum NoteName _X;

    /// abbreviations for accidentals
    static const enum Accid _2F;
    static const enum Accid _1F;
    static const enum Accid _0N;
    static const enum Accid _1S;
    static const enum Accid _2S;
    static const enum Accid _UA;

    /// Cflat, Bsharp, Fflat, Esharp
    static const PWO CFLAT[4];

    /// double flat or double sharp
    static bool doublaccid(const enum Accid& a);
    
    /// constant for the tables
    static const int ENHARMONIC_UNDEF;
    
    /// The table ENHARMONIC contains for each pitch class in 0..11
    /// three synonyms in 0..2 presented with 2 values :
    /// - name in 0..6 for C, D, E, F, G, A, B (see table NAME) and
    /// - accidental in -2..2
    /// In the cases where there are only 2 synonyms (e.g. pitch = 8),
    /// both components are set to ENHARMONIC_UNDEF.
    static const PWO ENHARMONIC[12][3];
    
    /// short list without double #, double b
    static const PWO ENHARMONIC1[12][3];

    /// short list without B#, Cb, E#, Fb
    static const PWO ENHARMONICb[12][3];

    /// very short list without double #, double b and without B#, Cb, E#, Fb
    static const PWO ENHARMONIC1b[12][3];

};
    
// same as ENHARMONIC with flats first
// static const int ENHARMONICF[12][3][2];


} // namespace pse

#endif /* Enharmonic_hpp */

/// @}


