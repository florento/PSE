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

#include "trace.hpp"
#include "PSEnum.hpp"
#include "NoteName.hpp"
#include "Accidental.hpp"
#include "Ton.hpp"
#include "Cost.hpp"


namespace pse {


/// helper class for redirecting the 2 needed binary comparison operators
/// to the dynamic type class (descendant).
/// @see https://stackoverflow.com/questions/24249327/how-to-see-if-two-polymorphic-objects-are-equal
template<typename Base, typename Derived>
struct PolymorphicComparable : public Base
{
public:
    
    bool operator==(const Base& rhs) const override;
    
    double dist(const Base& rhs) const override;

    bool operator<(const Base& rhs) const override;

    Base& operator+=(const Base& rhs) override;
    
    void print(std::ostream& o) const override;
    
};




/// Abstract class for cost model used for for the ordering of configurations
/// in the the PS algorithms.
/// instanciated by class implementing really cost values,
/// that must define:
/// - copy operator
/// - equality operator
/// - less operator
/// - cumul sum operator
/// - functions of update for transition to PSConfig's
/// @todo TBR (not used)
template<class T>     // class Compare
class Costt : public Cost
{
public:
    
    // create a new null cost value.
    // virtual std::shared_ptr<Cost> shared_zero() const override;

    // create a shared clone of this cost.
    // virtual std::shared_ptr<Cost> shared_clone() const override;
    
    /// assignement operator (required).
    virtual T& operator=(const T& rhs) = 0;
    
    /// equality (mandatory).
    virtual bool operator==(const T& rhs) const = 0;

    /// negation of equality.
    bool operator!=(const T& rhs) const;

    /// a distance value, in percent of the bigger cost.
    /// used for approximate equality.
    virtual double dist(const T& rhs) const = 0;
    
    /// strictly less (mandatory).
    virtual bool operator<(const T& rhs) const = 0;

    /// negation of >.
    bool operator<=(const T& rhs) const;

    /// inverse (commutation) of <.
    bool operator>(const T& rhs) const;

    /// negation of <.
    bool operator>=(const T& rhs) const;
    
    // sum operator (non const)  (required).
    // update this cost adding rhs.
    // @param rhs a cost to add.
    // @warning the ordering of this cost is preserved,
    // the ordering of rhs is ignored.
    // @todo TBR, replaced by update.
    // virtual T& operator+=(const T& rhs) = 0;
    
    // sum operator.
    // T operator+(const T& rhs) const;
    
    /// null cost value.
    virtual T zero() const = 0;
        
};

} // namespace pse

// separated definition of template class
#include "Costt.tpp"

#endif /* Costt_hpp */

/// @}
