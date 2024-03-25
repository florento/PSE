//
//  Accidental.cpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//


#include <cmath>        // std::abs
#include <algorithm>    // std::max

#include "Accidental.hpp"

namespace pse {


bool defined(const enum Accid& a)
{
    return (a != Accid::Undef);
}


int toint(const enum Accid& a)
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


enum Accid Accid(int a)
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


unsigned int accidDist(const enum Accid& lhs, const enum Accid& rhs)
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


unsigned int accidDistSigned(const enum Accid& lhs, const enum Accid& rhs)
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



bool flat(const enum Accid& a)
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


bool natural(const enum Accid& a)
{
    switch (a)
    {
        case Accid::Natural:
            return true;
            
        default:
            return false;
    }
}


bool sharp(const enum Accid& a)
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


enum Accid operator++(const enum Accid& a)
{
    switch (a)
    {
        case Accid::TripleFlat:
            return Accid::DoubleFlat;

        case Accid::DoubleFlat:
            return Accid::Flat;
            
        case Accid::ThreeQuartersFlat:
            return Accid::QuarterFlat;

        case Accid::Flat:
            return Accid::Natural;

        case Accid::QuarterFlat:
            return Accid::QuarterSharp;

        case Accid::Natural:
            return Accid::Sharp;

        case Accid::QuarterSharp:
            return Accid::ThreeQuartersSharp;

        case Accid::Sharp:
            return Accid::DoubleSharp;

        case Accid::ThreeQuartersSharp:
        {
            ERROR("sharpenization of 3 quarter sharp impossible");
            return Accid::ThreeQuartersSharp;
        }

        case Accid::DoubleSharp:
            return Accid::TripleSharp;

        case Accid::TripleSharp:
        {
            ERROR("sharpenization of triple sharp impossible");
            return Accid::TripleSharp;
        }

        case Accid::Undef:
        {
            WARN("sharpenization of undef accidental");
            return Accid::Undef;
        }

        // should not happen
        default:
        {
            ERROR("sharpenization of accidental: unexpected code {}", a);
            return Accid::Undef;
        }
    }
}


enum Accid operator--(const enum Accid& a)
{
    switch (a)
    {
        case Accid::TripleFlat:
        {
            ERROR("flattenization of triple flat impossible");
            return Accid::TripleFlat;
        }

        case Accid::DoubleFlat:
            return Accid::TripleFlat;
            
        case Accid::ThreeQuartersFlat:
        {
            ERROR("flattenization of 3 quarter flat impossible");
            return Accid::ThreeQuartersFlat;
        }

        case Accid::Flat:
            return Accid::DoubleFlat;

        case Accid::QuarterFlat:
            return Accid::ThreeQuartersFlat;

        case Accid::Natural:
            return Accid::Flat;

        case Accid::QuarterSharp:
            return Accid::QuarterFlat;

        case Accid::Sharp:
            return Accid::Natural;

        case Accid::ThreeQuartersSharp:
            return Accid::QuarterSharp;

        case Accid::DoubleSharp:
            return Accid::Sharp;

        case Accid::TripleSharp:
            return Accid::DoubleSharp;

        case Accid::Undef:
        {
            WARN("flattenization of undef accidental");
            return Accid::Undef;
        }

        // should not happen
        default:
        {
            ERROR("flattenization of accidental: unexpected code {}", a);
            return Accid::Undef;
        }
    }
}


std::string tostring(const enum Accid& a)
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
            return "n";
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
            ERROR("Accid: unexpected code {}", a);
            return "ERROR";
            break;
    }
}


std::ostream& operator<<(std::ostream& o, const enum Accid& a)
{
    o << tostring(a);
    return o;
}


} // namespace pse
