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


CostAT::CostAT(bool approx, bool tb_sum):
CostA(approx),
_chromharm(0),
_color(0),
_cflat(0),
_double(0),
_tbsum(0),
_tblex(!tb_sum)
{ }


CostAT::CostAT(const CostAT& rhs):
CostA(rhs),
_chromharm(rhs._chromharm),
_color(rhs._color),
_cflat(rhs._cflat),
_double(rhs._double),
_tbsum(rhs._tbsum),
_tblex(rhs._tblex)
{ }


CostAT::~CostAT()
{
    TRACE("delete CostATccid");
}


std::shared_ptr<Cost> CostAT::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostAT(this->_approx, this->_tblex));
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
    if (_tblex)
        return tiebreak_equal_lex(rhs);
    else
        return tiebreak_equal_sum(rhs);
}


bool CostAT::tiebreak_equal_lex(const CostAT& rhs) const
{
    return (_chromharm == rhs._chromharm and
            _color == rhs._color and
            _cflat == rhs._cflat and
            _double == rhs._double);
}


bool CostAT::tiebreak_equal_sum(const CostAT& rhs) const
{
    assert(_tbsum == _color + _cflat + _double);
    assert(rhs._tbsum == rhs._color + rhs._cflat + rhs._double);
    return (_chromharm == rhs._chromharm and _tbsum == rhs._tbsum);
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
    if (_tblex)
        return tiebreak_smaller_lex1(rhs);
    else
        return tiebreak_smaller_sum(rhs);
}

bool CostAT::tiebreak_smaller_sum(const CostAT& rhs) const
{
    assert(_tbsum == _color + _cflat + _double);
    assert(rhs._tbsum == rhs._color + rhs._cflat + rhs._double);
    if (_tbsum == rhs._tbsum)
                return (_chromharm < rhs._chromharm);
    else
        return (_tbsum < rhs._tbsum);
}
    

// - color
// - cflat
// - double
// - chromharm
bool CostAT::tiebreak_smaller_lex1(const CostAT& rhs) const
{
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
}


// - _chromharm
// - _color
// - _cflat
// - double
bool CostAT::tiebreak_smaller_lex2(const CostAT& rhs) const
{
    
    if (_chromharm == rhs._chromharm)
    {
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
    }
    else
        return (_chromharm < rhs._chromharm);
}



CostAT& CostAT::add(const CostAT& rhs)
{
    assert(_tbsum == _color + _cflat + _double);
    assert(rhs._tbsum == rhs._color + rhs._cflat + rhs._double);
    CostA::add(rhs);
    _chromharm += rhs._chromharm;
    _color += rhs._color;
    _cflat += rhs._cflat;
    _double += rhs._double;
    _tbsum += rhs._tbsum;
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
/// @todo not used. see pdist
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


bool CostAT::updateChroma1(const enum NoteName& name, const enum Accid& accid,
                           bool print, const Ton& ton)
{
    // count accid not in the chromatic harmonic scale of given ton
    assert(ton.defined());
    if (not Accids::contained(accid, ton.chromaton().accidScale(name)))
    {
        _chromharm += 1;
        return true;
    }
    else
    {
        return false;
    }
}
    
    
bool CostAT::updateChroma(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& gton, const Ton& lton)
{
    // count accid not in the chromatic harmonic scale of global ton
    if (lton.undef())
    {
        assert(gton.defined());
        return updateChroma1(name, accid, print, gton);
    }
    // count accid not in the chromatic harmonic scale of local ton
    else
    {
        return updateChroma1(name, accid, print, lton);
    }
}


bool CostAT::updateColor1(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& ton)
{
    // color of accident differs from color of given ton
    assert(ton.defined());
    int ks = ton.fifths();
    // discount for lead
    if (not Accids::contained(accid, ton.accidScale(name)))
    {
        switch (accid)
        {
            case Accid::DoubleSharp:
                if (ks < 0)
                {
                    _color += 2;
                    return true;
                }
                else
                    return false;
                
            case Accid::DoubleFlat:
                if (ks > 0)
                {
                    _color += 2;
                    return true;
                }
                else
                    return false;

            case Accid::Sharp:
                if (ks < 0)
                {
                    _color += 1;
                    return true;
                }
                else
                    return false;

            case Accid::Flat:
                if (ks > 0)
                {
                    _color += 1;
                    return true;
                }
                else
                    return false;

            case Accid::Natural:
                return false;

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


bool CostAT::updateColor(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // count accid with different color than global ton
    if (lton.undef())
    {
        assert(gton.defined());
        return updateColor1(name, accid, print, gton);
    }
    // count accid with different color than local ton
    else
    {
        return updateColor1(name, accid, print, lton);
    }
}


bool CostAT::updateCflat1(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& ton)
{
    // count  Cb B# E# Fb not in given ton
    assert(ton.defined());
    if ((not Accids::contained(accid, ton.accidScale(name))) and
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


bool CostAT::updateCflat(const enum NoteName& name, const enum Accid& accid,
                        bool print, const Ton& gton, const Ton& lton)
{
    // count Cb B# E# Fb not in global ton
    if (lton.undef())
    {
        assert(gton.defined());
        return updateCflat1(name, accid, print, gton);
    }
    // count Cb B# E# Fb not in local ton
    else
    {
        return updateCflat1(name, accid, print, lton);
    }
}


bool CostAT::updateDouble1(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& ton)
{
    assert(ton.defined());
    if ((not Accids::contained(accid, ton.accidScale(name))) and
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


bool CostAT::updateDouble(const enum NoteName& name, const enum Accid& accid,
                          bool print, const Ton& gton, const Ton& lton)
{
    // count double alteration not in global ton
    if (lton.undef())
    {
        assert(gton.defined());
        return updateDouble1(name, accid, print, gton);
    }
    // count double alteration not in glocal ton
    else
    {
        return updateDouble1(name, accid, print, lton);
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
    _tbsum = _color + _cflat + _double;
    return reta or retc or reto or retf or retd;
}


CostType CostAT::type() const
{
        return CostType::ACCIDtb;
}


void CostAT::print(std::ostream& o) const
{
    o << _accid << ':';
    o << _color << ':';
    o << _cflat << ':';
    o << _double << ':';
    o << _chromharm;
}


std::ostream& operator<<(std::ostream& o, const CostAT& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

