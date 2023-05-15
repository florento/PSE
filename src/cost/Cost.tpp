//
//  Cost.tpp
//  pse
//
//  Created by Florent Jacquemard on 12/05/2023.
//

namespace pse {


T Cost::operator+(const T& rhs) const
{
    Cost copy(*this);
    return copy.operator+=(rhs);
}


//const Cost operator+(const Cost& lhs, const Cost& rhs)
//{
//    return lhs.operator+(rhs);
//}


bool Cost::operator!=(const T& rhs) const
{
    return !operator==(rhs);
}


bool Cost::operator<=(const T& rhs) const
{
    return !operator>(rhs);
}


bool Cost::operator>(const T& rhs) const
{
    return rhs.operator<(*this); // rhs.Compare(*this);
}


bool Cost::operator>=(const T& rhs) const
{
    return !operator<(rhs);
}


} // end namespace pse
