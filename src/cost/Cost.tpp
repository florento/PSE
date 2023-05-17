//
//  Cost.tpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//

namespace pse {

//const Cost operator+(const Cost& lhs, const Cost& rhs)
//{
//    return lhs.operator+(rhs);
//}


template<class T>
bool Cost<T>::operator!=(const T& rhs) const
{
    return !operator==(rhs);
}


template<class T>
bool Cost<T>::operator<=(const T& rhs) const
{
    return !operator>(rhs);
}


template<class T>
bool Cost<T>::operator>(const T& rhs) const
{
    return rhs.operator<(*this); // rhs.Compare(*this);
}


template<class T>
bool Cost<T>::operator>=(const T& rhs) const
{
    return !operator<(rhs);
}


//template<class T>
//T Cost<T>::operator+(const T& rhs) const
//{
//    Cost<T> copy(*this);
//    return copy.operator+=(rhs);
//}


} // end namespace pse
