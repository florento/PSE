//
//  TCost.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//
/// @addtogroup pitch
/// @{


#ifndef TCost_hpp
#define TCost_hpp

#include <iostream>
#include <assert.h>

#include "pstrace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accid.hpp"
#include "Ton.hpp"
#include "Cost.hpp"

namespace pse {


/// This abstract template (CRTP)
/// helper for redirecting the needed operators
/// to the dynamic type class (descendant),
/// following the Curiously Recurring Template Pattern.
/// @see https://stackoverflow.com/questions/24249327/how-to-see-if-two-polymorphic-objects-are-equal
/// @see CRTP and inheritance hierarchy
/// https://stackoverflow.com/questions/18174441/crtp-and-multilevel-inheritance
template<typename T>
struct TCost : public Cost
{
    
public: // construction
    
    /// create a new null cost value.
    std::shared_ptr<Cost> shared_zero() const override;
    
    /// @todo shared_zero with flags
    
    /// create a shared clone of this cost.
    std::shared_ptr<Cost> shared_clone() const override;
    
    /// create a smart clone of this cost.
    std::unique_ptr<Cost> unique_clone() const override;
    
protected: // comparison
    
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
#include "TCost.tpp"

#endif /* TCost_hpp */

/// @}
