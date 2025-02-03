//
//  Costt.tpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//

namespace pse {

//const Costt operator+(const Costt& lhs, const Costt& rhs)
//{
//    return lhs.operator+(rhs);
//}

template<typename Base, typename Derived>
bool PolymorphicComparable<Base, Derived>::operator==(const Base& rhs) const
{
    // check if the dynamic types match
    if (typeid(rhs) != typeid(Derived))
        return false;

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& a = static_cast<const Derived&>(*this);
    const Derived& b = static_cast<const Derived&>(rhs);

    // redirect to Derived::operator==(Derived)
    return a == b;
}


template<typename Base, typename Derived>
double PolymorphicComparable<Base, Derived>::dist(const Base& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(Derived));   // or return 0;

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& a = static_cast<const Derived&>(*this);
    const Derived& b = static_cast<const Derived&>(rhs);

    // redirect to Derived::operator<(Derived)
    return a.dist(b);
}


template<typename Base, typename Derived>
bool PolymorphicComparable<Base, Derived>::operator<(const Base& rhs) const
{
    // check if the dynamic types match
    if (typeid(rhs) != typeid(Derived))
        return false;

    // cast to the concrete types; thanks to the check above this is safe
    const Derived& a = static_cast<const Derived&>(*this);
    const Derived& b = static_cast<const Derived&>(rhs);

    // redirect to Derived::operator<(Derived)
    return a < b;
}


template<typename Base, typename Derived>
Base& PolymorphicComparable<Base, Derived>::operator+=(const Base& rhs)
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(Derived));

    // cast to the concrete types; thanks to the check above this is safe
    Derived& a = static_cast<Derived&>(*this);
    const Derived& b = static_cast<const Derived&>(rhs);

    // redirect to Derived::operator+=(Derived)
    return (a += b);
}


template<typename Base, typename Derived>
void PolymorphicComparable<Base, Derived>::print(std::ostream& o) const
{
    const Derived& a = static_cast<const Derived&>(*this);
    a.print(o);
}


} // end namespace pse
