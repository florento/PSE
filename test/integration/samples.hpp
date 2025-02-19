//
//  samples.hpp
//  debugpse
//
//  Created by Florent Jacquemard on 28/06/2023.
//

#ifndef samples_hpp
#define samples_hpp

#include <iostream>

#include "PSE.hpp"
#include "PS13.hpp"
#include "PS14.hpp"


void issue7(pse::SpellerEnum& sp);

// LG
void LG101(pse::SpellerEnum& sp);
void LG102(pse::SpellerEnum& sp);
void LG128(pse::SpellerEnum& sp);
void LG228(pse::SpellerEnum& sp);
void LG275(pse::SpellerEnum& sp);
void LG349(pse::SpellerEnum& sp);
void LG405(pse::SpellerEnum& sp);
void LG461(pse::SpellerEnum& sp);
void LG470(pse::SpellerEnum& sp);

// ASAP Bach
void BWV_857prelude(pse::SpellerEnum& sp);
void BWV_857fugue(pse::SpellerEnum& sp);
void BWV_858prelude(pse::SpellerEnum& sp);
void BWV_864fugue(pse::SpellerEnum& sp);
void BWV_875prelude(pse::SpellerEnum& sp);

// ASAP
void Waldstein(pse::SpellerEnum& sp);

// FRB
void Airegin(pse::SpellerEnum& sp);
void Afternoon(pse::SpellerEnum& sp);

// Omnibook
void vRfYc(pse::SpellerEnum& sp);
void FRfYc(pse::SpellerEnum& sp);



#endif /* samples_hpp */
