//
//  CostADplus.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/06/2023.
//

#include "CostADplus.hpp"

namespace pse {


CostADplus::CostADplus():
CostAD(),
_sum(_accid + _dist)
{ }


CostADplus::CostADplus(const CostADplus& rhs):
CostAD(rhs),
_sum(rhs._sum)
{
    assert(_sum == _accid + _dist);
}


CostADplus::~CostADplus()
{
    TRACE("delete CostADplus");
}


bool CostADplus::operator==(const CostADplus& rhs) const
{
    assert(_sum == _accid + _dist);
    assert(rhs._sum == rhs._accid + rhs._dist);
    return (_sum == rhs._sum &&
            _chromharm == rhs._chromharm &&
            _color+_cflat == rhs._cflat+rhs._color);
    //        _color == rhs._color &&
    //        _cflat == rhs._cflat);
}


double CostADplus::dist(const CostADplus& rhs) const
{
    assert(_sum == _accid + _dist);
    assert(rhs._sum == rhs._accid + rhs._dist);
    if (_sum == rhs._sum)
    {
        if (_chromharm == rhs._chromharm) return distCost((double)(_color+_cflat), (double)(rhs._cflat+rhs._color));
        else return distCost((double)_chromharm , (double) rhs._chromharm);
        /*
        if (_color == rhs._color)
            return distCost((double) _cflat, (double) rhs._cflat);
        else
            return distCost((double) _color, (double) rhs._color);
        */
    }
    else
        return distCost((double) _sum, (double) rhs._sum);
}


bool CostADplus::operator<(const CostADplus& rhs) const
{
    assert(_sum == _accid + _dist);
    assert(rhs._sum == rhs._accid + rhs._dist);
    if (_sum == rhs._sum)
    {
        if (_chromharm == rhs._chromharm) return (_color+_cflat < rhs._cflat+rhs._color);
        else return (_chromharm < rhs._chromharm);
        /*
        if (_color == rhs._color)
            return (_cflat < rhs._cflat);
        else
            return (_color < rhs._color);
        */
    }
    else
        return (_sum < rhs._sum);
}


CostADplus& CostADplus::operator+=(const CostADplus& rhs)
{
    CostAD::operator+=(rhs);
    _sum = _accid + _dist;
    return *this;
}


std::shared_ptr<Cost> CostADplus::shared_zero() const
{
    return std::shared_ptr<Cost>(new CostADplus());
}


std::shared_ptr<Cost> CostADplus::shared_clone() const
{
    return std::shared_ptr<Cost>(new CostADplus(*this));
}


//std::unique_ptr<Cost> CostADplus::unique_clone() const
//{
//    return std::unique_ptr<Cost>(new CostADplus(*this));
//}


void CostADplus::update(const enum NoteName& name, const enum Accid& accid,
                        bool print,
                        const Ton& gton, const Ton& lton)
{
    CostAD::update(name, accid, print, gton, lton);
    _sum = _accid + _dist;
}


CostType CostADplus::type() const
{
    return CostType::ADplus;
}


void CostADplus::print(std::ostream& o) const
{
    // CostAD::print(o);
    // o << _sum;
    o << _accid;
    o << "/";
    o << _dist;
}


std::ostream& operator<<(std::ostream& o, const CostADplus& c)
{
    c.print(o);
    return o;
}

} // end namespace pse


