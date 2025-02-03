//
//  Costt.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//
/// @addtogroup pitch
/// @{


#ifndef Costt_hpp
#define Costt_hpp

#include <iostream>
#include <assert.h>

#include "pstrace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Ton.hpp"
#include "Cost.hpp"


namespace pse {


/// helper class for redirecting the needed operators
/// to the dynamic type class (descendant),
/// following the Curiously Recurring Template.
/// @see https://stackoverflow.com/questions/24249327/how-to-see-if-two-polymorphic-objects-are-equal
template<typename Derived>
struct PolymorphicCost
{
    
protected:
    
    /// override Cost::equal
    bool equal(const Cost& rhs) const;
    
    /// override Cost::smaller
    bool smaller(const Cost& rhs) const;

    /// override Cost::add
    Cost& add(const Cost& rhs);

    /// override Cost::pdist
    double pdist(const Cost& rhs) const;
    
};


} // namespace pse

// separated definition of template class
#include "Costt.tpp"

#endif /* Costt_hpp */

/// @}
