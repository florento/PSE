//
//  WeberModal_static.cpp
//  pse
//
//  Created by Florent Jacquemard on 04/03/2024.
//

#include "WeberModal_static.hpp"

namespace pse {



// static
size_t WeberModal_static::index(const Ton& ton)
{
    if (ton.getMode() == ModeName::Major)
    {
        return ton.fifths() + 7;
    }
    else if (ton.getMode() == ModeName::Minor)
    {
        return ton.fifths() + 22;
    }
    else if (ton.getMode() == ModeName::MinorMel)
    {
        return ton.fifths() + 37;
    }
    else if (ton.getMode() == ModeName::Dorian)
    {
        return ton.fifths() + 52;
    }
    else if (ton.getMode() == ModeName::Phrygian)
    {
        return ton.fifths() + 67;
    }
    else if (ton.getMode() == ModeName::Lydian)
    {
        return ton.fifths() + 82;
    }
    else if (ton.getMode() == ModeName::Mixolydian)
    {
        return ton.fifths() + 97;
    }
    else if (ton.getMode() == ModeName::Aeolian)
    {
        return ton.fifths() + 112;
    }
    else if (ton.getMode() == ModeName::Locrian)
    {
        return ton.fifths() + 127;
    }
    else
    {
        ERROR("Weber index undefined for {}", ton);
        return WeberModal_static::NB_TONS;
    }
}


} // end namespace pse

