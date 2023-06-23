//
//  Costtt.tpp
//  pse
//
//  Created by Florent Jacquemard on 23/06/2023.
//

namespace pse {


template<class T>
bool Costt<T>::operator!=(const T& rhs) const
{
    return !operator==(rhs);
}


template<class T>
bool Costt<T>::operator<=(const T& rhs) const
{
    return !operator>(rhs);
}


template<class T>
bool Costt<T>::operator>(const T& rhs) const
{
    return rhs.operator<(*this); // rhs.Compare(*this);
}


template<class T>
bool Costt<T>::operator>=(const T& rhs) const
{
    return !operator<(rhs);
}


//template<class T>
//T Costt<T>::operator+(const T& rhs) const
//{
//    Costt<T> copy(*this);
//    return copy.operator+=(rhs);
//}


//template<class T>
//std::shared_ptr<Cost> Costt<T>::shared_zero() const
//{
//    return std::shared_ptr<Cost>(new T());
//}



} // end namespace pse
