//
//  CostADlex.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADlex.hpp"

namespace pse {


CostADlex::CostADlex():
CostAD()
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
    return std::shared_ptr<Cost>(new CostADlex());
}


std::shared_ptr<Cost> CostADlex::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostADlex(*this));
}


//std::unique_ptr<Cost> CostADlex::unique_clone() const
//{
//    return std::unique_ptr<Cost>(new CostADlex(*this));
//}


bool CostADlex::operator==(const CostADlex& rhs) const
{
    return CostAD::operator==(rhs);
}


double CostADlex::dist(const CostADlex& rhs) const
{
    if (_accid == rhs._accid)
    {
        if (_dist == rhs._dist)
        {
            if (_chromharm == rhs._chromharm)
            {
                
                if (_color == rhs._color)
                    return distCost((double) _cflat, (double) rhs._cflat);
                else
                    return distCost((double) _color, (double) rhs._color);
            }
            else
                return distCost((double)_chromharm , (double) rhs._chromharm);
        }
        else
            return distCost((double) _dist, (double) rhs._dist);
    }
    else
        return distCost((double) _accid, (double) rhs._accid);
}


bool CostADlex::operator<(const CostADlex& rhs) const
{
    if (_accid == rhs._accid)
    {
        if (_dist == rhs._dist)
        {
            if (_chromharm == rhs._chromharm)
            {
                if (_color == rhs._color)
                    return (_cflat < rhs._cflat);
                else
                    return (_color < rhs._color);
            }
            else
                return (_chromharm < rhs._chromharm);
        }
        else
            return (_dist < rhs._dist);
    }
    else
        return (_accid < rhs._accid);
}


CostADlex& CostADlex::operator+=(const CostADlex& rhs)
{
    CostAD::operator+=(rhs);
    return *this;
}


void CostADlex::update(const enum NoteName& name, const enum Accid& accid,
                       bool print,
                       const Ton& gton, const Ton& lton)
{
    CostAD::update(name, accid, print, gton, lton);
}


CostType CostADlex::type() const
{
    return CostType::ADlex;
}


void CostADlex::print(std::ostream& o) const
{
    CostAD::print(o);
}


std::ostream& operator<<(std::ostream& o, const CostADlex& c)
{
    c.CostAD::print(o);
    return o;
}

} // end namespace pse
