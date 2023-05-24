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


} // end namespace pse
