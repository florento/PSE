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


void SpellerEnum::add(int note, int bar, bool simult, const PSRatio& dur)
{
    TRACE("Speller: add {} {} {}", note, bar, dur);
    rawenum().add(note, bar, simult, dur);
}


void SpellerEnum::add2(int note, int bar, bool simult,
                             long dur_num, long dur_den)
{
    TRACE("Speller: add {} {} {}", note, bar, PSRatio(dur_num, dur_den));
    rawenum().add(note, bar, simult, PSRatio(dur_num, dur_den));
}


void SpellerEnum::add0(int note, int bar, bool simult)
{
    TRACE("Speller: add {} {}", note, bar);
    rawenum().add(note, bar, simult);
}


PSRawEnum& SpellerEnum::rawenum() const
{
    assert(_enum);
    PSRawEnum* penum = dynamic_cast<PSRawEnum*>(_enum);
    assert(penum);
    return *penum;
}


} // namespace pse
