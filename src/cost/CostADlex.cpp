//
//  CostADlex.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADlex.hpp"

namespace pse {


CostADlex::CostADlex(bool approx, bool tb_sum):
CostAD(approx, tb_sum)
{ }


CostADlex::CostADlex(const CostADlex& rhs):
CostAD(rhs)
{ }


CostADlex::~CostADlex()
{
    TRACE("delete Cost_ADlex");
}


std::shared_ptr<Cost> CostADlex::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostADlex(this->_approx, this->_tblex));
}


std::shared_ptr<Cost> CostADlex::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostADlex(*this));
}


//std::unique_ptr<Cost> CostADlex::unique_clone() const
//{
//    return std::unique_ptr<Cost>(new CostADlex(*this));
//}


//bool CostADlex::equal(const CostADlex& rhs) const
//{
//    return CostAD::equal(rhs);
//}


bool CostADlex::smaller(const Cost& rhs) const
{
    const CostADlex& rhs_ADlex = dynamic_cast<const CostADlex&>(rhs);
    if (_accid == rhs_ADlex._accid)
    {
        if (_dist == rhs_ADlex._dist)
        {
            return tiebreak_smaller(rhs_ADlex);
        }
        else
            return (_dist < rhs_ADlex._dist);
    }
    else
        return (_accid < rhs_ADlex._accid);
}


Cost& CostADlex::add(const Cost& rhs)
{
    // const CostADlex& rhs_ADlex = dynamic_cast<const CostADlex&>(rhs);
    CostAD::add(rhs);
    return *this;
}


/// @todo TBR sum of dists ?
double CostADlex::pdist(const Cost& rhs) const
{
    const CostADlex& rhs_ADlex = dynamic_cast<const CostADlex&>(rhs);

    // ignore the tiebreaking measures (only counts accids + dist)
    return Cost::dist((double) _accid + _dist,
                      (double) rhs_ADlex._accid + rhs_ADlex._dist);

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
    return CostType::ADlex;
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
