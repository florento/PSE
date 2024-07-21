//
//  WeberBluesModal_static.cpp
//  pse
//
//  Created by Augustin Bouquillard on 21/07/2024.
//

#include "WeberBluesModal_static.hpp"

namespace pse {

// number of tonalities considered.
// static const size_t WeberModal_static::NB_TONS = 165;


// static
int WeberBluesModal_static::dist(const Ton& ton1, const Ton& ton2)
{
    size_t i1 = index(ton1);
    // DEBUG("{} in NB_TONS={}", i1, NB_TONS);
    
    assert(i1 < WeberBluesModal_static::NB_TONS);
    size_t i2 = index(ton2);
    assert(i2 < WeberBluesModal_static::NB_TONS);
    return WeberBluesModal_static::WEBER_DIST[i1][i2];
}

// static
size_t WeberBluesModal_static::index(const Ton& ton)
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
    else if (ton.getMode() == ModeName::MajorBlues)
    {
        return ton.fifths() + 142;
    }
    else if (ton.getMode() == ModeName::MinorBlues)
    {
        return ton.fifths() + 157;
    }
    else
    {
        ERROR("Weber index undefined for {}", ton);
        return WeberBluesModal_static::NB_TONS;
    }
}

const std::vector<std::vector<int>> WeberBluesModal_static::WEBER_DIST =
{
};
}
