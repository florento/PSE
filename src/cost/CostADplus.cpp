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


std::shared_ptr<Cost> CostADplus::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostADplus());
}


std::shared_ptr<Cost> CostADplus::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostADplus(*this));
}


//std::unique_ptr<Cost> CostADplus::unique_clone() const
//{
//    return std::unique_ptr<Cost>(new CostADplus(*this));
//}


bool CostADplus::equal(const Cost& rhs) const
{
    const CostADplus& rhs_ADplus = dynamic_cast<const CostADplus&>(rhs);
    assert(_sum == _accid + _dist);
    assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    return ((_sum == rhs_ADplus._sum) and tiebreak_equal(rhs_ADplus));
}


bool CostADplus::smaller(const Cost& rhs) const
{
    const CostADplus& rhs_ADplus = dynamic_cast<const CostADplus&>(rhs);
    assert(_sum == _accid + _dist);
    assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    if (_sum == rhs_ADplus._sum)
    {
        return tiebreak_smaller(rhs_ADplus);
    }
    else
    {
        return (_sum < rhs_ADplus._sum);
    }
}


Cost& CostADplus::add(const Cost& rhs)
{
    CostAD::add(rhs);
    _sum = _accid + _dist;
    return *this;
}


// TBR
double CostADplus::pdist(const Cost& rhs) const
{
    const CostADplus& rhs_ADplus = dynamic_cast<const CostADplus&>(rhs);
    assert(_sum == _accid + _dist);
    assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    // ignore the tiebreaking measures (only counts accids + dist)
    return Cost::dist((double) _sum, (double) rhs_ADplus._sum);
    // if (_sum == rhs_ADplus._sum)
    // {
    //     return tiebreak_pdist(rhs_ADplus);
    // }
    // else
    //     return Cost::dist((double) _sum, (double) rhs_ADplus._sum);
}


bool CostADplus::update(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    bool ret = CostAD::update(name, accid, print, gton, lton);
    _sum = _accid + _dist;
    return ret;
}


CostType CostADplus::type() const
{
    return CostType::ADplus;
}


void CostADplus::print(std::ostream& o) const
{
    // CostAD::print(o);
    // o << _sum;
    o << _accid << "+" << _dist << ':';
    o << _color << ':';
    o << _cflat << ':';
    o << _double << ':';
    o << _chromharm;
}


std::ostream& operator<<(std::ostream& o, const CostADplus& c)
{
    c.print(o);
    return o;
}

} // end namespace pse


