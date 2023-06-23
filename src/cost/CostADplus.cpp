//
//  CostADplus.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADplus.hpp"

namespace pse {


CostADplus::CostADplus():
CostAD(),
_sum(_accid + _dist)
{ }


CostADplus::CostADplus(const CostADplus& rhs):
CostAD(rhs),
_sum(rhs._sum)
{
    assert(_sum == _accid + _dist);
}


CostADplus::~CostADplus()
{
    TRACE("delete CostADplus");
}


bool CostADplus::operator==(const CostADplus& rhs) const
{
    assert(_sum == _accid + _dist);
    assert(rhs._sum == rhs._accid + rhs._dist);
    return (_sum == rhs._sum);
}


double CostADplus::dist(const CostADplus& rhs) const
{
    assert(_sum == _accid + _dist);
    assert(rhs._sum == rhs._accid + rhs._dist);
    return distCost((double) _sum, (double) rhs._sum);
}


bool CostADplus::operator<(const CostADplus& rhs) const
{
    assert(_sum == _accid + _dist);
    assert(rhs._sum == rhs._accid + rhs._dist);
    return (_sum < rhs._sum);
}


CostADplus& CostADplus::operator+=(const CostADplus& rhs)
{
    CostAD::operator+=(rhs);
    _sum = _accid + _dist;
    return *this;
}


std::shared_ptr<Cost> CostADplus::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostADplus());
}


std::shared_ptr<Cost> CostADplus::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostADplus(*this));
}

std::unique_ptr<Cost> CostADplus::unique_clone() const
{
    return std::unique_ptr<Cost>(new CostADplus(*this));
}


void CostADplus::update(const enum NoteName& name, const enum Accid& accid,
                        bool print,
                        const Ton& gton, const Ton& lton)
{
    CostAD::update(name, accid, print, gton, lton);
    _sum = _accid + _dist;
}


void CostADplus::print(std::ostream& o) const
{
    CostAD::print(o);
    o << " sum=" << _sum;
}


std::ostream& operator<<(std::ostream& o, const CostADplus& c)
{
    c.print(o);
    return o;
}

} // end namespace pse

