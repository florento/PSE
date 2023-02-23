//
//  PSOrder.hpp
//  pse
//
//  Created by Florent Jacquemard on 22/02/2023.
//
/// @addtogroup pitch
/// @{


#ifndef PSOrder_hpp
#define PSOrder_hpp

#include <iostream>
#include <assert.h>

#include "trace.hpp"
#include "PSEnum.hpp"
#include "PSCost.hpp"
#include "PSConfig0.hpp"


namespace pse {


/// ordering for PS Config0 based on lexico combination of
/// - cost (nb accidents, dist. to local tonality, number of disjoint moves, color)
/// - index in enumerator
PSCCompare PSClex =
[](std::shared_ptr<const PSC0> lhs, std::shared_ptr<const PSC0> rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost() == rhs->cost())
        return (lhs->id() < rhs->id());      // largest index
    else
        return (lhs->cost() > rhs->cost());  // smallest cost
};
//    if (lhs->accidentals() == rhs->accidentals())
//    {
//        // dist, lexicographically
//        if (lhs->dist() == rhs->dist())
//        {
//            // disjoint moves, lexicographically
//            if (lhs->disjoint() == rhs->disjoint())
//                return false; // tie break fail
//            else
//                return (lhs->disjoint() > rhs->disjoint());  // smallest nb mv disjoint moves
//        }
//        else
//            return (lhs->dist() > rhs->dist());  // smallest dist
//    }
//    else
//        return (lhs->accidentals() > rhs->accidentals());  // smallest cost


/// ordering for PS Config0 based on nb of accidents only.
PSCCompare PSCacc =
[](std::shared_ptr<const PSC0> lhs, std::shared_ptr<const PSC0> rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost().getAccid() == rhs->cost().getAccid())
        return (lhs->id() < rhs->id());  // largest index
    else
        return (lhs->cost().getAccid() > rhs->cost().getAccid());
};


/// ordering for base PSConfig0 based on dist.
/// @todo TBR
/// @todo mv to PSC0
PSCCompare PSCdist =
[](std::shared_ptr<const PSC0> lhs, std::shared_ptr<const PSC0> rhs)
{
    assert (lhs);
    assert (rhs);
    return (lhs->cost().getDist() > rhs->cost().getDist());
};






} // end namespace pse

#endif /* PSOrder_hpp */

/// @}

