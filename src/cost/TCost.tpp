//
//  TCost.tpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//

namespace pse {


template<typename T>
bool TCost<T>::equal(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return T::equal(rhs_T);
}


template<typename T>
bool TCost<T>::smaller(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return T::smaller(rhs_T);
}


template<typename T>
Cost& TCost<T>::add(const Cost& rhs)
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return T::add(rhs_T);
}


template<typename T>
double TCost<T>::pdist(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return T::pdist(rhs_T);
}


template<typename T>
TCostA<T>::~TCostA()
{
    TRACE("delete CostAccid");
}


template<typename T>
std::shared_ptr<Cost> TCostA<T>::shared_zero() const
{
    return std::shared_ptr<Cost>(new T());
}


template<typename T>
std::shared_ptr<Cost> TCostA<T>::shared_clone() const
{
    return std::shared_ptr<Cost>(new T(*this));
}


template<typename T>
std::unique_ptr<Cost> TCostA<T>::unique_clone() const
{
    return std::unique_ptr<Cost>(new T(*this));
}


} // end namespace pse
