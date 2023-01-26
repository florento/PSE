//
//  KeyFind.cpp
//  qparse
//
//  Created by Florent Jacquemard on 26/03/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "KeyFind.hpp"


namespace scoremodel {


KeyFind::KeyFind(Part& part):
_part(part),
_durr({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
_dury({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
_meany(0),
_sig(SIG_UNDEF),
_major(true)
{
    init_dur();
    init_sig();
}


int KeyFind::signature() const
{
    return _sig;
}


bool KeyFind::major()
{
    return _major;
}


KeyFind::KeyFind(const KeyFind& kf):
_part(kf._part),
_durr(kf._durr),
_dury(kf._dury),
_meany(kf._meany),
_sig(kf._sig),
_major(kf._major)
{ }


KeyFind::~KeyFind()
{ }


void KeyFind::init_dur()
{
    for (Part::iterator i = _part.begin(); i != _part.end(); ++i)
    {
        Note& n = *i;
        const Pitch& p = n.pitch();
        int k = p.midi() % 12; // pitch class in 0..11
        _durr[k] += n.duration(); // in MTU
    }
    
    // test
    //_durr = { 432, 231, 0, 405, 12, 316, 4, 126, 612, 0, 191, 1 };
    
    double sumy = 0;
    for (size_t j = 0; j < 12; ++j)
    {
        _dury[j] = toDouble(_durr[j]);
        TRACE("Keyfind: total duration for {} = {} = {}", j, _durr[j], _dury[j]);
        sumy += _dury[j];
    }

    _meany = sumy/12;
}


double KeyFind::correlation_coefficient(int key, bool major)
{
    assert(0 <= key);
    assert(key < 12);
    std::array<double, 12> profilex;
    double meanx;
        
    if (major)
    {
        profilex = KS_MAJOR_PROFILE;
        meanx = KS_MAJOR_MEAN;
    }
    else
    {
        profilex = KS_MINOR_PROFILE;
        meanx = KS_MINOR_MEAN;
    }
    
    double num = 0;
    double denx = 0;
    double deny = 0;

    for (size_t i = 0; i < 12; ++i)
    {
        size_t j = (i+key) % 12;
        double xxi = (profilex[i] - meanx);
        double yyi = (_dury[j] - _meany);

        num += xxi * yyi;
        denx += xxi * xxi;
        deny += yyi * yyi;
    }
    assert(denx != 0);
    assert(deny != 0);

    return (num / sqrt(denx * deny));
}


void KeyFind::init_sig()
{
    double max = -2;
    bool cont = true;
    bool major = true;
    int k = 0;
    TRACE("Keyfind: Key estimation");
    do
    {
        for (int i = 0; i < 12; ++i)
        {
            double c = correlation_coefficient(i, major);
            
            TRACE("Keyfind: {} ({}): CC={}", i, (major?"Major":"minor"), c);
            if (c > max)
            {
                max = c;
                k = i;
                _major = major;
            }
        }
        
        if (major)
            major = false;
        else
            cont = false;
    }
    while (cont);
    
    _sig = KeySig::key(k, _major);
    TRACE("Keyfind: estimation = {} {}, sig={}",
         k, (_major?"Major":"minor"), _sig);
}


int KeyFind::SIG_UNDEF = 9;

const std::array<double, 12> KeyFind::KS_MAJOR_PROFILE =
{  6.35, 2.23, 3.48, 2.33, 4.38, 4.09, 2.52, 5.19, 2.39, 3.66, 2.29, 2.88 };
// do    do#   re    re#   mi    fa    fa#   sol   sol#  la    la#   si

// KS_MAJOR_SUM = 41.79
const double KeyFind::KS_MAJOR_MEAN = 3.4825;

const std::array<double, 12> KeyFind::KS_MINOR_PROFILE =
{  6.33, 2.68, 3.52, 5.38, 2.60, 3.53, 2.54, 4.75, 3.98, 2.69, 3.34, 3.17 };
// la    la#   si    do    do#   re    re#   mi    fa    fa#   sol   sol#

// KS_MINOR_SUM = 44.51
const double KeyFind::KS_MINOR_MEAN = 3.2092;


} // namespace scoremodel


