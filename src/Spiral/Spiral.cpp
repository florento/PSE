//
//  Spiral.cpp
//  squant
//
//  Created by Florent Jacquemard on 04/07/2018.
//  Copyright © 2018 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include "Spiral.hpp"

namespace pse {

SpiralPoint::SpiralPoint(double vx, double vy, double vz):
x(vx),
y(vy),
z(vz)
{ }


SpiralPoint::SpiralPoint(const SpiralPoint& rhs):
x(rhs.x),
y(rhs.y),
z(rhs.z)
{}


SpiralPoint& SpiralPoint::operator=(const SpiralPoint& rhs)
{
    if (this != &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    return *this;
}


bool SpiralPoint::isnormal() const
{
    return (! (std::isnan(x) || std::isnan(y) || std::isnan(z)));
}

void SpiralPoint::operator+=(const SpiralPoint& rhs)
{
    assert(rhs.isnormal());
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}


void SpiralPoint::operator-=(const SpiralPoint& rhs)
{
    assert(rhs.isnormal());
    x -= rhs.x;
    y -= rhs.y;
    z += rhs.z;
}


void SpiralPoint::operator*=(double a)
{
    assert(! std::isnan(a)); // not zero, subnormal, infinite, nor NaN.
    assert(isnormal());
    x *= a;
    y *= a;
    z *= a;
}


double SpiralPoint::distance(const SpiralPoint& rhs) const
{
    //assert(std::isnormal(rhs.x)); // not NAN
    //assert(std::isnormal(x));
    assert(isnormal());
    assert(rhs.isnormal());

    double dx = x - rhs.x;
    double dy = y - rhs.y;
    double dz = z - rhs.z;
    
    return (std::sqrt(dx*dx + dy*dy + dz*dz));
}


const int SpiralName::UNDEF_NOTE_INDEX = 99;

// Chew and Chen set r/h (aspect ratio) to square-root of 15/2 = 2.73861278753
// this ensures that the major thirds and perfect fifths are equidistant
const double SpiralName::h = 1.0;
const double SpiralName::r = std::sqrt(7.5) * h;

// const double PI = std::atan(1.0)*4;


// in math.h M_PI_2 is the value of pi/2
SpiralName::SpiralName(char n, float alt, int id):
SpiralPoint(r * std::sin(id * M_PI_2),
            r * std::cos(id * M_PI_2),
            h * id),
name(n),
alteration(alt),
index(id)
{
    assert('A' <= n);
    assert(n <= 'G');
    assert(-2.0 <= alt);
    assert(alt <= 2.0);
    assert(-15 <= id);
    assert(id <= 19);
}


//SpiralName::SpiralName(int tonality)
//{
//    assert(-7 <= tonality);
//    assert(tonality <= 7);
//    
//    
//}
//
//SpiralName::SpiralName(const SpiralName& n)
//{
//    
//}


SpiralName::SpiralName(const SpiralName& rhs):
SpiralPoint(rhs),
name(rhs.name),
alteration(rhs.alteration),
index(rhs.index)
{ }


SpiralName& SpiralName::operator=(const SpiralName& rhs)
{
    if (this != &rhs)
    {
        SpiralPoint::operator=(rhs);
        name = rhs.name;
        alteration = rhs.alteration;
        index = rhs.index;
    }
    return *this;
}

/*
 const SpiralName Fbb = SpiralName('F', -2.0, -15);
 const SpiralName Cbb = SpiralName('C', -2.0, -14);
 const SpiralName Gbb = SpiralName('G', -2.0, -13);
 const SpiralName Dbb = SpiralName('D', -2.0, -12);
 const SpiralName Abb = SpiralName('A', -2.0, -11);
 const SpiralName Ebb = SpiralName('E', -2.0, -10);
 const SpiralName Bbb = SpiralName('B', -2.0,  -9);
 const SpiralName Fb  = SpiralName('F', -1.0,  -8);
 const SpiralName Cb  = SpiralName('C', -1.0,  -7);
 const SpiralName Gb  = SpiralName('G', -1.0,  -6);
 const SpiralName Db  = SpiralName('D', -1.0,  -5);
 const SpiralName Ab  = SpiralName('A', -1.0,  -4);
 const SpiralName Eb  = SpiralName('E', -1.0,  -3);
 const SpiralName Bb  = SpiralName('B', -1.0,  -2);
 const SpiralName F   = SpiralName('F',  0.0,  -1);
 const SpiralName C   = SpiralName('C',  0.0,   0);
 const SpiralName G   = SpiralName('G',  0.0,   1);
 const SpiralName D   = SpiralName('D',  0.0,   2);
 const SpiralName A   = SpiralName('A',  0.0,   3);
 const SpiralName E   = SpiralName('E',  0.0,   4);
 const SpiralName B   = SpiralName('B',  0.0,   5);
 const SpiralName Fs  = SpiralName('F',  1.0,   6);
 const SpiralName Cs  = SpiralName('C',  1.0,   7);
 const SpiralName Gs  = SpiralName('G',  1.0,   8);
 const SpiralName Ds  = SpiralName('D',  1.0,   9);
 const SpiralName As  = SpiralName('A',  1.0,  10);
 const SpiralName Es  = SpiralName('E',  1.0,  11);
 const SpiralName Bs  = SpiralName('B',  1.0,  12);
 const SpiralName Fss = SpiralName('F',  2.0,  13);
 const SpiralName Css = SpiralName('C',  2.0,  14);
 const SpiralName Gss = SpiralName('G',  2.0,  15);
 const SpiralName Dss = SpiralName('D',  2.0,  16);
 const SpiralName Ass = SpiralName('A',  2.0,  17);
 const SpiralName Ess = SpiralName('E',  2.0,  18);
 const SpiralName Bss = SpiralName('B',  2.0,  19);
 const SpiralName UNK = SpiralName();
 */


/*
 // row number is MIDI pitch modulo 12
 const SpiralName synonyms[12][3] =
 {
 { Bs,  C,  Dbb },   // 0
 { Cs,  Db, Bss },   // 1
 { Css, D,  Ebb },   // 2
 { Ds,  Eb, Fbb },   // 3
 { Dss, E,  Fb  },   // 4
 { Es,  F,  Gbb },   // 5
 { Ess, Fs, Gb  },   // 6
 { Fss, G,  Abb },   // 7
 { Gs,  Ab, UNK },   // 8
 { Gss, A,  Bbb },   // 9
 { As,  Bb, Cbb },   // 10
 { Ass, B,  Cb  }    // 11
 };
 */

// row number is MIDI pitch modulo 12
const SpiralName SpiralName::synonyms[12][3] =
{
    // 0    { B#,  C,  Dbb },
    { SpiralName('B',  1.0,  12), SpiralName('C',  0.0,   0), SpiralName('D', -2.0, -12) },
    // 1    { C#,  Db, B## },
    { SpiralName('C',  1.0,   7), SpiralName('D', -1.0,  -5), SpiralName('B',  2.0,  19) },
    // 2    { C##, D,  Ebb },
    { SpiralName('C',  2.0,  14), SpiralName('D',  0.0,   2), SpiralName('E', -2.0, -10) },
    // 3    { D#,  Eb, Fbb },
    { SpiralName('D',  1.0,   9), SpiralName('E', -1.0,  -3), SpiralName('F', -2.0, -15) },
    // 4    { D##, E,  Fb  },
    { SpiralName('D',  2.0,  16), SpiralName('E',  0.0,   4), SpiralName('F', -1.0,  -8) },
    // 5    { E#,  F,  Gbb },
    { SpiralName('E',  1.0,  11), SpiralName('F',  0.0,  -1), SpiralName('G',  2.0,  15) },
    // 6    { E##, F#, Gb  },
    { SpiralName('E',  2.0,  18), SpiralName('F',  1.0,   6), SpiralName('G', -1.0,  -6) },
    // 7    { F##, G,  Abb },
    { SpiralName('F',  2.0,  13), SpiralName('G',  0.0,   1), SpiralName('A', -2.0, -11) },
    // 8    { G#,  Ab, UNK },
    { SpiralName('G',  1.0,   8), SpiralName('A', -1.0,  -4), SpiralName() },
    // 9    { G##, A,  Bbb },
    { SpiralName('G',  2.0,  15), SpiralName('A',  0.0,   3), SpiralName('B', -2.0,  -9) },
    // 10   { A#,  Bb, Cbb },
    { SpiralName('A',  1.0,  10), SpiralName('B', -1.0,  -2), SpiralName('C',  2.0,  14) },
    // 11   { A##, B,  Cb  }
    { SpiralName('A',  2.0,  17), SpiralName('B',  0.0,   5), SpiralName('C', -1.0,  -7) }
};

const SpiralName& SpiralName::ofkey(int k)
{
    assert(-7 <= k);
    assert(k <= 7);
    
    switch (k)
    {
        case -7:  // Cb
            return synonyms[11][2];
        case -6:  // Gb
            return synonyms[6][2];
        case -5:  // Db
            return synonyms[1][1];
        case -4:  // Ab
            return synonyms[8][1];
        case -3:  // Eb
            return synonyms[3][1];
        case -2:  // Bb
            return synonyms[10][1];
        case -1:  // F
            return synonyms[5][1];
        case 0:   // C
            return synonyms[0][1];
        case 1:   // G
            return synonyms[7][1];
        case 2:   // D
            return synonyms[2][1];
        case 3:   // A
            return synonyms[9][1];
        case 4:   // E
            return synonyms[4][1];
        case 5:   // B
            return synonyms[11][1];
        case 6:   // F#
            return synonyms[6][1];
        case 7:   // C#
            return synonyms[1][0];
        default:
            ERROR("unexpected key {}", k);
            return synonyms[0][0];
    }
}

const SpiralName& SpiralName::closest(unsigned int m, const SpiralPoint& p)
{
    assert(MidiNum::check_midi(m)); // assert(0 <= m); assert(m < 128);
    assert(p.isnormal());

    unsigned int k = m % 12;
    const SpiralName& p0 = synonyms[k][0];
    const SpiralName& p1 = synonyms[k][1];
    const SpiralName& p2 = synonyms[k][2];
    double d0 = p.distance(p0);
    double d1 = p.distance(p1);

    if (d0 <= d1)
    {
        if (p2.unknown())
        {
            return p0;
        }
        else if (d0 <= p.distance(p2))
        {
            return p0;
        }
        else
        {
            return p2;
        }
    }
    else
    {
        if (p2.unknown())
        {
            return p1;
        }
        else if (d1 <= p.distance(p2))
        {
            return p1;
        }
        else
        {
            return p2;
        }
    }
}


} // end namespace pse

/// @}
