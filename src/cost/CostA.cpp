//
//  CostA.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include "CostA.hpp"
#include "PSConfig1.hpp"
#include "PSConfig2.hpp"

namespace pse {


CostA::CostA():
_accid(0)
{}


CostA::CostA(const CostA& rhs):
_accid(rhs._accid)
{}


CostA::~CostA()
{
    TRACE("delete Cost_accid");
}


//CostA& CostA::operator=(const CostA& rhs)
//{
//    if (this != &rhs)
//    {
//        _accid = rhs._accid;
//    }
//    return *this;
//}


bool CostA::operator==(const CostA& rhs) const
{
    return (_accid == rhs._accid);
}


double CostA::dist(const CostA& rhs) const
{
    return distCost((double) _accid, (double) rhs._accid);
}


bool CostA::operator<(const CostA& rhs) const
{
    return (_accid < rhs._accid);
}


CostA& CostA::operator+=(const CostA& rhs)
{
    _accid += rhs._accid;
    return *this;
}


//CostA operator+(const CostA& c1, const CostA& c2)
//{
//    return c1.operator+(c2);
//}


std::shared_ptr<Cost> CostA::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostA());
}


std::shared_ptr<Cost> CostA::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostA(*this));
}

std::unique_ptr<Cost> CostA::unique_clone() const
{
    return std::unique_ptr<Cost>(new CostA(*this));
}

void CostA::update(const PSC1& c, const PSEnum& e, const Ton& gton)
{
    // count the cost
    // bool cc = false;

    const enum NoteName& name = c.name();
    const enum Accid& accid = c.accidental();
    
    // update cost when accident for the name was updated
    // discount for lead degree
    if (c.printed() && !(gton.accidDia(name) == accid)) //!(gton.lead()  &&  gton.accidDia(name) == accid)
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


void CostA::update(const PSC1& c, const PSEnum& e,
                    const Ton& gton, const Ton& lton)
{ }


void CostA::update(const PSC2& c, const PSEnum& e,
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
    
    if (print && !(gton.lead(name) && gton.accidDia(name) == accid))
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


void CostA::update(const PSC2& c, const PSEnum& e,
                    const enum NoteName& name, const enum Accid& accid,
                    bool print, size_t nbocc,
                    const Ton& gton, const Ton& lton)
{
    // update number of accid
    //bool res = updateCost(name, accid, print, nbocc, ton);;
    
    // complete the update
}


void CostA::print(std::ostream& o) const
{
    o << "accid=" << _accid;
}


std::ostream& operator<<(std::ostream& o, const CostA& c)
{
    c.print(o);
    return o;
}


} // end namespace pse


