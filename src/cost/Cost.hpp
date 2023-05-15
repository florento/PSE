//
//  Cost.hpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//
/// @addtogroup pitch
/// @{


#ifndef Cost_hpp
#define Cost_hpp

#include <iostream>
#include <assert.h>

#include "trace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"


namespace pse {

/// Abstract class for cost model used for for the ordering of configurations
/// in the the PS algorithms.
/// instanciated by class implementing really cost values,
/// that must define:
/// - copy operator
/// - equality operator
/// - less operator
/// - cumul sum operator
/// - functions of update for transition to PSConfig's
template<class T>     // class Compare
class Cost
{
public:
    
    /// assignement operator (required).
    virtual T& operator=(const T& rhs) = 0;
    
    /// sum operator (non const)  (required).
    /// update this cost adding rhs.
    /// @param rhs a cost to add.
    /// @warning the ordering of this cost is preserved,
    /// the ordering of rhs is ignored.
    virtual T& operator+=(const T& rhs) = 0;
    
    /// sum operator.
    T operator+(const T& rhs) const;

    /// equality (mandatory).
    virtual bool operator==(const T& rhs) const = 0;

    /// negation of equality.
    bool operator!=(const T& rhs) const;

    /// strictly less (mandatory).
    virtual bool operator<(const T& rhs) const = 0;

    /// negation of >.
    bool operator<=(const T& rhs) const;

    /// inverse (commutation) of <.
    bool operator>(const T& rhs) const;

    /// negation of <.
    bool operator>=(const T& rhs) const;
    
};

} // namespace pse

// separated definition of template class
#include "Cost.tpp"

#endif /* Cost_hpp */

/// @}
