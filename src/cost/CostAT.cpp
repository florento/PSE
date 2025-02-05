//
//  CostA.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include "CostAT.hpp"
//#include "PSConfig1.hpp"
//#include "PSConfig1c.hpp"
//#include "PSConfig2.hpp"

namespace pse {


CostAT::CostAT():
CostA(),
_chromharm(0),
_color(0),
_cflat(0),
_double(0)
{ }


CostAT::CostAT(const CostAT& rhs):
CostA(rhs),
_chromharm(rhs._chromharm),
_color(rhs._color),
_cflat(rhs._cflat),
_double(rhs._double)
{ }


CostAT::~CostAT()
{
    TRACE("delete CostATccid");
}


std::shared_ptr<Cost> CostAT::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostAT());
}


std::shared_ptr<Cost> CostAT::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostAT(*this));
}


std::unique_ptr<Cost> CostAT::unique_clone() const
{
    return std::unique_ptr<Cost>(new CostAT(*this));
}


//CostAT& CostAT::operator=(const CostAT& rhs)
//{
//    if (this != &rhs)
//    {
//        _accid = rhs._accid;
//    }
//    return *this;
//}



bool CostAT::equal(const CostAT& rhs) const
{
    return (CostA::equal(rhs) and tiebreak_equal(rhs));
}


bool CostAT::equal(const Cost& rhs) const
{
    return equal(dynamic_cast<const CostAT&>(rhs));
}


bool CostAT::tiebreak_equal(const CostAT& rhs) const
{
    return (_chromharm == rhs._chromharm and
            _color == rhs._color and
            _cflat == rhs._cflat and
            _double == rhs._double);
}


bool CostAT::smaller(const Cost& rhs) const
{
    const CostAT& rhs_AT = dynamic_cast<const CostAT&>(rhs);
    if (CostA::equal(rhs_AT))
        return tiebreak_smaller(rhs_AT);
    else
        return CostA::smaller(rhs_AT);
}


bool CostAT::tiebreak_smaller(const CostAT& rhs) const
{
    // - color
    // - cflat
    // - double
    // - chromharm
    if (_color == rhs._color)
    {
        if (_cflat == rhs._cflat)
        {
            if (_double == rhs._double)
                return (_chromharm < rhs._chromharm);
            else
                return (_double < rhs._double);
        }
        else
            return (_cflat < rhs._cflat);
    }
    else
        return (_color < rhs._color);
    
    // - _chromharm
    // - _color
    // - _cflat
    // if (_chromharm == rhs._chromharm)
    // {
    //     if (_color == rhs._color)
    //         return (_cflat < rhs._cflat);
    //     else
    //         return (_color < rhs._color);
    // }
    // else
    //     return (_chromharm < rhs._chromharm);
    
    // - _color
    // - _chromharm
    // - _cflat
    // if (_color == rhs._color)
    // {
    //     if (_chromharm == rhs._chromharm) // _chromharm
    //         return (_cflat < rhs._cflat);
    //     else
    //         return (_chromharm < rhs._chromharm);
    // }
    // else
    //     return (_color < rhs._color);
}


CostAT& CostAT::add(const CostAT& rhs)
{
    CostA::add(rhs);
    _chromharm += rhs._chromharm;
    _color += rhs._color;
    _cflat += rhs._cflat;
    _double += rhs._double;
    return *this;
}


Cost& CostAT::add(const Cost& rhs)
{
    return add(dynamic_cast<const CostAT&>(rhs));
}


// is only used for selection of global (rowcost comparison)
double CostAT::pdist(const Cost& rhs) const
{
    const CostAT& rhs_AT = dynamic_cast<const CostAT&>(rhs);
    // ignore the tiebreaking measures (only counts accids)
    return CostA::pdist(rhs_AT);
    // if (CostA::equal(rhs_AT)) // if (_accid == rhs_AT._accid)
    //     return tiebreak_pdist(rhs_AT);
    // else
    //     return CostA::pdist(rhs_AT);
    // return Cost::dist((double) _accid, (double) rhs_AT._accid);
}


/// @todo TBR. sum of differences?
double CostAT::tiebreak_pdist(const CostAT& rhs) const
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


bool CostAT::updateChroma(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& gton, const Ton& lton)
{
    // count accid not in the chromatic harmonic scale
    assert(gton.defined());
    if (print and
        not Accids::contained(accid, gton.chromaton().accidScale(name)))
    {
        _chromharm += 1;
        return true;
    }
    else
    {
        return false;
    }
}


bool CostAT::updateColor(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // color of accident differs from color of global ton
    assert(gton.defined());
    int ks = gton.fifths();
    // const enum Accid& a = c.accidental();
    if (print and
        (not Accids::contained(accid, gton.accidScale(name))) and
        (((ks >= 0) and flat(accid)) or ((ks <= 0) and sharp(accid))))
    {
        _color += 1;
        return true;
    }
    else
    {
        return false;
    }
}


bool CostAT::updateCflat(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // count  Cb B# E# Fb.
    assert(gton.defined());
    if (print and
        (not Accids::contained(accid, gton.accidScale(name))) and
        (((name == NoteName::C) && (accid == Accid::Flat)) or
         ((name == NoteName::B) && (accid == Accid::Sharp)) or
         ((name == NoteName::F) && (accid == Accid::Flat)) or
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


bool CostAT::updateDouble(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& gton, const Ton& lton)
{
    assert(gton.defined());
    if (print and
        (not Accids::contained(accid, gton.accidScale(name))) and
        ((accid == Accid::DoubleFlat) or (accid == Accid::DoubleSharp)))
    {
        ++_double;
        return true;
    }
    else
    {
        return false;
    }
}


// update cost when accident for the name was updated
bool CostAT::update(const enum NoteName& name, const enum Accid& accid,
                   bool print, const Ton& gton, const Ton& lton)
{
    bool reta = CostA::update(name, accid, print, gton, lton);
    bool retc = updateChroma(name, accid, print, gton, lton);
    bool reto = updateColor(name, accid, print, gton, lton);
    bool retf = updateCflat(name, accid, print, gton, lton);
    bool retd = updateDouble(name, accid, print, gton, lton);
    return reta or retc or reto or retf or retd;
}


CostType CostAT::type() const
{
        return CostType::ACCIDtb;
}


void CostAT::print(std::ostream& o) const
{
    o << _accid << ':';
    o << _chromharm << ':';
    o << _color << ':';
    o << _cflat;
}


std::ostream& operator<<(std::ostream& o, const CostAT& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

