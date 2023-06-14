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


double distCost(const double lhs, const double rhs)
{
    assert(lhs >= 0);
    assert(rhs >= 0);
    double d = (lhs - rhs);
    if (d < 0)
    {
        assert(rhs > 0);
        return (d/rhs)*100;
    }
    else if (d > 0)
    {
        assert(lhs > 0);
        return (d/lhs)*100;
    }
    else
        return 0;
}


} // end namespace pse
