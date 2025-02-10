//
//  SpellerEnum.cpp
//  pse
//
//  Created by Florent Jacquemard on 23/01/2025.
//

#include "SpellerEnum.hpp"

namespace pse {


SpellerEnum::SpellerEnum(size_t nbton, const Algo& algo, bool dflag):
Speller(new PSRawEnum(0, 0), nbton, algo, dflag)
{ }


SpellerEnum::~SpellerEnum()
{
    assert(_enum);
    delete _enum;
}


//
// notes input and output
//


size_t SpellerEnum::size() const
{
    //TRACE("Speller::size");
    return rawenum().size();
}


void SpellerEnum::resetEnum(size_t i0, size_t i1)
{
    rawenum().reset(i0, i1);
}


void SpellerEnum::add(int midi, int bar, bool simult, const PSRatio& dur)
{
    TRACE("Speller: add {} {} {}", midi, bar, dur);
    rawenum().add(midi, bar, simult, dur);
}

void SpellerEnum::add(int midi, int bar,
                      const enum NoteName& name, const enum Accid& accid,
                      int octave, bool printed,
                      bool simult, const PSRatio& dur)
{
    TRACE("Speller: add {} {} {}", midi, bar, dur);
    rawenum().add(midi, bar, name, accid, octave, printed, simult, dur);
}

// for pybind
void SpellerEnum::add0(int midi, int bar, bool simult)
{
    TRACE("Speller: add {} {}", midi, bar);
    rawenum().add(midi, bar, simult);
}


// for pybind
void SpellerEnum::add2(int midi, int bar, bool simult,
                       long dur_num, long dur_den)
{
    TRACE("Speller: add {} {} {}", midi, bar, PSRatio(dur_num, dur_den));
    rawenum().add(midi, bar, simult, PSRatio(dur_num, dur_den));
}

// for pybind
void SpellerEnum::add4(int midi, int bar, bool simult,
                       const enum NoteName& name, const enum Accid& accid,
                       int octave, bool printed)
{
    TRACE("Speller: add {} {}", midi, bar);
    rawenum().add(midi, bar, name, accid, octave, printed, simult);
}


// for pybind
void SpellerEnum::add6(int midi, int bar, bool simult,
                       long dur_num, long dur_den,
                       const enum NoteName& name, const enum Accid& accid,
                       int octave, bool printed)
{
    TRACE("Speller: add {} {} {}", midi, bar, PSRatio(dur_num, dur_den));
    rawenum().add(midi, bar, name, accid, octave, printed, simult,
                  PSRatio(dur_num, dur_den));
}


PSRawEnum& SpellerEnum::rawenum() const
{
    assert(_enum);
    PSRawEnum* penum = dynamic_cast<PSRawEnum*>(_enum);
    assert(penum);
    return *penum;
}


} // namespace pse
