//
//  CostADlex.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADlex.hpp"

namespace pse {


CostADlex::CostADlex(bool tb_sum):
CostAD(tb_sum)
{ }


CostADlex::CostADlex(const CostADlex& rhs):
CostAD(rhs)
{ }


CostADlex::~CostADlex()
{
    TRACE("delete Cost_ADlex");
}


// same as CostAD::equal
bool CostADlex::equal(const CostADlex& rhs) const
{
    return CostAD::equal(rhs);
}


bool CostADlex::smaller(const CostADlex& rhs) const
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
 Cost& CostADlex::add(const CostADlex& rhs)
 {
     CostAD::add(rhs);
     return *this;
 }


/// @todo TBR sum of dists ?
double CostADlex::pdist(const CostADlex& rhs) const
{
    // ignore the tiebreaking measures (only counts accids + dist)
    return Cost::dist((double) _accid + _dist, (double) rhs._accid + rhs._dist);

    // if (_accid == rhs_ADlex._accid)
    // {
    //     if (_dist == rhs_ADlex._dist)
    //     {
    //         return tiebreak_pdist(rhs_ADlex);
    //     }
    //     else
    //         return Cost::dist((double) _dist, (double) rhs_ADlex._dist);
    // }
    // else
    //     return Cost::dist((double) _accid, (double) rhs_ADlex._accid);
}


//bool CostADlex::update(const enum NoteName& name, const enum Accid& accid,
//                       bool print, const Ton& gton, const Ton& lton)
//{
//    return CostAD::update(name, accid, print, gton, lton);
//}


CostType CostADlex::type() const
{
    if (_tblex)
        return CostType::ADlex;
    else
        return CostType::ADlexs;
}


void CostADlex::print(std::ostream& o) const
{
    o << _accid << ":";
    o << _dist << ':';
    o << _color << ':';
    o << _cflat << ':';
    o << _double << ':';
    o << _chromharm;
}


std::ostream& operator<<(std::ostream& o, const CostADlex& c)
{
    c.print(o);
    return o;
}

} // end namespace pse
