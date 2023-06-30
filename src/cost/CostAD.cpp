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
_accid(0),
_dist(0),
_color(0),
_cflat(0)
{ }


CostAD::CostAD(const CostAD& rhs):
_accid(rhs._accid),
_dist(rhs._dist),
_color(rhs._color),
_cflat(rhs._cflat)
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


//bool CostAD::operator==(const CostAD& rhs) const
//{
//    return (_accid == rhs._accid && _dist == rhs._dist);
//}


CostAD& CostAD::operator+=(const CostAD& rhs)
{
    _accid += rhs._accid;
    _dist += rhs._dist;
    _color += rhs._color;
    _cflat += rhs._cflat;
    return *this;
}


//CostAD operator+(const CostAD& c1, const CostAD& c2)
//{
//    return c1.operator+(c2);
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


void CostAD::update(const enum NoteName& name,
                    const enum Accid& accid,
                    bool print,
                    const Ton& gton, const Ton& lton)
{
    bool boo = true;
    
    if ((gton.accidDia(name) != accid) &&
        gton.lead(name) && (gton.getMode() == ModeName::Minor) &&
        ((gton.accidDia(name) == Accid::Sharp && accid == Accid::Natural) ||
         (gton.accidDia(name) == Accid::Natural && accid == Accid::Flat) ||
         (gton.accidDia(name) == Accid::DoubleSharp && accid == Accid::Sharp)))
    {
        // on pénalise un peu lorsque la sensible n'est pas augmentée,
        // mais pas de 1 car il peut s'agir du mode mineur descendant
        // not used (_accid of type size_t is incremented of 0)
        _accid += 1/2;
    }
    
    // update cost when accident for the name was updated (printed)
    // discount for lead degree in minor modes
    // !(gton.lead()  &&  gton.accidDia(name) == accid)
    if ((print) && (gton.accidDia(name) != accid))
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
    
    // si l'on veut juger purement d'un point de vue tonal
    // afin de déduire la meilleure tonalité locale,
    // il vaut mieux ne plus se poser la question du print :
    // !(print  &&  lton.accidDia(name) == accid)

    // was : if (lton.defined() && print && (lton.accidDia(name) != accid))
    // no print : print flag is related to gton, not for lton
    if (lton.defined() && (lton.accidDia(name) != accid))
    {
        if (lton.lead(name) &&
            (lton.getMode() == ModeName::Minor) &&
            ((lton.accidDia(name)==Accid::Sharp && accid==Accid::Natural) ||
             (lton.accidDia(name)==Accid::Natural && accid==Accid::Flat) ||
             (lton.accidDia(name)==Accid::DoubleSharp && accid==Accid::Sharp)))
        {
            _dist += 1/2; //on pénalise un peu lorsque la sensible n'est pas augmentée,
                          // mais pas de 1 car il peut s'agir du mode mineur descendant
            boo = false;  //on a déjà traité la sensible de cette façon,
                          //pas la peine de la repénaliser par la suite
        }
        if (boo and print)
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
    
    if (print && (gton.accidDia(name) != accid) &&
        (((name == NoteName::C) && (accid == Accid::Flat)) ||
         ((name == NoteName::B) && (accid == Accid::Sharp)) ||
         ((name == NoteName::F) && (accid == Accid::Flat)) ||
         ((name == NoteName::E) && (accid == Accid::Sharp))))
    {
        ++_cflat;
    }
    
    // color of accident and color of global ton
    int ks = gton.fifths();
    // const enum Accid& a = c.accidental();
    if (((ks >= 0) && (flat(accid))) || ((ks <  0) && (sharp(accid))))
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
    if (gton.accidDia(name) != accid)
    {
        
        // not used (_accid of type size_t is incremented of 0)
        if (gton.lead(name))
        {
            if (gton.getMode()==ModeName::Minor)
            {
                if ((gton.accidDia(name)==Accid::Sharp && accid==Accid::Natural) || (gton.accidDia(name)==Accid::Natural && accid==Accid::Flat) || (gton.accidDia(name)==Accid::DoubleSharp && accid==Accid::Sharp))
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
    if (lton.defined() && print && (lton.accidDia(name) != accid))
    {
        if (lton.lead(name) &&
            (lton.getMode() == ModeName::Minor) &&
            ((lton.accidDia(name)==Accid::Sharp && accid==Accid::Natural) ||
             (lton.accidDia(name)==Accid::Natural && accid==Accid::Flat) ||
             (lton.accidDia(name)==Accid::DoubleSharp && accid==Accid::Sharp)))
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


void CostAD::print(std::ostream& o) const
{
    o << "accid=" << _accid;
    o << " dist=" << _dist;
    o << " color=" << _color;
    o << " cflat=" << _cflat;
}


std::ostream& operator<<(std::ostream& o, const CostAD& c)
{
    c.print(o);
    return o;
}


} // end namespace pse


