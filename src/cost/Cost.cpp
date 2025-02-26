//
//  Cost.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include "Cost.hpp"

namespace pse {


bool Cost::operator==(const Cost& rhs) const
{
    // RTTI check
    if (typeid(*this) != typeid(rhs))
    {
        ERROR("Cost: equality between different types");
        return false;
    }
    // Invoke equal on derived types
    return equal(rhs);
}


bool Cost::operator!=(const Cost& rhs) const
{
    return !operator==(rhs);
}


bool Cost::operator<(const Cost& rhs) const
{
    // RTTI check
    if (typeid(*this) != typeid(rhs))
    {
        ERROR("Cost: disequality between different types");
        return false;
    }
    // Invoke smaller on derived types
    return smaller(rhs);
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


Cost& Cost::operator+=(const Cost& rhs)
{
    // RTTI check
    if (typeid(*this) != typeid(rhs))
    {
        ERROR("Cost: sum between different types");
    }
    // Invoke smaller on derived types
    return add(rhs);
}


double Cost::dist(const Cost& rhs) const
{
    // RTTI check
    if (typeid(*this) != typeid(rhs))
    {
        ERROR("Cost: dist between different types");
        return false;
    }
    // Invoke smaller on derived types
    return pdist(rhs);
}


// static
double Cost::dist(const double lhs, const double rhs)
{
    assert(lhs >= 0);
    assert(rhs >= 0);
    double d = (lhs - rhs);
    if (d < 0)
    {
        assert(rhs > 0);
        return (d*100/lhs); // negative when lhs smaller than rhs
    }
    else if (d > 0)
    {
        assert(lhs > 0);
        return (d*100/rhs); // positive when lhs larger than rhs
    }
    else
        return 0;
}


void Cost::print(std::ostream& o) const
{
    // should not be called
    ERROR("Cost: print abstract Cost");
}


std::ostream& operator<<(std::ostream& o, const Cost& c)
{
    c.print(o);
    return o;
}





} // end namespace pse
