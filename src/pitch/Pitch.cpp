//
//  Pitch.cpp
//  qparse
//
//  Created by Florent Jacquemard on 10/05/17.
//  Copyright © 2017 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include "Pitch.hpp"

namespace pse {

const unsigned int  pse::Pitch::UNDEF_MIDICENT        = 12800;
const enum NoteName pse::Pitch::UNDEF_NOTE_NAME       = NoteName::Undef;
const int           pse::Pitch::UNDEF_NOTE_OCTAVE     = 128;
const enum Accid    pse::Pitch::UNDEF_NOTE_ALTERATION = Accid::Undef;

// note name for each index
const char pse::Pitch::NAME[7] =
    {'C', 'D', 'E', 'F', 'G', 'A', 'B'};

// names for the 12 semi-tones with flat accidents. private.
const char pse::Pitch::NAMEFLAT[12] =
{'C', 'D', 'D', 'E', 'E', 'F', 'G', 'G', 'A', 'A', 'B', 'B'};

// flat alterations to add to names indexes. private.
const int  pse::Pitch::FLAT[12] =
{ 0 ,  1 ,  0 ,  1 ,  0 ,  0 ,  1 ,  0 ,  1 ,  0 ,  1 ,  0 };

// names for the 12 semi-tones with sharp accidents. private.
const char pse::Pitch::NAMESHARP[12] =
{'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G', 'A', 'A', 'B'};

// sharp alterations to add to names indexes. private.
const int  pse::Pitch::SHARP[12] =
{ 0 ,  1 ,  0 ,  1 ,  0 ,  0 ,  1 ,  0 ,  1 ,  0 ,  1 ,  0 };


Pitch::Pitch():
PWO(),
//name(), // UNDEF_NOTE_NAME
//alteration(), // UNDEF_NOTE_ALTERATION
octave(UNDEF_NOTE_OCTAVE),
altprint(false),
_midi(UNDEF_MIDICENT)
{ };


Pitch::Pitch(const enum NoteName& name, const enum Accid& accid, int oct):
PWO(name, accid),
//name(name),
//alteration(accid),
octave(oct),
altprint(false)
{
    //assert('A' <= n);
    //assert(n <= 'G');
    assert(-10 <= oct);
    assert(oct <= 10);
    //assert(-2.0 <= alt);
    //assert(alt <= 2.0);
    assert(named());
    
    _midi = to_midi(name, accid, oct)*100;
    assert(_midi != UNDEF_MIDICENT);
}


Pitch::Pitch(unsigned int k, PitchUnit unit, bool named):
PWO(),
_midi((unit==PitchUnit::MIDI)?k*100:k),
altprint(false)
{
    assert((unit == PitchUnit::MIDI) || (unit == PitchUnit::MIDICENT));
    assert(0 <= k);
    assert(k < 12800);
    assert(_midi != UNDEF_MIDICENT);

    // compute default name (can be not relevant)
    if (named)
    {
        name = midi_to_name(_midi);
        alteration = midi_to_alt(_midi);
        assert(-2 <= toint(alteration));
        assert(toint(alteration) <= 2);
        octave = midicent_to_octave(_midi);
        assert((-1 <= octave) && (octave <= 9));
    }
    // do not name before pitchspelling : set to undef name/alt/octave
    else
    {
        name       = UNDEF_NOTE_NAME;
        alteration = UNDEF_NOTE_ALTERATION;
        octave     = UNDEF_NOTE_OCTAVE;
    }
}


Pitch::Pitch(const Pitch& rhs):
PWO(rhs),
//name(p.name),
//alteration(p.alteration),
octave(rhs.octave),
altprint(rhs.altprint),
_midi(rhs._midi)
{
    // @todo sanity check?
}


Pitch& pse::Pitch::operator=(const Pitch& rhs)
{
    if(this != &rhs)
    {
        PWO::operator=(rhs);
        //name = rhs.name;
        //alteration = rhs.alteration;
        octave = rhs.octave;
        altprint = rhs.altprint;
        _midi = rhs._midi;
    };
    return *this;
}


bool pse::Pitch::equal(const Pitch& rhs) const
{
    return (_midi == rhs._midi);
}


bool pse::Pitch::less(const Pitch& rhs) const
{
    assert(_midi != UNDEF_MIDICENT);
    assert(rhs._midi != UNDEF_MIDICENT);
    return (_midi < rhs._midi);
}


bool pse::Pitch::undef() const
{
    return (_midi == UNDEF_MIDICENT);
}


bool pse::Pitch::named() const
{
    return ((name != UNDEF_NOTE_NAME) &&
            (alteration != UNDEF_NOTE_ALTERATION) &&
            (octave != UNDEF_NOTE_OCTAVE));
}


void pse::Pitch::rename(const enum NoteName& n, const enum Accid& a, int o,
                   bool altpr)
{
    if (MidiNum::to_midi(n, a, o) == midi())
    {
        ERROR("pitch {} cannot be named by {}{} {}", midi(), n, a, o);
        return;
    }
    if (name != UNDEF_NOTE_NAME)
    {
        TRACE("Pitch rename: aleady named, replace name by {}.", name);
        TRACE("former pitch name : (MIDI {}%{}) {}", midi(), midi()%12, *this);
    }
    //assert('A' <= n);
    //assert(n <= 'G');
    assert(defined(n));
    name = n;
    //assert(-2.0 <= alt);
    //assert(alt <= 2.0);
    assert(defined(a));
    alteration = a;
    assert(-10 <= o);
    assert(o <= 10);
    octave = o;
    altprint = altpr;
    TRACE("Pitch rename: (MIDI {}%{}): {}", midi(), midi()%12,  *this);
    // assert(name_to_midi(name, alteration, octave) == midi());
}


void pse::Pitch::rename(const enum NoteName& n)
{
    int m = midi();
    enum Accid a = MidiNum::accid(m%12, n);
    if (a == Accid::Undef)
    {
        ERROR("pitch {} cannot be named by {}", m, n);
        return;
    }
    int o = MidiNum::midi_to_octave(m, n, a);
    rename(n, a, o, true);
}

    
// static
// revise to MIDIcent
enum NoteName pse::Pitch::midi_to_name(unsigned int k)
{
    unsigned int p = int(floor(k / 100));
    assert ((0 <= p) && (p <= 127));
    char n = NAMESHARP[p % 12];
    return NoteName(n);
}


// static
enum Accid pse::Pitch::midi_to_alt(unsigned int k)
{
    unsigned int p = int(floor(k / 100));
    assert(0 <= p);
    assert(p <= 127);
    int a = SHARP[p%12] +  (k - (p*100))/100;
    return Accid(a);
}


// C4 is MIDI key 60
int pse::Pitch::midicent_to_octave(unsigned int k)
{
    int oct = int(floor(k/1200)) - 1;
    assert(-1 <= oct);
    assert(oct <= 9);
    return oct;
}


// @todo TBR (incorrect)
//int pse::Pitch::midi_to_octave(unsigned int k)
//{
//    int oct = int(floor(k/12)) - 1;
//    assert(-1 <= oct);
//    assert(oct <= 9);
//    return oct;
//}


// static
//int pse::Pitch::midi_to_octave(unsigned int m,
//                          const enum NoteName& n, const enum Accid& a)
//{
//    return midi_to_octave(m);
//}


unsigned int pse::Pitch::pitchClass(const enum NoteName& name)
{
    char c = tochar(name);  // toupper(n);
    for (int i = 0; i < 12; ++i)
    {
        if (NAMESHARP[i] == c)
            return i;
    }
    throw std::runtime_error("wrong note name\n");
}


// TBC
unsigned int pse::Pitch::to_midi(const enum NoteName& name, const enum Accid& accid,
                            int oct)
{
    int alt = toint(accid);
    assert(-2 <= alt);
    assert(alt <= 2);
    assert(-1 <= oct);
    assert(oct <= 9);
    
    unsigned int i = pitchClass(name);
    int falt = int(floor(alt)); // useless ?
    int r = (i + falt)%12;
    assert(0 <= r);
    assert(r <= 11);
    unsigned int p = ((oct+1) * 12) + r;
    assert(0 <= p);
    assert(p <= 127);
    
    return ((p*100)+ int((alt-falt)*100));
}


// static
// TBR (use NoteName)
int pse::Pitch::name_to_int(char n)
{
    assert('A' <= n);
    assert(n <= 'G');
    for (int i = 0; i < 7; ++i)
    {
        if (NAME[i] == n)
            return i;
    }
    ERROR("name_to_int: invalid note name {}", n);
    return 99;
}


// static
// TBR (use NoteName)
char pse::Pitch::name_of_int(int c)
{
    assert(0 <= c);
    assert(c <= 6);
    return NAME[c];
}


// static
// TBR (use Accid)
//std::string pse::Pitch::alt_to_string(float alt)
//{
//    if ( alt == pse::Pitch::UNDEF_NOTE_ALTERATION)
//        return "?";
//    else if (alt == 0.0)
//        return "";
//    else if (alt == 1.0)
//        return "#";
//    else if (alt == 2.0)
//        return "##";
//    else if (alt == -1.0)
//        return "b";
//    else if (alt == -2.0)
//        return "bb";
//    else
//        return std::to_string(alt);
//}


void pse::Pitch::print(std::ostream& o) const
{
    if (undef()) o << "Unpitched";
    else if (named())
    {
        o << name;
        o << alteration;
        if ( octave == pse::Pitch::UNDEF_NOTE_OCTAVE)
            o << "?";
        else
            o << octave;
        if (altprint)
            o << "(p)";
    }
    else
        o << "(" << midi() << ")";  // MIDI key
}


std::ostream& operator<<(std::ostream& o, const Pitch& p)
{
    p.print(o);
    return o;
}


} // end namespace pse

/// @}
