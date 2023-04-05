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


/// ordering relationships for costs.
enum class CostOrdering
{
    /// default ordering definned byb operator== and operator<
    Default,
    
    /// lexicographic ordering (on cost components)
    Lex,
    
    /// lexicographhic ordering with first component
    /// a cumul of number of accid and non-diatonic moves,
    /// and the nnext components.
    Cumul,
    
    /// lexicographhic with approximate same number of accid.
    Approx,
    
    /// unknown
    Undef
};


std::ostream& operator<<(std::ostream& o, const CostOrdering& co);


/// ordering for PS Config0 based on lexico combination of
/// - cost (nb accidents, dist. to local tonality, number of disjoint moves, color), ordered lexxicographically
/// - index in enumerator
PSCCompare PSClex =
[](std::shared_ptr<const PSC0>& lhs, std::shared_ptr<const PSC0>& rhs)
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


/// ordering for PS Config0 based on lexico combination of
/// - cost (nb accidents, dist. to local tonality, number of disjoint moves, color), ordered lexicographically,
/// withh cumul of number of accids and non-diatoinc moves.
/// - index in enumerator
PSCCompare PSCcumul =
[](std::shared_ptr<const PSC0>& lhs, std::shared_ptr<const PSC0>& rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost().eq_cumul(rhs->cost()))
        return (lhs->id() < rhs->id()); // largest index
    else
        return (lhs->cost().greater_cumul(rhs->cost()));
        // smallest cost
};


/// ordering for PS Config0 based on nb of accidents only.
PSCCompare PSCacc =
[](std::shared_ptr<const PSC0>& lhs, std::shared_ptr<const PSC0>& rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost().getAccid() == rhs->cost().getAccid())
        return (lhs->id() < rhs->id());  // largest index
    else
        return (lhs->cost().getAccid() > rhs->cost().getAccid());
};


/// ordering for PS Config0 based on nb of accidents and
/// number of non-diatonic moves only
PSCCompare PSCad =
[](std::shared_ptr<const PSC0>& lhs, std::shared_ptr<const PSC0>& rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost().getAccid() == rhs->cost().getAccid())
    {
        if (lhs->cost().getDia() == rhs->cost().getDia())
            return (lhs->id() < rhs->id());  // largest index
        else
            return (lhs->cost().getDia() > rhs->cost().getDia());
    }
    else
        return (lhs->cost().getAccid() > rhs->cost().getAccid());
};


/// ordering for PS Config0
PSCCompare PSCaplusd =
[](std::shared_ptr<const PSC0>& lhs, std::shared_ptr<const PSC0>& rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost().getAccid() + lhs->cost().getDia() ==
        rhs->cost().getAccid() + rhs->cost().getDia())
    {
        if (lhs->cost().getDist() == rhs->cost().getDist())
        {
            if (lhs->cost().getColor() == rhs->cost().getColor())
                return (lhs->id() < rhs->id()); // largest index
            else
                return (lhs->cost().getColor() > rhs->cost().getColor());
       }
        else
            return (lhs->cost().getDist() > rhs->cost().getDist());
    }
    else
        return (lhs->cost().getAccid() + lhs->cost().getDia() >
                rhs->cost().getAccid() + rhs->cost().getDia());
};


/// ordering for base PSConfig0 based on dist.
/// @todo TBR
/// @todo mv to PSC0
PSCCompare PSCdist =
[](std::shared_ptr<const PSC0>& lhs, std::shared_ptr<const PSC0>& rhs)
{
    assert (lhs);
    assert (rhs);
    return (lhs->cost().getDist() > rhs->cost().getDist());
};



} // end namespace pse

#endif /* PSOrder_hpp */

/// @}

