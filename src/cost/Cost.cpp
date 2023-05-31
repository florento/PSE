//
//  Cost.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include "Cost.hpp"

namespace pse {

bool Cost::operator!=(const Cost& rhs) const
{
    return !operator==(rhs);
}


bool Cost::operator<=(const Cost& rhs) const
{
    return !operator>(rhs);
}


bool Cost::operator>(const Cost& rhs) const
{
    return rhs.operator<(*this); // rhs.Compare(*this);
}


bool Cost::operator>=(const Cost& rhs) const
{
    return !operator<(rhs);
}


std::ostream& operator<<(std::ostream& o, const Cost& c)
{
    c.print(o);
    return o;
}

} // end namespace pse
