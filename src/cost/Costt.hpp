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


} // namespace pse

// separated definition of template class
#include "Costt.tpp"

#endif /* Costt_hpp */

/// @}
