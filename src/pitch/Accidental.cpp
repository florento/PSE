//
//  Accidental.cpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "Accidental.hpp"
#include <cmath>        // std::abs
#include <algorithm>    // std::max

namespace pse {


bool defined(const Accid& a)
{
    return (a != Accid::Undef);
}


int toint(const Accid& a)
{
    switch (a)
    {
        case Accid::TripleSharp:
            return 3;
            
        case Accid::DoubleSharp:
            return 2;
            
        case Accid::Sharp:
            return 1;
            
        case Accid::Natural:
            return 0;
            
        case Accid::Flat:
            return -1;

        case Accid::DoubleFlat:
            return -2;

        case Accid::TripleFlat:
            return -3;

        case Accid::ThreeQuartersSharp:
        case Accid::QuarterSharp:
        case Accid::QuarterFlat:
        case Accid::ThreeQuartersFlat:
        {
            ERROR("Accid to int {}: not defined for micro tonality");
            return 9;
        }
            
        case Accid::Undef:
        {
            ERROR("Accid to int of undef");
            return 9;
        }

        default:
        {
            ERROR("Accid to int: unexpected accidental value");
            return 9;
        }
    }
}


Accid accidofint(int a)
{
    switch (a)
    {
        case 3:
            return Accid::TripleSharp;
            
        case 2:
            return Accid::DoubleSharp;
            
        case 1:
            return Accid::Sharp;
            
        case 0:
            return Accid::Natural;
            
        case -1:
            return Accid::Flat;

        case -2:
            return Accid::DoubleFlat;

        case -3:
            return Accid::TripleFlat;

        default:
        {
            return Accid::Undef;
        }
    }
}


unsigned int accidDist(const Accid& lhs, const Accid& rhs)
{
    // one undef: just ignore
    if (lhs == Accid::Undef || rhs == Accid::Undef)
    {
        WARN("Accid::dist {} {}: one undef accid");
        return 0;
    }
    else
    {
        int l = toint(lhs);
        assert(-3 <= l);
        assert(l <= 3);
        int r = toint(rhs);
        assert(-3 <= r);
        assert(r <= 3);

        return std::abs(r - l);
        
//        // same sign
//        if ((l <= 0 && r <=0) || (l >= 0 && r >=0))
//        {
//            return std::abs(r - l);
//        }
//        // diff sign
//        else if (l < 0 && r > 0)
//        {
//            return std::max(-l, r);
//        }
//        // diff sign
//        else if (l > 0 && r < 0)
//        {
//            return std::max(l, -r);
//        }
//        // should not happen
//        else
//        {
//            ERROR("Accid::dist {} {}: missing case");
//            return 0;
//        }
    }
}


unsigned int accidDistSigned(const Accid& lhs, const Accid& rhs)
{
    // one undef: just ignore
    if (lhs == Accid::Undef || rhs == Accid::Undef)
    {
        WARN("Accid::dist {} {}: one undef accid");
        return 0;
    }
    else
    {
        int l = toint(lhs);
        assert(-3 <= l);
        assert(l <= 3);
        int r = toint(rhs);
        assert(-3 <= r);
        assert(r <= 3);

        // same sign
        if ((l <= 0 && r <=0) || (l >= 0 && r >=0))
        {
            return std::abs(r - l);
        }
        // diff sign
        else if (l < 0 && r > 0)
        {
            return std::max(-l, r);
        }
        // diff sign
        else if (l > 0 && r < 0)
        {
            return std::max(l, -r);
        }
        // should not happen
        else
        {
            ERROR("Accid::dist {} {}: missing case");
            return 0;
        }
    }
}



bool flat(const Accid& a)
{
    switch (a)
    {
        case Accid::TripleFlat:
        case Accid::DoubleFlat:
        case Accid::ThreeQuartersFlat:
        case Accid::Flat:
        case Accid::QuarterFlat:
            return true;
            
        default:
            return false;
    }
}


bool natural(const Accid& a)
{
    switch (a)
    {
        case Accid::Natural:
            return true;
            
        default:
            return false;
    }
}


bool sharp(const Accid& a)
{
    switch (a)
    {
        case Accid::QuarterSharp:
        case Accid::Sharp:
        case Accid::ThreeQuartersSharp:
        case Accid::DoubleSharp:
        case Accid::TripleSharp:
            return true;
            
        default:
            return false;
    }
}


std::string tostring(const Accid& a)
{
    switch (a)
    {
        case Accid::TripleFlat:
            return "bbb";
            break;

        case Accid::DoubleFlat:
            return "bb";
            break;
            
        case Accid::ThreeQuartersFlat:
            return "b+";
            break;

        case Accid::Flat:
            return "b";
            break;

        case Accid::QuarterFlat:
            return "b-";
            break;

        case Accid::Natural:
            return "0";
            break;

        case Accid::QuarterSharp:
            return "#-";
            break;

        case Accid::Sharp:
            return "#";
            break;

        case Accid::ThreeQuartersSharp:
            return "#+";
            break;

        case Accid::DoubleSharp:
            return "##";
            break;

        case Accid::TripleSharp:
            return "###";
            break;

        case Accid::Undef:
            return "NaN";
            break;

        // should not happen
        default:
            ERROR("Accid: unexpected code {}");
            return "ERROR";
            break;
    }
}


std::ostream& operator<<(std::ostream& o, const Accid& a)
{
    o << tostring(a);
    return o;
}


} // namespace pse
