//
//  NoteName.hpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#ifndef NoteName_hpp
#define NoteName_hpp

#include <iostream>
#include <assert.h>

#include "trace.hpp"


namespace pse {

/// Values of Note names
/// from `A` to `G` and `Undef` (undefined or unpitched)
/// @see https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/step/class NoteName
enum class NoteName
{
    C,
    D,
    E,
    F,
    G,
    A,
    B,
    Undef
};

///  cast to 0..6 (0 is 'C', 6 is 'B'), or 9 for `Undef`.
///  only for internal use.
int toint(const NoteName& n);

/// cast to `A` to `G` or 'X' for `Undef`.
///  only for internal use.
char tochar(const NoteName& n);

/// the given note name is between 'A' and 'G'
bool defined(const NoteName& n);

/// the distance between the 2 note names is diatonic
bool diatonicStep(const NoteName& n1, const NoteName& n2);

/// constructor of note name
/// @param c char between 'A' and 'G' or 'X' for Undef.
/// @todo TBR obsolete
NoteName NNofchar(char c);

/// constructor of note name
/// @param i int between 0 ('C') and 6 ('B') or 9 for Undef.
NoteName NNofint(int i);
    
std::ostream& operator<<(std::ostream& o, const NoteName& c);


} // namespace pse

#endif /* NoteName_hpp */

/// @}
