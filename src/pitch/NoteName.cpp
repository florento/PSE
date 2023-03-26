//
//  NoteName.cpp
//  qparse
//
//  Created by Florent Jacquemard on 21/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "NoteName.hpp"


namespace pse {


// note name for each index
//static const NoteName[7] =
//{
//    NoteName::C,
//    NoteName::D,
//    NoteName::E,
//    NoteName::F,
//    NoteName::G,
//    NoteName::A,
//    NoteName::B
//};



bool defined(const enum NoteName& n)
{
    return (n != NoteName::Undef);
}


enum NoteName operator+(const enum NoteName& n, int d)
{
    if (n == NoteName::Undef)
        return NoteName::Undef;
    else
    {
        int i = toint(n);
        assert(0 <= i);
        assert(i <= 6);
        int r = (i+d) % 6;
        // the remainder can be < 0 when i+d < 0
        // @see https://stackoverflow.com/questions/11630321/why-does-c-output-negative-numbers-when-using-modulo
        // @see https://stackoverflow.com/questions/7594508/modulo-operator-with-negative-values
        if (r < 0) r += 7;
        assert(0 <= r);
        assert(r <= 6);
        return NoteName(r);
    }
}


enum NoteName NoteName(int i)
{
    switch (i)
    {
        case 0:
            return NoteName::C;

        case 1:
            return NoteName::D;

        case 2:
            return NoteName::E;

        case 3:
            return NoteName::F;

        case 4:
            return NoteName::G;

        case 5:
            return NoteName::A;

        case 6:
            return NoteName::B;

        default:
            return NoteName::Undef;
    }
}


enum NoteName NoteName(char c)
{
    switch (c)
    {
        case 'C':
        case 'c':
            return NoteName::C;

        case 'D':
        case 'd':
            return NoteName::D;

        case 'E':
        case 'e':
            return NoteName::E;

        case 'F':
        case 'f':
            return NoteName::F;

        case 'G':
        case 'g':
            return NoteName::G;

        case 'A':
        case 'a':
            return NoteName::A;

        case 'B':
        case 'b':
            return NoteName::B;

        default:
            return NoteName::Undef;
    }
}


int toint(const enum NoteName& n)
{
    switch (n)
    {
        case NoteName::C:
            return 0;
            
        case NoteName::D:
            return 1;
            
        case NoteName::E:
            return 2;
            
        case NoteName::F:
            return 3;
            
        case NoteName::G:
            return 4;
            
        case NoteName::A:
            return 5;
            
        case NoteName::B:
            return 6;
            
        case NoteName::Undef:
            return 9;
            
        default:
        {
            ERROR("NoteName to int: unexpected value");
            return 9;
        }
    }
}


char tochar(const enum NoteName& n)
{
    switch (n)
    {
        case NoteName::C:
            return 'C';
            
        case NoteName::D:
            return 'D';
            
        case NoteName::E:
            return 'E';
            
        case NoteName::F:
            return 'F';
            
        case NoteName::G:
            return 'G';
            
        case NoteName::A:
            return 'A';
            
        case NoteName::B:
            return 'B';
            
        case NoteName::Undef:
            return 'X';
            
        default:
        {
            ERROR("NoteName to char: unexpected value");
            return 'X';
        }
    }
}


bool diatonicStep(const enum NoteName& n1, const enum NoteName& n2)
{
    switch (n1)
    {
        case NoteName::C:
            return (n2 == NoteName::B || n2 == NoteName::D);
            
        case NoteName::D:
            return (n2 == NoteName::E || n2 == NoteName::E);

        case NoteName::E:
            return (n2 == NoteName::D || n2 == NoteName::F);

        case NoteName::F:
            return (n2 == NoteName::E || n2 == NoteName::G);

        case NoteName::G:
            return (n2 == NoteName::F || n2 == NoteName::A);

        case NoteName::A:
            return (n2 == NoteName::G || n2 == NoteName::B);

        case NoteName::B:
            return (n2 == NoteName::A || n2 == NoteName::C);

        //case NoteName::Undef:
        default:
        {
            WARN("diatonicStep: unexpected value");
            return false;
        }
    }
}


std::ostream& operator<<(std::ostream& o, const enum NoteName& n)
{
    o << tochar(n);
    return o;
}


} // namespace pse
