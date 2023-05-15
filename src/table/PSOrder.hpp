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
/// - index in enumerator.
struct PSClex
{
    bool operator()(std::shared_ptr<const PSC0>& lhs,
                    std::shared_ptr<const PSC0>& rhs);
   
};

/// ordering for PS Config0 based on lexico combination of
/// - cost (nb accidents, dist. to local tonality,
/// number of disjoint moves, color), ordered lexicographically,
/// withh cumul of number of accids and non-diatonic moves.
/// - index in enumerator
struct PSCcumul
{
    bool operator()(std::shared_ptr<const PSC0>& lhs,
                    std::shared_ptr<const PSC0>& rhs);
};

/// ordering for PS Config0 based on nb of accidents only.
struct PSCacc
{
    bool operator()(std::shared_ptr<const PSC0>& lhs,
                    std::shared_ptr<const PSC0>& rhs);
};

/// ordering for PS Config0 based on nb of accidents and
/// number of non-diatonic moves only
struct PSCad
{
    bool operator()(std::shared_ptr<const PSC0>& lhs,
                    std::shared_ptr<const PSC0>& rhs);
};

/// ordering for PS Config0
struct PSCaplusd
{
    bool operator()(std::shared_ptr<const PSC0>& lhs,
                    std::shared_ptr<const PSC0>& rhs);
};

/// ordering for base PSConfig0 based on dist.
/// @todo TBR
/// @todo mv to PSC0
struct PSCdist
{
    bool operator()(std::shared_ptr<const PSC0>& lhs,
                    std::shared_ptr<const PSC0>& rhs);
};



} // end namespace pse

#endif /* PSOrder_hpp */

/// @}

