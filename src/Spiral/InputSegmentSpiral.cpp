//
//  InputSegmentSpiral.cpp
//  qparse
//
//  Created by Florent Jacquemard on 04/02/2020.
//  Copyright © 2020 Florent Jacquemard. All rights reserved.
//

#include "InputSegmentSpiral.hpp"

namespace scoremodel {

void InputSegmentSpiral::respell(int k)
{
    TRACE("respell segment (h={}, r={}", SpiralName::h, SpiralName::r);

// dump the table of synonym notes
//    for (size_t i = 0; i < 12; i++)
//    {
//        for (size_t j = 0; j < 3; j++)
//        {
//            INFO("syn[{}][{}] = {}", i, j, SpiralName::synonyms[i][j]);
//        }
//    }
    
    //assert(quantized());
    //const SpiralName& current = SpiralName::ofkey(k);
    // unwindowed: centroid of all pitches so far.
    SpiralPoint centroid(SpiralName::ofkey(k));
    
    for (segment::MusPointSeq::iterator i = _points.begin();
         i != _points.end(); i++)
    {
        segment::MusPoint& p = *i;
        segment::MusEvent* e = p.event();
        assert(e);
        if (e->isNote())
        {
            segment::NoteEvent* ne = dynamic_cast<segment::NoteEvent*>(e);
            assert(ne);
            if (ne->unpitched())
            {
                continue;
            }
            else
            {
                const SpiralName& nn =
                    SpiralName::closest(ne->pitch().midi(), centroid);
                // update musevent
                INFO("respell {} to {}", *e, nn);
//                      ne->pitch().name, ne->pitch().alteration,
//                      nn.name, nn.alteration);
                ne->pitch().name = pse::NNofchar(nn.name);
                ne->pitch().alteration = pse::accidofint(nn.alteration);
                SpiralPoint update(nn);
                // segment is quantized
                assert(MTU::defined(p.mduration()));
                // TBC: update = zero for grace-notes (ignored)
                update *= toDouble(p.mduration()/Interval::mduration());
                centroid += update;
                TRACE("new point: {} centroid={}", nn, centroid);
            }
        }
        else
        {
            continue;
        }
    }
}


void InputSegmentSpiral::respell(mtu_t wlength, int k)
{
    WARN("respell SlidingWindow: not implemented");
}

} // namespace scoremodel

