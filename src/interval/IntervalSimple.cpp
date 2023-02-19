//
//  IntervalSimple.cpp
//  pse
//
//  Created by Florent Jacquemard on 13/02/2023.
//

#include "IntervalSimple.hpp"


namespace pse {


const unsigned int SimpleInterval::UNDEF_INTERVAL = 0;

const unsigned int SimpleInterval::UNDEF_COST = -1;


SimpleInterval::SimpleInterval():
_nb(UNDEF_INTERVAL),
_quality(Quality::Undef),
_cost(UNDEF_COST)
{ }


SimpleInterval::SimpleInterval(unsigned int i, const Quality& q,
                               unsigned int cost):
_nb(i),
_quality(q),
_cost(0)
{
    assert(i != UNDEF_INTERVAL);
    assert(1 <= i && i <= 8); // unison to octave
    assert(q != Quality::Undef);
    assert(cost != UNDEF_COST);
    assert(0 <= cost && cost <= 2);
}


// static abbreviations for interval qualities
//const SimpleInterval::Quality SimpleInterval::_s2D = Quality::SuperDiminished;
//const SimpleInterval::Quality SimpleInterval::_sD  = Quality::SubDiminished;
//const SimpleInterval::Quality SimpleInterval::_D   = Quality::Diminished;
//const SimpleInterval::Quality SimpleInterval::_m   = Quality::Minor;
//const SimpleInterval::Quality SimpleInterval::_P   = Quality::Perfect;
//const SimpleInterval::Quality SimpleInterval::_M   = Quality::Major;
//const SimpleInterval::Quality SimpleInterval::_A   = Quality::Augmented;
//const SimpleInterval::Quality SimpleInterval::_oA  = Quality::OverAugmented;
//const SimpleInterval::Quality SimpleInterval::_s2A = Quality::SuperAugmented;
//const SimpleInterval::Quality SimpleInterval::_U   = Quality::Undef;

//static
const std::vector<SimpleInterval> SimpleInterval::INTERVALS[15] =
{
    { { 1, Quality::Perfect,       0 },   // 0 semitone
      { 2, Quality::Diminished,    2 } },
    
    { { 1, Quality::Augmented,     2 },   // 1
      { 2, Quality::Minor,         0 },
      { 3, Quality::SubDiminished, 2 } },

    { { 1, Quality::OverAugmented, 2 },   // 2
      { 2, Quality::Major,         0 },
      { 3, Quality::Diminished,    2 } },

    { { 2, Quality::Augmented,     1 },   // 3
      { 3, Quality::Minor,         0 },
      { 4, Quality::SubDiminished, 2 } },

    { { 2, Quality::OverAugmented, 2 },   // 4
      { 3, Quality::Major,         0 },
      { 4, Quality::Diminished,    1 } },

    { { 3, Quality::Augmented,     2 },   // 5
      { 4, Quality::Perfect,       0 },
      { 5, Quality::SubDiminished, 2 } },

    { { 3, Quality::OverAugmented, 2 },   // 6
      { 4, Quality::Augmented,     1 },
      { 5, Quality::Diminished,    1 },
      { 6, Quality::SubDiminished, 2 } },

    { { 4, Quality::OverAugmented, 2 },   // 7
      { 5, Quality::Perfect,       0 },
      { 6, Quality::Diminished,    2 } },

    { { 5, Quality::Augmented,     1 },   // 8
      { 6, Quality::Minor,         0 },
      { 7, Quality::SubDiminished, 2 } },

    { { 5, Quality::OverAugmented, 2 },   // 9
      { 6, Quality::Major,         0 },
      { 7, Quality::Diminished,    1 } },

    { { 6, Quality::Augmented,     2 },   // 10
      { 7, Quality::Minor,         0 },
      { 8, Quality::SubDiminished, 2 } },

    { { 6, Quality::OverAugmented, 2 },   // 11
      { 7, Quality::Major,         0 },
      { 8, Quality::Diminished,    2 } },

    { { 7, Quality::Augmented,     2 },   // 12
      { 8, Quality::Perfect,       0 } },

    { { 7, Quality::OverAugmented, 2 },   // 13
      { 8, Quality::Augmented,     2 } },

    { { 8, Quality::OverAugmented, 2 } }  // 14
};


// static
std::string SimpleInterval::tostring(const Quality& q)
{
    switch (q)
    {
        //case Quality::SuperDiminished:
        //    return "s2D";

        case Quality::SubDiminished:
            return "sD";

        case Quality::Diminished:
            return "D";

        case Quality::Minor:
            return "m";

        case Quality::Perfect:
            return "P";

        case Quality::Major:
            return "M";

        case Quality::Augmented:
            return "A";

        case Quality::OverAugmented:
            return "oA";

        //case Quality::SuperAugmented:
        //    return "o2A";
            
        case Quality::Undef:
            return "UNDEF";

        default:
        {
            ERROR("unknown interval quality");
            return "ERROR";
        }
    }
}

void SimpleInterval::print(std::ostream& o) const
{
    o << _nb << tostring(_quality);
}


std::ostream& operator<<(std::ostream& o, const SimpleInterval::Quality& q)
{
    o << SimpleInterval::tostring(q);
    return o;
}


std::ostream& operator<<(std::ostream& o, const SimpleInterval& i)
{
    i.print(o);
    return o;
}

} // end namespace pse
