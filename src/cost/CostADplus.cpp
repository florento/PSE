//
//  CostADplus.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADplus.hpp"

namespace pse {


CostADplus::CostADplus(bool tb_lex):
CostAD(tb_lex)
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


// equality of (_accid = accids + dist) and TB
bool CostADplus::equal(const CostADplus& rhs) const
{
    // compare _accid (which is the sum of nb of accids and dist) and TB.
    // ignore the distance value.
    return CostAT::equal(rhs);

    // assert(_sum == _accid + _dist);
    // assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    // return ((_sum == rhs_ADplus._sum) and tiebreak_equal(rhs_ADplus));
}


bool CostADplus::smaller(const CostADplus& rhs) const
{
    // compare _accid (which is the sum of nb of accids and dist), then TB
    // ignore the distance value.
    return CostAT::smaller(rhs);
    
    // assert(_sum == _accid + _dist);
    // assert(rhs_ADplus._sum == rhs_ADplus._accid + rhs_ADplus._dist);
    // if (_sum == rhs_ADplus._sum)
    //     return tiebreak_smaller(rhs_ADplus);
    // else
    //     return (_sum < rhs_ADplus._sum);
}


// same as CostAD::add
Cost& CostADplus::add(const CostADplus& rhs)
{
    CostAD::add(rhs);
    // _sum = _accid + _dist;
    return *this;
}


// TBR
double CostADplus::pdist(const CostADplus& rhs) const
{
    // _accid is the sum of nb of accids and dist
    return Cost::dist((double) this->_accid, (double) rhs._accid);

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



size_t CostADplus::accids() const
{
    assert(_accid >= _dist);
    return (_accid - _dist);
}


bool CostADplus::update(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton,
                        const enum NoteName& prev_name)
{
    size_t olddist(_dist);
    // update accid and dist
    bool ret = CostAD::update(name, accid, print, gton, lton, prev_name);
    // dist has been increased (by new dists)
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
    assert(_accid >= _inconsist + _dist);
    o << (_accid - _inconsist - _dist) << '+';
    o <<  _inconsist << '+' << _dist << ':';
    printTB(o);
}


std::ostream& operator<<(std::ostream& o, const CostADplus& c)
{
    c.print(o);
    return o;
}


} // end namespace pse
