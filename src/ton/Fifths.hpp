//
//  Fifths.hpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef Fifths_hpp
#define Fifths_hpp

#include <iostream>

#include "trace.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "PWO.hpp"

namespace pse {


/// array of fifths, with double sharps and flats.
/// every note is presented with 2 values :
/// - name in 0..6 for C, D, E, F, G, A, B (see table Pitch::name_to_int)
/// - alteration in -2..2
///
/// Fbb Cbb Gbb Dbb Abb Ebb Bbb Fb Cb Gb Db Ab Eb Bb
/// F C G D A E B F# C# G# D# A# E# B# F## C## G## D## A## E## B##
///
/// @see NoteName for the encoding of note names
/// @see Accid for the encoding of accidentals
class Fifths
{
public:
    
    /// name of the ith entry in the array of fifth.
    /// @param i index in the array of fifth, between -15 and 19 (0 is 'C').
    /// @return a note name in 'A' .. 'G'.
    static enum NoteName name(int i);
      
    /// alterations for the ith entry in the array of fifth.
    /// @param i index in the array of fifth, between -15 and 19 (0 is 'C').
    /// @return an alteration number in -2..2.
    static enum Accid accid(int i);

    /// pitch class for the ith entry in the array of fifth.
    /// @param i index in the array of fifth, between -15 and 19 (0 is 'C').
    /// @return a pitch class in 0..11.
    static int pc(int i);

    /// index in the array of fifth of the note with given name and alteration.
    /// @param n a note name in 0..6 for C, D, E, F, G, A, B
    /// @param a an alteration number in -2..2.
    /// @return index in the array of fifth, between -15 and 19 (0 is 'C').
    static int index(const enum NoteName& n, const enum Accid& a);
    
    // index in the array of fifth of the note with given name and alteration.
    // @param name a note name in 'A' .. 'G'.
    // @param alt an alteration number in -2..2.
    // @return index in the array of fifth, between -15 and 19 (0 is 'C').
    // static int index(char name, int alt);
    
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

    //static const int FIFTHS[35][2];
    /// PWO values (name, accidental) in the line of fifths from -15 to 19.
    static const PWO FIFTHS[35];

    /// pitch class value (in 0..11) in the line of fifths from -15 to 19.
    static const int PCFIFTHS[35];

    static const enum NoteName MODI[7];
    
    // static const char MODC[7];
    
    /// name of the ith entry in the array of fifth.
    /// @param i index in the array of fifth, between -15 and 19 (0 is 'C').
    /// @return a note name in 0..6 for C, D, E, F, G, A, B
    /// @see table Pitch::name_to_int
    //static int iname(int i);
    
};

} // namespace pse

#endif /* Fifths_hpp */

/// @}
