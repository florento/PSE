//
//  Speller.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "Speller.hpp"


namespace pse {

std::string tostring(const Algo& a)
{
    switch (a)
    {
        case Algo::Undef:
            return "undef";

        case Algo::PSE:
            return "PSE";

        case Algo::PS13:
            return "PS13";

        case Algo::PS14:
            return "PS14";

        case Algo::RewritePassing:
            return "RewritePassing";

        default:
        {
            ERROR("unknown Algo Name");
            return "ERROR";
        }
    }
}


std::ostream& operator<<(std::ostream& o, const Algo& a)
{
    o << tostring(a);
    return o;
}


Speller::Speller(const Algo& algo, bool dflag):
_algo(algo),
_enum(0, 0),
_debug(dflag)
{
    spdlog_setVerbosity(4);
    debug(dflag);
    spdlog_setPattern();
}


Speller::Speller(const Speller& rhs):
_algo(rhs._algo),
_enum(rhs._enum),
_debug(rhs._debug)
{
    TRACE("Speller copy");
}


Speller::~Speller()
{
    TRACE("delete Speller of type {}", _algo);
}


//Speller::Speller(py::list& ln, const py::list& lb):
//_names(ln.size(), pse::NoteName::Undef),
//_accids(ln.size(), pse::Accid::Undef),
//_octave(ln.size(), 100),
//_enum(ln, lb)
//{
//    assert(ln.size() == lb.size());
//}


//Speller::Speller(const std::vector<int>& notes, const std::vector<int>& barnum):
//_names(notes.size(), pse::NoteName::Undef),
//_accids(notes.size(), pse::Accid::Undef),
//_octave(notes.size(), 100),
//_enum(notes, barnum)
//{
//    assert(notes.size() == barnum.size());
//}


//Algo Speller::algo() const
//{
//    return Algo::Undef;
//}


void Speller::debug(bool flag)
{
    TRACE("Speller: debug mode {}", flag);
    if (flag)
        spdlog_setVerbosity(5);
    else
        spdlog_setVerbosity(4);
}


size_t Speller::size() const
{
    //TRACE("Speller::size");
    return _enum.size();
}


void Speller::add(int note, int bar, bool simult)
{
    TRACE("Speller: add {} {}", note, bar);
    _enum.add(note, bar, simult);
}


enum NoteName Speller::name(size_t i) const
{
    return _enum.name(i);
}


enum Accid Speller::accidental(size_t i) const
{
    return _enum.accidental(i);
}


int Speller::octave(size_t i) const
{
    return _enum.octave(i);
}


bool Speller::printed(size_t i) const
{
    return _enum.printed(i);
}

bool Speller::rewritePassing()
{
    TRACE("Rewriting passing notes");
    return _enum.rewritePassing();
}



} // namespace pse
