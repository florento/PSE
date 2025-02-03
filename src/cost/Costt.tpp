//
//  Costt.tpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//

namespace pse {


template<typename Derived>
bool PolymorphicCost<Derived>::equal(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(Derived));

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& rhs_Derived = static_cast<const Derived&>(rhs);

    // redirect to Derived operator
    return Derived::equal(rhs_Derived);
}


template<typename Derived>
bool PolymorphicCost<Derived>::smaller(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(Derived));

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& rhs_Derived = static_cast<const Derived&>(rhs);

    // redirect to Derived operator
    return Derived::smaller(rhs_Derived);
}


template<typename Derived>
Cost& PolymorphicCost<Derived>::add(const Cost& rhs)
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(Derived));

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& rhs_Derived = static_cast<const Derived&>(rhs);

    // redirect to Derived operator
    return Derived::add(rhs_Derived);
}


template<typename Derived>
double PolymorphicCost<Derived>::pdist(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(Derived));

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& rhs_Derived = static_cast<const Derived&>(rhs);

    // redirect to Derived operator
    return Derived::pdist(rhs_Derived);
}


} // end namespace pse
