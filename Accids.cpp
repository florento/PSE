//
//  Accids.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/03/2024.
//

#include "Accids.hpp"


namespace pse {


// static
accids_t Accids::encode()
{
    return 0;
}


// static
accids_t Accids::encode(const enum Accid a1)
{
    assert(a1 != Accid::Undef);
    return (toint(a1) << 4);
}


// static
accids_t Accids::encode(const enum Accid a1, const enum Accid a2)
{
    assert(a1 != Accid::Undef);
    assert(a2 != Accid::Undef);
    return (encode(a1) + toint(a2));
}


// static
enum Accid Accids::first(accids_t p)
{
    return (ofint(fst(p)));
}


// static
enum Accid Accids::second(accids_t p)
{
    return (ofint(snd(p)));
}


// static
bool Accids::zero(accids_t p)
{
    return (p == 0);
}


// static
bool Accids::single(accids_t p)
{
    return !zero(p) && (snd(p) == 0); // second is undef
}


// static
bool Accids::two(accids_t p)
{
    return (snd(p) != 0); // second is undef
}


// static
bool Accids::contained(enum Accid a, accids_t p)
{
    accids_t c = toint(a);
    return (fst(p) == c || snd(p) == c);
}


// static
accids_t Accids::fst(accids_t p)
{
    return (p >> 4);
}


// static
accids_t Accids::snd(accids_t p)
{
    return (p & 0b00001111);
}


// static
accids_t Accids::toint(enum Accid a)
{
    switch (a)
    {
        case Accid::Undef:
            return 0;

        case Accid::TripleFlat:
            return 1;

        case Accid::DoubleFlat:
            return 2;
            
        case Accid::ThreeQuartersFlat:
            return 3;

        case Accid::Flat:
            return 4;
            
        case Accid::QuarterFlat:
            return 5;

        case Accid::Natural:
            return 6;

        case Accid::QuarterSharp:
            return 7;

        case Accid::Sharp:
            return 8;

        case Accid::ThreeQuartersSharp:
            return 9;

        case Accid::DoubleSharp:
            return 10;

        case Accid::TripleSharp:
            return 11;

        // should not happen
        default:
            ERROR("Accids: unexpected code {}", a);
            return 0;
    }
}


// static
enum Accid Accids::ofint(accids_t a)
{
    assert(0 <= a && a < 12);

    switch (a)
    {
        case 0:
            return Accid::Undef;

        case 1:
            return Accid::TripleFlat;
            
        case 2:
            return Accid::DoubleFlat;

        case 3:
            return Accid::ThreeQuartersFlat;

        case 4:
            return Accid::Flat;

        case 5:
            return Accid::QuarterFlat;
            
        case 6:
            return Accid::Natural;

        case 7:
            return Accid::QuarterSharp;

        case 8:
            return Accid::Sharp;

        case 9:
            return Accid::ThreeQuartersSharp;

        case 10:
            return Accid::DoubleSharp;

        case 11:
            return Accid::TripleSharp;

        default:         // should not happen
        {
            ERROR("Accids ofint: unexpected value {}", a);
            return Accid::Undef;
        }
    }
}
 

// static
std::string Accids::string(accids_t p)
{
    if (zero(p))
        return "UNDEF";
    else if (single(p))
        return tostring(first(p));
    else
    {
        assert(two(p));
        return (tostring(first(p))+" or "+tostring(second(p)));
    }
}


} // namespace pse
