//
//  CostADplus.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADplus.hpp"

namespace pse {


CostADplus::CostADplus(bool tb_sum):
CostAD(tb_sum)
// _sum(_accid + _dist)
{ }


CostADplus::CostADplus(const CostADplus& rhs):
CostAD(rhs)
// _sum(rhs._sum)
{
    // assert(_sum == _accid + _dist);
}


CostADplus::~CostADplus()
{
    TRACE("delete CostADplus");
}


std::shared_ptr<Cost> CostADplus::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostADplus(this->_tblex));
}


std::shared_ptr<Cost> CostADplus::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostADplus(*this));
}


//std::unique_ptr<Cost> CostADplus::unique_clone() const
//{
//    return std::unique_ptr<Cost>(new CostADplus(*this));
//}


// equality of (_accid = accids + dist) and TB
bool CostADplus::equal(const Cost& rhs) const
{
    // _accid is the sum of nb of accids and dist
    const CostADplus& rhs_ADplus = dynamic_cast<const CostADplus&>(rhs);
    return CostAT::equal(rhs_ADplus);

    // assert(_sum == _accid + _dist);
    // assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    // return ((_sum == rhs_ADplus._sum) and tiebreak_equal(rhs_ADplus));
}


bool CostADplus::smaller(const Cost& rhs) const
{
    // _accid is the sum of nb of accids and dist
    const CostADplus& rhs_ADplus = dynamic_cast<const CostADplus&>(rhs);
    return CostAT::smaller(rhs_ADplus);
    
    // assert(_sum == _accid + _dist);
    // assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    // if (_sum == rhs_ADplus._sum)
    //     return tiebreak_smaller(rhs_ADplus);
    // else
    //     return (_sum < rhs_ADplus._sum);
}


size_t CostADplus::accids() const
{
    assert(_accid >= _dist);
    return (_accid - _dist);
}


// same as CostAD::add(
//Cost& CostADplus::add(const Cost& rhs)
//{
//    CostAD::add(rhs);
//    // _sum = _accid + _dist;
//    return *this;
//}


// TBR
double CostADplus::pdist(const Cost& rhs) const
{
    // _accid is the sum of nb of accids and dist
    const CostADplus& rhs_ADplus = dynamic_cast<const CostADplus&>(rhs);
    return Cost::dist((double) _accid, (double) rhs_ADplus._accid);
    // assert(_sum == _accid + _dist);
    // assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    // ignore the tiebreaking measures (only counts accids + dist)
    // return Cost::dist((double) _sum, (double) rhs_ADplus._sum);

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
    size_t olddist(_dist);
    bool ret = CostAD::update(name, accid, print, gton, lton);
    // _sum = _accid + _dist;
    // dist increased (by new dists)
    assert(olddist <= _dist);
    // _accid was increased only by new accids, add the new dists
    _accid += (_dist - olddist);
    
    return ret;
}


CostType CostADplus::type() const
{
    if (_tblex)
        return CostType::ADplus;
    else
        return CostType::ADpluss;
}


void CostADplus::print(std::ostream& o) const
{
    // CostAD::print(o);
    // o << _accid << '=';
    o << accids() << "+" << _dist << ':';
    o << _color << ':' << _cflat << ':' << _double << ':' << _chromharm;
}


std::ostream& operator<<(std::ostream& o, const CostADplus& c)
{
    c.print(o);
    return o;
}

} // end namespace pse
