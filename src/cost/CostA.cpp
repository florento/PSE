//
//  CostA.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include "CostA.hpp"
#include "PSConfig1.hpp"
#include "PSConfig1c.hpp"
#include "PSConfig2.hpp"

namespace pse {


CostA::CostA(): // bool discount
_accid(0),
_chromharm(0),
_color(0),
_cflat(0)
{ }


CostA::CostA(const CostA& rhs):
_accid(rhs._accid),
_chromharm(rhs._chromharm),
_color(rhs._color),
_cflat(rhs._cflat)
{ }


CostA::~CostA()
{
    TRACE("delete CostAccid");
}


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


//CostA& CostA::operator=(const CostA& rhs)
//{
//    if (this != &rhs)
//    {
//        _accid = rhs._accid;
//    }
//    return *this;
//}


bool CostA::equal(const CostA& rhs) const
{
    return (_accid == rhs._accid and tiebreak_equal(rhs));
}


bool CostA::equal(const Cost& rhs) const
{
    return equal(dynamic_cast<const CostA&>(rhs));
}


bool CostA::tiebreak_equal(const CostA& rhs) const
{
    return (_chromharm == rhs._chromharm and
            _color == rhs._color and
            _cflat == rhs._cflat);
}


bool CostA::smaller(const Cost& rhs) const
{
    const CostA& rhs_A = dynamic_cast<const CostA&>(rhs);
    if (_accid == rhs_A._accid)
    {
        return tiebreak_smaller(rhs_A);
    }
    else
        return (_accid < rhs_A._accid);    
}


bool CostA::tiebreak_smaller(const CostA& rhs) const
{
    if (_chromharm == rhs._chromharm)
    {
        if (_color == rhs._color)
            return (_cflat < rhs._cflat);
        else
            return (_color < rhs._color);
        // return (_color+_cflat < rhs._cflat+rhs._color);
    }
    else
        return (_chromharm < rhs._chromharm);
}


CostA& CostA::add(const CostA& rhs)
{
    _accid += rhs._accid;
    _chromharm += rhs._chromharm;
    _color += rhs._color;
    _cflat += rhs._cflat;
    return *this;
}


Cost& CostA::add(const Cost& rhs)
{
    return add(dynamic_cast<const CostA&>(rhs));
}


double CostA::pdist(const Cost& rhs) const
{
    const CostA& rhs_A = dynamic_cast<const CostA&>(rhs);
    if (_accid == rhs_A._accid)
        return tiebreak_pdist(rhs_A);
    else
        return Cost::dist((double) _accid, (double) rhs_A._accid);
    
}


/// @todo TBR. sum of differences?
double CostA::tiebreak_pdist(const CostA& rhs) const
{
    if (_chromharm == rhs._chromharm)
    {
        if (_color == rhs._color)
            return Cost::dist((double) _cflat, (double) rhs._cflat);
        else
            return Cost::dist((double) _color, (double) rhs._color);
    }
    else
    {
        return Cost::dist((double)_chromharm , (double) rhs._chromharm);
    }
}


bool CostA::updateAccid(const enum NoteName& name,
                        const enum Accid& accid,
                        bool print,
                        const Ton& gton, const Ton& lton)
{
    if (print)
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

bool CostA::updateChroma(const enum NoteName& name, const enum Accid& accid,
                         bool print, const Ton& gton, const Ton& lton)
{
    // count accid not in the chromatic harmonic scale
    assert(gton.defined());
    if (print && !Accids::contained(accid, gton.chromaton().accidScale(name)))
    {
        _chromharm += 1;
        return true;
    }
    else
    {
        return false;
    }
}


bool CostA::updateColor(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // color of accident differs from color of global ton
    assert(gton.defined());
    int ks = gton.fifths();
    // const enum Accid& a = c.accidental();
    if (((ks >= 0) && (flat(accid))) || ((ks <=  0) && (sharp(accid))))
    {
        _color += 1;
        return true;
    }
    else
    {
        return false;
    }
}


bool CostA::updateCflat(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // count  Cb B# E# Fb.
    assert(gton.defined());
    if (print && (!Accids::contained(accid, gton.accidScale(name))) &&
            (((name == NoteName::C) && (accid == Accid::Flat)) ||
             ((name == NoteName::B) && (accid == Accid::Sharp)) ||
             ((name == NoteName::F) && (accid == Accid::Flat)) ||
             ((name == NoteName::E) && (accid == Accid::Sharp))))
    {
        ++_cflat;
        return true;
    }
    else
    {
        return false;
    }
}


// update cost when accident for the name was updated
bool CostA::update(const enum NoteName& name, const enum Accid& accid,
                   bool print, const Ton& gton, const Ton& lton)
{
    bool reta = CostA::updateAccid(name, accid, print, gton, lton);
    bool retc = updateChroma(name, accid, print, gton, lton);
    bool reto = updateColor(name, accid, print, gton, lton);
    bool retf = updateCflat(name, accid, print, gton, lton);
    return reta or retc or reto or retf;
}


CostType CostA::type() const
{
//    if (_discount)
//        return CostType::ACCIDlead;
//    else
        return CostType::ACCID;
}


void CostA::print(std::ostream& o) const
{
    o << _accid;
    // o << "acc=" << _accid;
    // o << "chr=" << _chromharm;
    // o << " col=" << _color;
    // o << " cf=" << _cflat;
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
