//
//  MidiNum.cpp
//  qparse
//
//  Created by Florent Jacquemard on 16/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "MidiNum.hpp"

namespace pse {

//namespace MidiNum {

// static
int midi_to_octave(unsigned int m,
                   const enum pse::NoteName& n, const enum pse::Accid& a)
{
    assert(0 <= m);
    assert(m <= 128);
    int oct = int(floor(m/12)) - 1;
    assert(-1 <= oct);
    assert(oct <= 9);
    int chroma = m % 12;
    // adjust octave for extreme notes
    if (chroma == 0 && n == pse::NoteName::B)
    {
        assert(a == pse::Accid::Sharp);
        return oct - 1;
    }
    else if (chroma == 1 && n == pse::NoteName::B)
    {
        assert(a == pse::Accid::DoubleSharp);
        return oct - 1;
    }
    else if (chroma == 10 && n == pse::NoteName::C)
    {
        assert(a == pse::Accid::DoubleFlat);
        return oct + 1;
    }
    else if (chroma == 11 && n == pse::NoteName::C)
    {
        assert(a == pse::Accid::Flat);
        return oct + 1;
    }
    else
    {
        // TBD assert the name // accid in the other cases
        return oct;
    }
}

//} // namespace MidiNum

} // end namespace pse
