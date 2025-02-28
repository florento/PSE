//
//  CostAT.cpp
//  pse
//
//  Created by Florent on 16/05/2023.
//

#include "CostAT.hpp"
//#include "PSConfig1.hpp"
//#include "PSConfig1c.hpp"
//#include "PSConfig2.hpp"

namespace pse {


CostAT::CostAT(bool tb_lex):
CostA(),
_chromharm(0),
_color(0),
_cflat(0),
_double(0),
_tbsum(0),
_tblex(tb_lex)
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


//    // return equal(dynamic_cast<const CostAT&>(rhs));
bool CostAT::equal(const CostAT& rhs) const
{
    return (CostA::equal(rhs) and tiebreak_equal(rhs));
}


bool CostAT::smaller(const CostAT& rhs) const
{
    if (CostA::equal(rhs))
        return tiebreak_smaller(rhs);
    else
        return CostA::smaller(rhs);
}


CostAT& CostAT::add(const CostAT& rhs)
{
    assert(_tbsum == _color + _cflat + _double);
    assert(rhs._tbsum == rhs._color + rhs._cflat + rhs._double);
    // add accids
    CostA::add(rhs);
    // add Tie Breaking components
    _chromharm += rhs._chromharm;
    _color += rhs._color;
    _cflat += rhs._cflat;
    _double += rhs._double;
    _tbsum += rhs._tbsum;
    return *this;
}


// is only used for selection of global (rowcost comparison)
double CostAT::pdist(const CostAT& rhs) const
{
    // ignore the tiebreaking measures (only counts accids)
    return CostA::pdist(rhs);
    
    // if (CostA::equal(rhs_AT)) // if (_accid == rhs_AT._accid)
    //     return tiebreak_pdist(rhs_AT);
    // else
    //     return CostA::pdist(rhs_AT);
    // return Cost::dist((double) _accid, (double) rhs_AT._accid);
}


bool CostAT::tiebreak_equal(const CostAT& rhs) const
{
    if (_tblex)
        return tiebreak_equal_lex0(rhs);
    else
        return tiebreak_equal_sum(rhs);
}


bool CostAT::tiebreak_smaller(const CostAT& rhs) const
{
    if (_tblex)
        return tiebreak_smaller_lex01(rhs);
    else
        return tiebreak_smaller_sum(rhs);
}


// truly lexico on all TB components
bool CostAT::tiebreak_equal_lex(const CostAT& rhs) const
{
    return (_chromharm == rhs._chromharm and
            _color == rhs._color and
            _cflat == rhs._cflat and
            _double == rhs._double);
}


// sum of Cb and doubles
bool CostAT::tiebreak_equal_lex0(const CostAT& rhs) const
{
    return (_chromharm == rhs._chromharm and
            _double + _cflat == rhs._double + rhs._cflat and
            _color == rhs._color);
}


bool CostAT::tiebreak_equal_sum(const CostAT& rhs) const
{
    assert(_tbsum == _color + _cflat + _double);
    assert(rhs._tbsum == rhs._color + rhs._cflat + rhs._double);
    return (_chromharm == rhs._chromharm and _tbsum == rhs._tbsum);
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
    

// - cflat + double
// - chromharm
// - color
bool CostAT::tiebreak_smaller_lex00(const CostAT& rhs) const
{
    if (_cflat + _double == rhs._cflat + rhs._double)
    {
        if (_chromharm == rhs._chromharm)
        {
            return (_color < rhs._color);
        }
        else
            return (_chromharm < rhs._chromharm);
    }
    else
        return (_cflat + _double < rhs._cflat + rhs._double);
}


// - cflat + double
// - color
// - chromharm
bool CostAT::tiebreak_smaller_lex01(const CostAT& rhs) const
{
    if (_cflat + _double == rhs._cflat + rhs._double)
    {
        if (_color == rhs._color)
        {
            return (_chromharm < rhs._chromharm);
        }
        else
            return (_color < rhs._color);
    }
    else
        return (_cflat + _double < rhs._cflat + rhs._double);
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
                
            case Accid::Sharp:
                if (ks < 0)
                {
                    _color += 1;
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
                    bool printed, const Ton& gton, const Ton& lton,
                    const enum NoteName& prev_name)
{
    bool ret = CostA::update(name, accid, printed, gton, lton, prev_name);

    // update only for printed accidentals
    if (printed)
    {
        ret = updateColor(name, accid, printed, gton, lton) or ret;
        ret = updateCflat(name, accid, printed, gton, lton) or ret;
        ret = updateDouble(name, accid, printed, gton, lton) or ret;
        ret = updateChroma(name, accid, printed, gton, lton) or ret;
    }
    if (ret)
        _tbsum = _color + _cflat + _double;

    return ret;
}


CostType CostAT::type() const
{
    if (_tblex)
        return CostType::ACCIDtb;
    else
        return CostType::ACCIDtbs;
}

void CostAT::printTB(std::ostream& o) const
{
    if (_tblex)
    {
        o << _color << ':';
        o << _cflat << ':';
        o << _double << ':';
        o << _chromharm;
    }
    else
    {
        o << _color << '+';
        o << _cflat << '+';
        o << _double << ':';
        o << _chromharm;
    }
}


void CostAT::print(std::ostream& o) const
{
    CostA::print(o);
    o << ':';
    printTB(o);
}


std::ostream& operator<<(std::ostream& o, const CostAT& c)
{
    c.print(o);
    return o;
}


} // end namespace pse

