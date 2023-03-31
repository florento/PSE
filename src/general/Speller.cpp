//
//  Speller.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "Speller.hpp"


namespace pse {


Speller::Speller(bool dflag):
_enum(0, 0),
_debug(dflag)
{
    spdlog_setVerbosity(4);
    debug(dflag);
    spdlog_setPattern();
}


Speller::~Speller()
{
    TRACE("delete Speller");
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




} // namespace pse
