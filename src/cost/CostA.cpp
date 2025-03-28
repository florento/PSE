//
//  CostA.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include <cmath> // std::abs

#include "CostA.hpp"
#include "PSConfig1.hpp"
#include "PSConfig1c.hpp"
#include "PSConfig2.hpp"

namespace pse {


CostA::CostA():
_accid(0),
_inconsist(0)
{ }


CostA::CostA(const CostA& rhs):
_accid(rhs._accid),
_inconsist(rhs._inconsist)
{ }


CostA::~CostA()
{
    TRACE("delete CostA");
}


//    const CostA& rhs_A = dynamic_cast<const CostA&>(rhs);
bool CostA::equal(const CostA& rhs) const
{
        return (_accid == rhs._accid);
}


bool CostA::smaller(const CostA& rhs) const
{
    return (_accid < rhs._accid);
}


CostA& CostA::add(const CostA& rhs)
{
    _accid += rhs._accid;
    _inconsist += rhs._inconsist;
    return *this;
}


double CostA::pdist(const CostA& rhs) const
{
    return Cost::dist((double) _accid, (double) rhs._accid);
}


bool CostA::updateAccid(const enum NoteName& name,
                        const enum Accid& accid,
                        bool printed,
                        const Ton& gton, const Ton& lton)
{
    if (printed)
    {
        switch (accid)
        {
            case Accid::DoubleSharp:
            case Accid::DoubleFlat:
                _accid += 2;
                return true;

            case Accid::Sharp:
            case Accid::Flat:
            case Accid::Natural:
                _accid += 1;
                return true;

            default:
            {
                ERROR("updateAccid: unexpected accidental"); // accid
                return false;
            }
        }
    }
    else
    {
        return false;
    }
}


bool CostA::updateInconsistency(const enum NoteName& prev_name,
                                const enum NoteName& name)
{
    if (prev_name != NoteName::Undef and prev_name != name)
    {
        _accid += 1;     // cumul accid + inconsist.
        _inconsist += 1; // inconsist. only
        return true;
    }
    else
        return false;
}


// update cost when accident for the name was updated
bool CostA::update(const enum NoteName& name, const enum Accid& accid,
                   bool print, const Ton& gton, const Ton& lton,
                   const enum NoteName& prev_name)
{
    bool reti = updateInconsistency(prev_name, name);
    bool reta = updateAccid(name, accid, print, gton, lton);
    
    return reti or reta;
}


CostType CostA::type() const
{
    // if (_discount)
    //     return CostType::ACCIDlead;
    // else
    return CostType::ACCID;
}


void CostA::print(std::ostream& o) const
{
    assert(_accid >= _inconsist);
    //    if (_inconsist > 0)
    //        DEBUG("{} INCONSISTENCIES (CostA)", _inconsist);
    o << (_accid - _inconsist) << '+' << _inconsist;
}


std::ostream& operator<<(std::ostream& o, const CostA& c)
{
    c.print(o);
    return o;
}


} // end namespace pse




// update cost when accident for the name was updated
//void CostA::update(const enum NoteName& name, const enum Accid& accid,
//                   bool print, const Ton& gton, const Ton& lton)
//{
//    // second pass
//    // @todo revise this case
//    if (lton.defined())
//    {
//        //_dist += c.state().dist(lton);
//        //if (print && !(lton.accidDia(name) == accid))
//        // si l'on veut juger purement d'un point de vue tonal
//        // afin de déduire la meilleure tonalité locale,
//        // il vaut mieux ne plus se poser la question du print :
//        // !(gton.lead()  &&  gton.accidDia(name) == accid)
//        // (lton.accidDia(name) != accid)
//        if (!Accids::contained(accid, lton.accidScale(name)))
//        {
//            updateAccid(accid);
//        }
//    }
//    // first pass
//    // @todo suppr. optional discount for lead degree
//    else if (print)
//    {
//        // !(gton.lead()  &&  gton.accidDia(name) == accid)
//        // ((! _discount)  || (gton.accidDia(name) != accid))
//        if ((! _discount)  || !Accids::contained(accid, gton.accidScale(name)))
//        {
//            updateAccid(accid);
//        }
//    }
//}


//void CostA::update(const PSC1& c, const PSEnum& e, const Ton& gton)
//{
//    // count the cost
//    // bool cc = false;
//
//    const enum NoteName& name = c.name();
//    const enum Accid& accid = c.accidental();
//
//    // update cost when accident for the name was updated
//    // discount for lead degree
//    if (c.printed() && !(gton.accidDia(name) == accid)) //!(gton.lead()
//                    &&  gton.accidDia(name) == accid)
//    {
//        switch (accid)
//        {
//            case Accid::DoubleSharp:
//            case Accid::DoubleFlat:
//                _accid += 2;
//                break;
//
//            case Accid::Sharp:
//            case Accid::Flat:
//            case Accid::Natural:
//                _accid += 1;
//                break;
//
//            default:
//            {
//                ERROR("PSC: unexpected accidental"); // accid
//                break;
//            }
//        }
//    }
//}


//void CostA::update(const PSC1& c, const PSEnum& e,
//                    const Ton& gton, const Ton& lton)
//{ }


//void CostA::update(const PSC2& c, const PSEnum& e,
//                    const enum NoteName& name, const enum Accid& accid,
//                    bool print, size_t nbocc,
//                    const Ton& gton)
//{
//    // whether the cost has to be changed
//    //    bool cc = false;
//    //    if (gton.lead(name)) // sensible
//    //        cc = print && (gton.accidDia(name) != accid);
//    //    else
//    //        cc = print;
//
//    if (print && !(gton.accidDia(name) == accid))
//    {
//        switch (accid)
//        {
//            case Accid::DoubleSharp:
//            case Accid::DoubleFlat:
//                _accid += 2*nbocc;
//                break;
//
//            case Accid::Sharp:
//            case Accid::Flat:
//            case Accid::Natural:
//                _accid += nbocc;
//                break;
//
//            default:
//            {
//                ERROR("PSC: unexpected accidental"); // accid
//                break;
//            }
//        }
//    }
//}


//void CostA::update(const PSC2& c, const PSEnum& e,
//                    const enum NoteName& name, const enum Accid& accid,
//                    bool print, size_t nbocc,
//                    const Ton& gton, const Ton& lton)
//{
//    // update number of accid
//    //bool res = updateCost(name, accid, print, nbocc, ton);;
//
//    // complete the update
//}
