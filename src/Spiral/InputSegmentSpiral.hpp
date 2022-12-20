//
//  InputSegmentSpiral.hpp
//  qparse
//
//  Created by Florent Jacquemard on 04/02/2020.
//  Copyright © 2020 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#ifndef InputSegmentSpiral_hpp
#define InputSegmentSpiral_hpp

#include <stdio.h>
#include <assert.h>

#include "Spiral.hpp"
#include "InputSegment.hpp"

namespace scoremodel {

/// pitch spelling with the spiral of fifths
class InputSegmentSpiral : public segment::InputSegment
{
public:

    /// @warning seg must be quantized
    InputSegmentSpiral(const InputSegment& seg);
    
    
    /// @brief pitch spelling. unwindowed.
    /// @warning this segment must have been quantized.
    void respell(int k=0);

    /// @brief pitch spelling
    /// with a sliding window of given musical duration.
    /// @warning this segment must have been quantized.
    void respell(mtu_t ws, int k=0);
    
};

} // namespace scoremodel

#endif /* InputSegmentSpiral_hpp */

/// @}
