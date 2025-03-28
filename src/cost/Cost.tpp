//
//  Cost.tpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//

namespace pse {


template<typename T>
std::shared_ptr<Cost> Cost::shared_zero() const
{
    return std::shared_ptr<Cost>(new T());
}


template<typename T>
std::shared_ptr<Cost> Cost::shared_zero(bool flag) const
{
    return std::shared_ptr<Cost>(new T(flag));
}


template<typename T>
std::shared_ptr<Cost> Cost::shared_clone() const
{
    return std::shared_ptr<Cost>(new T(static_cast<const T&>(*this)));
}


template<typename T>
std::unique_ptr<Cost> Cost::unique_clone() const
{
    return std::unique_ptr<Cost>(new T(static_cast<const T&>(*this)));
}


template<typename T>
bool Cost::equal(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return static_cast<const T*>(this)->T::equal(rhs_T);
}

template<typename T>
bool Cost::smaller(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return static_cast<const T*>(this)->T::smaller(rhs_T);
}


template<typename T>
Cost& Cost::add(const Cost& rhs)
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return static_cast<T*>(this)->T::add(rhs_T);
}


template<typename T>
double Cost::pdist(const Cost& rhs) const
{
    // check if the dynamic types match
    assert(typeid(rhs) == typeid(T));

    // cast to the concrete types; thanks to the check above this is safe
    const T& rhs_T = static_cast<const T&>(rhs);

    // redirect to T operator
    return static_cast<const T*>(this)->T::pdist(rhs_T);
}


} // end namespace pse
