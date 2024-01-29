//
//  Speller.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "Speller.hpp"


namespace pse {


Speller::Speller(const Algo& algo, size_t nbt, bool dflag):
_algo(algo),
_enum(0, 0),
_index(nbt),
_debug(dflag)
{
    spdlog_setVerbosity(4);
    debug(dflag);
    spdlog_setPattern();
}


Speller::Speller(const Speller& rhs):
_algo(rhs._algo),
_enum(rhs._enum),  /// @todo AV copy
_index(rhs._index), /// @todo AV copy
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


void Speller::add(int note, int bar, bool simult, const Rational& dur)
{
    TRACE("Speller: add {} {} {}", note, bar, dur);
    _enum.add(note, bar, simult, dur);
}


void Speller::add_pybindwd(int note, int bar, bool simult,
                             long dur_num, long dur_den)
{
    TRACE("Speller: add {} {} {}", note, bar, Rational(dur_num, dur_den));
    _enum.add(note, bar, simult, Rational(dur_num, dur_den));
}


void Speller::add_pybindwod(int note, int bar, bool simult)
{
    TRACE("Speller: add {} {}", note, bar);
    _enum.add(note, bar, simult);
}


size_t Speller::nbTons() const
{
    return _index.size();  // nbTons();
}


const Ton& Speller::ton(size_t i) const
{
    return _index.ton(i);
}


void Speller::resetTons()
{
    _index.reset();
}


void Speller::addTon(const Ton& ton, bool global)
{
    TRACE("Speller: add tonality {}", ton);
    _index.add(ton);
}


void Speller::addTon(int ks, ModeName mode, bool global)
{
    if (ks < -7 || 7 < ks)
    {
        ERROR("Speller addTon: wrong key signature value {}", ks);
    }
    TRACE("Speller: add tonality {} {}", ks, mode);
    _index.add(ks, mode, global);
}


void Speller::closeTons()
{
    _index.close();
}


bool Speller::closedTons() const
{
    return _index.closed();
}


size_t Speller::rewritePassing()
{
    TRACE("Rewriting passing notes");
    return _enum.rewritePassing();
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


} // namespace pse
