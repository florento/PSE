//
//  SpellerEnum.cpp
//  pse
//
//  Created by Florent Jacquemard on 23/01/2025.
//

#include "SpellerEnum.hpp"

namespace pse {


SpellerEnum::SpellerEnum(size_t tons, bool aux_enum,
                         const Algo& algo, bool dflag):
Speller(new PSRawEnum(0, 0), (aux_enum?new PSRawEnum(0, 0):nullptr),
        tons, algo, dflag)
{ }


SpellerEnum::~SpellerEnum()
{
    assert(_enum);
    delete _enum;
    _enum = nullptr;
    if (_enum_aux)
    {
        delete _enum_aux;
        _enum_aux = nullptr;
    }        
}


//
// notes input and output
//


size_t SpellerEnum::size(bool aux) const
{
    //TRACE("Speller::size");
    if (aux and not hasAuxEnumerator())
    {
        ERROR("Speller size: no auxilliary enumerator");
        return 0;
    }
    else
    {
        return rawenum(aux).size();
    }
}


void SpellerEnum::resetEnum(size_t i0, size_t i1)
{
    rawenum(false).reset(i0, i1);
    if (hasAuxEnumerator())
    {
        rawenum(true).reset(i0, i1);
    }
}


bool SpellerEnum::addAuxEnumerator()
{
    if (hasAuxEnumerator())
    {
        ERROR("Speller: there is already an auxiliary enumerator. cannot add.");
        return false;
    }
    else
        return Speller::setAuxEnumerator(new PSRawEnum(0, 0));
}


//void SpellerEnum::add(int midi, int bar, bool simult, const PSRatio& dur)
//{
//    TRACE("Speller: add {} {} {}", midi, bar, dur);
//    rawenum().add(midi, bar, simult, dur);
//}


void SpellerEnum::add(int midi, int bar, bool simultaneous,
                      bool aux, const PSRatio& dur,
                      const enum NoteName& name, const enum Accid& accid,
                      int octave, bool printed)
{
    TRACE("Speller: add {} {} {}", midi, bar, dur);
    // name, accid, oct are all defined or all undef
    assert((accid == Accid::Undef) == (name == NoteName::Undef));
    assert((octave == Pitch::UNDEF_OCTAVE) == (name == NoteName::Undef));
    assert(Pitch::check_octave(octave));
    rawenum(aux).add(midi, bar, simultaneous, dur,
                     name, accid, octave, printed);
}


// for pybind
void SpellerEnum::add0(int midi, int bar, bool simultaneous, bool aux)
{
    TRACE("Speller: add {} {}", midi, bar);
    add(midi, bar, simultaneous, aux);
}


// for pybind
void SpellerEnum::add2(int midi, int bar, bool simultaneous,
                       long dur_num, long dur_den, bool aux)
{
    TRACE("Speller: add {} {} {}", midi, bar, PSRatio(dur_num, dur_den));
    add(midi, bar, simultaneous, aux, PSRatio(dur_num, dur_den));
}

// for pybind
void SpellerEnum::add4(int midi, int bar, bool simultaneous,
                       const enum NoteName& name, const enum Accid& accid,
                       int octave, bool printed, bool aux)
{
    TRACE("Speller: add {} {}", midi, bar);
    add(midi, bar, simultaneous, aux, PSRatio(0), name, accid, octave, printed);
}


// for pybind
void SpellerEnum::add6(int midi, int bar, bool simultaneous,
                       long dur_num, long dur_den,
                       const enum NoteName& name, const enum Accid& accid,
                       int octave, bool printed, bool aux)
{
    TRACE("Speller: add {} {} {}", midi, bar, PSRatio(dur_num, dur_den));
    add(midi, bar, simultaneous, aux, PSRatio(dur_num, dur_den),
        name, accid, octave, printed);
}


PSRawEnum& SpellerEnum::rawenum(bool aux) const
{
    PSEnum* e = (aux?_enum_aux:_enum);
    assert(e);
    PSRawEnum* penum = dynamic_cast<PSRawEnum*>(e);
    assert(penum);
    return *penum;
}


} // namespace pse
