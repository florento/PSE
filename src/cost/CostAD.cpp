//
//  CostAD.cpp
//  pse
//
//  Created by Florent Jacquemard on 13/06/2023.
//

#include "CostAD.hpp"

#include "PSConfig1.hpp"
#include "PSConfig2.hpp"

namespace pse {


CostAD::CostAD():
_accid(0),
_dist(0)
{ }


CostAD::CostAD(const CostAD& rhs):
_accid(rhs._accid),
_dist(rhs._dist)
{ }


CostAD::~CostAD()
{
    TRACE("delete Cost_AD");
}


//CostAD& CostAD::operator=(const CostAD& rhs)
//{
//    if (this != &rhs)
//    {
//        _accid = rhs._accid;
//    }
//    return *this;
//}


bool CostAD::operator==(const CostAD& rhs) const
{
    return (_accid == rhs._accid && _dist == rhs._dist);
}


double CostAD::dist(const CostAD& rhs) const
{
    if (_accid == rhs._accid)
        return distCost((double) _dist, (double) rhs._dist);
    else
        return distCost((double) _accid, (double) rhs._accid);
}


bool CostAD::operator<(const CostAD& rhs) const
{
    if (_accid == rhs._accid)
        return (_dist < rhs._dist);
    else
        return (_accid < rhs._accid);
}


CostAD& CostAD::operator+=(const CostAD& rhs)
{
    _accid += rhs._accid;
    _dist += rhs._dist;
    return *this;
}


//CostAD operator+(const CostAD& c1, const CostAD& c2)
//{
//    return c1.operator+(c2);
//}


std::shared_ptr<Cost> CostAD::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostAD());
}


std::shared_ptr<Cost> CostAD::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostAD(*this));
}

std::unique_ptr<Cost> CostAD::unique_clone() const
{
    return std::unique_ptr<Cost>(new CostAD(*this));
}

void CostAD::update(const PSC1& c, const PSEnum& e, const Ton& gton)
{
    // count the cost
    // bool cc = false;

    const enum NoteName& name = c.name();
    const enum Accid& accid = c.accidental();
    
    // update cost when accident for the name was updated
    // discount for lead degree
    if (c.printed() && !(gton.accidDia(name) == accid))
    {
        switch (accid)
        {
            case Accid::DoubleSharp:
            case Accid::DoubleFlat:
                _accid += 2;
                break;

            case Accid::Sharp:
            case Accid::Flat:
                _accid += 1;
                break;

            case Accid::Natural:
                _accid += 1;
                break;

            default:
            {
                ERROR("PSC: unexpected accidental"); // accid
                break;
            }
        }
    }
}


void CostAD::update(const PSC1& c, const PSEnum& e,
                    const Ton& gton, const Ton& lton)
{
    _dist += c.state().dist(lton);
}


void CostAD::update(const PSC2& c, const PSEnum& e,
                    const enum NoteName& name, const enum Accid& accid,
                    bool print, size_t nbocc,
                    const Ton& gton)
{
    // whether the cost has to be changed
    //    bool cc = false;
    //    if (gton.lead(name)) // sensible
    //        cc = print && (gton.accidDia(name) != accid);
    //    else
    //        cc = print;
    
    if (print && !(gton.accidDia(name) == accid))
    {
        switch (accid)
        {
            case Accid::DoubleSharp:
            case Accid::DoubleFlat:
                _accid += 2*nbocc;
                break;
                
            case Accid::Sharp:
            case Accid::Flat:
            case Accid::Natural:
                _accid += nbocc;
                break;
                
            default:
            {
                ERROR("PSC: unexpected accidental"); // accid
                break;
            }
        }
    }
}


void CostAD::update(const PSC2& c, const PSEnum& e,
                    const enum NoteName& name, const enum Accid& accid,
                    bool print, size_t nbocc,
                    const Ton& gton, const Ton& lton)
{
    // update number of accid
    //bool res = updateCost(name, accid, print, nbocc, ton);;
    
    // complete the update
    
    // distance to conjectured local ton.
    _dist += c.state().dist(lton);
}


void CostAD::print(std::ostream& o) const
{
    o << "accid=" << _accid;
    o << " dist=" << _dist;
}


std::ostream& operator<<(std::ostream& o, const CostAD& c)
{
    c.print(o);
    return o;
}


} // end namespace pse


