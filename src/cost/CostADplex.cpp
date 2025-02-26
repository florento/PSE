//
//  CostADplex.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/02/2025.
//

#include "CostADplex.hpp"


namespace pse {


CostADplex::CostADplex(bool tb_sum):
CostADplus(tb_sum)
{ }


CostADplex::CostADplex(const CostADplex& rhs):
CostADplus(rhs)
{ }


CostADplex::~CostADplex()
{
    TRACE("delete CostADplex");
}


// equality of (_accid = accids + dist), dist and TB
bool CostADplex::equal(const CostADplex& rhs) const
{
    // compare _accid (which is the sum of nb of accids and dist)
    // and dist and TB
    return CostAD::equal(rhs);
}


// lexico (_accid = accids + dist), dist, TB
bool CostADplex::smaller(const CostADplex& rhs) const
{
    if (_accid == rhs._accid)
    {
        if (_dist == rhs._dist)
        {
            return tiebreak_smaller(rhs);
        }
        else
            return (_dist < rhs._dist);
    }
    else
        return (_accid < rhs._accid);
}


// same as CostAD::add
Cost& CostADplex::add(const CostADplex& rhs)
{
    CostAD::add(rhs);
    return *this;
}


double CostADplex::pdist(const CostADplex& rhs) const
{
    // _accid is the sum of nb of accids and dist
    return CostADplus::pdist(rhs);
}


CostType CostADplex::type() const
{
    if (_tblex)
        return CostType::ADplex;
    else
        return CostType::ADplexs;
}


std::ostream& operator<<(std::ostream& o, const CostADplex& c)
{
    c.print(o);
    return o;
}


} // end namespace pse
