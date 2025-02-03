//
//  CostAD.cpp
//  pse
//
//  Created by Florent Jacquemard on 13/06/2023.
//

#include "CostAD.hpp"

//#include "PSConfig1.hpp"
//#include "PSConfig1c.hpp"
//#include "PSConfig2.hpp"

namespace pse {


CostAD::CostAD():
CostA(),
_dist(0)
{ }


CostAD::CostAD(const CostAD& rhs):
CostA(rhs),
_dist(rhs._dist)
{ }


CostAD::~CostAD()
{
    TRACE("delete CostAD");
}


//CostAD& CostAD::operator=(const CostAD& rhs)
//{
//    if (this != &rhs)
//    {
//        _accid = rhs._accid;
//    }
//    return *this;
//}


//std::shared_ptr<Cost> CostAD::shared_zero() const
//{
//    return std::shared_ptr<Cost>(new CostAD());
//}


//std::shared_ptr<Cost> CostAD::shared_clone() const
//{
//    return std::shared_ptr<Cost>(new CostAD(*this));
//}

//std::unique_ptr<Cost> CostAD::unique_clone() const
//{
//    return std::unique_ptr<Cost>(new CostAD(*this));
//}


bool CostAD::equal(const Cost& rhs) const
{
    const CostAD& rhs_AD = dynamic_cast<const CostAD&>(rhs);
    return (CostA::equal(rhs_AD) and _dist == rhs_AD._dist);
}


Cost& CostAD::add(const Cost& rhs)
{
    const CostAD& rhs_AD = dynamic_cast<const CostAD&>(rhs);
    CostA::add(rhs_AD);
    _dist += rhs_AD._dist;
    return *this;
}


bool CostAD::updateDist(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // count accidental different from lton
    if (lton.defined() and (!Accids::contained(accid, lton.accidScale(name))))
    {
        switch (accid)
        {
            case Accid::DoubleSharp:
            case Accid::DoubleFlat:
                _dist += 2;
                break;

            case Accid::Sharp:
            case Accid::Flat:
            case Accid::Natural:
                _dist += 1;
                break;

            default:
            {
                ERROR("updateDist: unexpected accidental"); // accid
                break;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}


bool CostAD::updateChroma(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // count accid not in the chromatic harmonic scale
    //    if (print && !(lton.chromatic().contains(name,accid)))
    if (lton.undef())
    {
        return CostA::updateChroma(name, accid, print, gton, lton);
    }
    else if (print and
             !Accids::contained(accid, lton.chromaton().accidScale(name)))
    {
        _chromharm += 1;
        return true;
    }
    else
    {
        return false;
    }
}


bool CostAD::update(const enum NoteName& name,
                    const enum Accid& accid,
                    bool print,
                    const Ton& gton, const Ton& lton)
{
    bool reta = CostA::update(name, accid, print, gton, lton); // updateAccid
    bool retd = updateDist(name, accid, print, gton, lton);

    return reta or retd;
}



void CostAD::print(std::ostream& o) const
{
    o << "acc=" << _accid;
    o << " dst=" << _dist;
    o << "chr=" << _chromharm;
    o << " col=" << _color;
    o << " cf=" << _cflat;
}


std::ostream& operator<<(std::ostream& o, const CostAD& c)
{
    c.print(o);
    return o;
}


// version (TENOR paper)
void CostAD::update_tonale(const enum NoteName& name,
                           const enum Accid& accid,
                           bool print,
                           const Ton& gton, const Ton& lton)
{
    bool boo = true;
    
    // update cost when accident for the name was updated (printed)
    // discount for lead degree in minor modes
    // && (gton.accidDia(name) != accid))
    // else if (print && ((!gton.lead(name)) || gton.accidDia(name) != accid))
    // !(gton.lead()  &&  gton.accidDia(name) == accid)
    
    //if (gton.getMode() == ModeName::Minor)
    //{
    //    if ((gton.accidDia(name, ModeName::Minor) != accid) && (gton.accidDia(name, ModeName::MinorNat)==accid || gton.accidDia(name, ModeName::MinorMel)==accid))
    //    {
    //        _accid += 1;
    //        boo=false;
    //    }
    //}
    
    if ((print) && (!Accids::contained(accid, gton.accidScale(name)))) //&& (boo))
    {
        switch (accid)
        {
            case Accid::DoubleSharp:
            case Accid::DoubleFlat:
                _accid += 2;
                //_accid += 3;
                break;
                
            case Accid::Sharp:
            case Accid::Flat:
            case Accid::Natural:
                _accid += 1;
                //_accid += 2;
                break;
                    
            default:
            {
                ERROR("PSC: unexpected accidental"); // accid
                break;
            }
        }
    }
    /*if (lton.defined() && (lton.accidDia(name) == accid))
    {
        if ((accid == Accid::DoubleSharp) || (accid == Accid::DoubleFlat))
        {
            if (_accid>1)
            {
                _accid -= 1;
            }
        }
    }*/
    
    // si l'on veut juger purement d'un point de vue tonal
    // afin de déduire la meilleure tonalité locale,
    // il vaut mieux ne plus se poser la question du print :
    // !(print  &&  lton.accidDia(name) == accid)

    // was : if (lton.defined() && print && (lton.accidDia(name) != accid))
    // no print : print flag is related to gton, not for lton
    
    //boo=true
    if (lton.defined() && (!Accids::contained(accid, lton.accidScale(name))))
    {
        if (lton.getMode() == ModeName::Minor)
        {
            if (lton.accidDia(name, ModeName::MinorNat)==accid || 
                lton.accidDia(name, ModeName::MinorMel)==accid)
            {
                //_dist += 1;
                boo=false;
            }
        }
        if (boo)
        {
            switch (accid)
            {
                case Accid::DoubleSharp:
                case Accid::DoubleFlat:
                    _dist += 2;
                    break;

                case Accid::Sharp:
                case Accid::Flat:
                case Accid::Natural:
                    _dist += 1;
                    break;

                default:
                {
                    ERROR("PSC: unexpected accidental"); // accid
                    break;
                }
            }
        }
    }
    
    // if (print && !(lton.chromatic().contains(name,accid)))
    if (print && !Accids::contained(accid, lton.chromaton().accidScale(name)))
    {
        _chromharm += 1;
    }
    
    if (print && (!Accids::contained(accid, gton.accidScale(name))) &&
            (((name == NoteName::C) && (accid == Accid::Flat))  ||
             ((name == NoteName::B) && (accid == Accid::Sharp)) ||
             ((name == NoteName::F) && (accid == Accid::Flat))  ||
             ((name == NoteName::E) && (accid == Accid::Sharp))))
    {
        ++_cflat;
    }
    
    // color of accident and color of global ton
    int ks = gton.fifths();
    // const enum Accid& a = c.accidental();
    if (((ks >= 0) && (flat(accid))) || ((ks <=  0) && (sharp(accid))))
    {
        _color += 1;
    }
}


// previous version of update
void CostAD::update99(const enum NoteName& name,
                      const enum Accid& accid,
                      bool print,
                      const Ton& gton, const Ton& lton)
{
    bool boo = true;
    
    // update cost when accident for the name was updated
    // discount for lead degree
    // !(gton.lead()  &&  gton.accidDia(name) == accid)
    if (!Accids::contained(accid, gton.accidScale(name)))
    {
        
        // not used (_accid of type size_t is incremented of 0)
        if (gton.lead(name))
        {
            if (gton.getMode()==ModeName::Minor)
            {
                if ((gton.accidDia(name) == Accid::Sharp && accid==Accid::Natural) || (gton.accidDia(name) == Accid::Natural && accid==Accid::Flat) || (gton.accidDia(name) == Accid::DoubleSharp && accid==Accid::Sharp))
                {
                    _accid += 1/2;//on pénalise un peu lorsque la sensible n'est pas augmentée, mais pas de 1 car il peut s'agir du mode mineur descendant
                }
            }
        }
        
        if (print)
        {
            switch (accid)
            {
                case Accid::DoubleSharp:
                case Accid::DoubleFlat:
                    _accid += 2;
                    break;
                    
                case Accid::Sharp:
                case Accid::Flat:
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
        
        if (((name == NoteName::C) && (accid == Accid::Flat)) ||
            ((name == NoteName::B) && (accid == Accid::Sharp)) ||
            ((name == NoteName::F) && (accid == Accid::Flat)) ||
            ((name == NoteName::E) && (accid == Accid::Sharp)))
        {
            ++_cflat;
        }
    }
    
    // si l'on veut juger purement d'un point de vue tonal
    // afin de déduire la meilleure tonalité locale,
    // il vaut mieux ne plus se poser la question du print :
    // !(print  &&  lton.accidDia(name) == accid)

    // was : if (lton.defined() && print && (lton.accidDia(name) != accid))
    // no print : print flag is related to gton, not for lton
    if (lton.defined() && print && (!Accids::contained(accid, lton.accidScale(name))))
    {
        if (lton.lead(name) &&
            (lton.getMode() == ModeName::Minor) &&
            ((lton.accidDia(name) == Accid::Sharp && accid==Accid::Natural) ||
             (lton.accidDia(name) == Accid::Natural && accid==Accid::Flat) ||
             (lton.accidDia(name) == Accid::DoubleSharp && accid==Accid::Sharp)))
        {
            _dist += 1/2; //on pénalise un peu lorsque la sensible n'est pas augmentée,
                          // mais pas de 1 car il peut s'agir du mode mineur descendant
            boo = false;  //on a déjà traité la sensible de cette façon,
                          //pas la peine de la repénaliser par la suite
        }
        if (boo)
        {
            switch (accid)
            {
                case Accid::DoubleSharp:
                case Accid::DoubleFlat:
                    _dist += 2;
                    break;

                case Accid::Sharp:
                case Accid::Flat:
                case Accid::Natural:
                    _dist += 1;
                    break;

                default:
                {
                    ERROR("PSC: unexpected accidental"); // accid
                    break;
                }
            }
        }
    }
    
    // color of accident and color of global ton
    int ks = gton.fifths();
    // const enum Accid& a = c.accidental();
    if (((ks >= 0) && (flat(accid))) || ((ks <  0) && (sharp(accid))))
    {
        _color += 1;
    }
}


} // end namespace pse



//void CostAD::update(const PSC1& c, const PSEnum& e, const Ton& gton)
//{
//    // count the cost
//    // bool cc = false;
//
//    const enum NoteName& name = c.name();
//    const enum Accid& accid = c.accidental();
//
//    // update cost when accident for the name was updated
//    // discount for lead degree
//    if (c.printed() && !(gton.accidDia(name) == accid))
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
//                _accid += 1;
//                break;
//
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


//void CostAD::update(const PSC1& c, const PSEnum& e,
//                    const Ton& gton, const Ton& lton)
//{
//    _dist += c.state().dist(lton);
//}


//void CostAD::update(const PSC2& c, const PSEnum& e,
//                    const enum NoteName& name, const enum Accid& accid,
//                    bool print, size_t nbocc,
//                    const Ton& gton)
//{
//    // whether the cost has to be changed
//    //    bool cc = false;
//    //    if (gton.lead(name)) // sensible
//    //        cc = print && (!Accids::contained(accid, gton.accidScale(name)));
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


//void CostAD::update(const PSC2& c, const PSEnum& e,
//                    const enum NoteName& name, const enum Accid& accid,
//                    bool print, size_t nbocc,
//                    const Ton& gton, const Ton& lton)
//{
//    // update number of accid
//    //bool res = updateCost(name, accid, print, nbocc, ton);;
//
//    // complete the update
//
//    // distance to conjectured local ton.
//    _dist += c.state().dist(lton);
//}
