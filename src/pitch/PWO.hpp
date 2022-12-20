//
//  PWO.hpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#ifndef PWO_hpp
#define PWO_hpp

#include <iostream>

#include "NoteName.hpp"
#include "Accidental.hpp"

namespace pse {


/// Pitch without octave
struct PWO
{
public:
    pse::NoteName name;
    pse::Accid    accid;
};


} // namespace pse

#endif /* PWO_hpp */

/// @}

