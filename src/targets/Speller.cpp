//
//  Speller.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "Speller.hpp"


namespace pse {


Speller::Speller(bool finit):
_enum(0, 0),
_table(_enum),
_global(0) // C maj
{
    spdlog_setVerbosity(4);
    spdlog_setPattern();
    
    // init table with default vector of tons
//    if (finit)
//    {
//        for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
//        _rowcost.assign(nbtons(), 0);
//        _frowcost.assign(nbtons(), false);
//        bool status = init();
//        if (status == false)
//        {
//            ERROR("PST: fail to compute spelling table {}-{}",
//                  _enum.first(), _enum.stop());
//        }
//    }
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


size_t Speller::nbTons() const
{
    return _table.index.size();  // nbTons();
}


const Ton& Speller::ton(size_t i) const
{
    return _table.index.ton(i);
}


void Speller::resetTons()
{
    _table.index.reset();
}


void Speller::addTon(const Ton& ton)
{
    TRACE("Speller: add tonality {}", ton);
    _table.index.add(ton);
}


void Speller::addTon(int ks, Ton::Mode mode)
{
    if (ks < -7 || 7 < ks)
    {
        ERROR("Speller addTon: wrong key signature value {}", ks);
    }
    TRACE("Speller: add tonality {} {}", ks, mode);
    _table.index.add(ks, mode);
}


bool Speller::respell()
{
    //DEBUGU("Speller respell: nb tonalities in table: {}", _table.nbTons());
    if (_table.index.size() == 0)
    {
        TRACE("Speller respell: no tonality added, use default tonality array");
        for (int ks = -7; ks <= 7; ++ks)
            _table.index.add(ks, Ton::Mode::Maj);
        for (int ks = -7; ks <= 7; ++ks)
            _table.index.add(ks, Ton::Mode::Min);
    }
    
    //    if (finit)
    //    {
    //        for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
    //        _rowcost.assign(nbtons(), 0);
    //        _frowcost.assign(nbtons(), false);
    //    }

    TRACE("pitch-spelling: building pitch-spelling table");
    bool status = _table.init();
    if (status == false)
    {
        ERROR("PST: fail to compute spelling table {}-{}",
              _enum.first(), _enum.stop());
        return false;
    }

    TRACE("pitch-spelling: {} bars", _table.size());
    // estimate global tonality for table
    TRACE("pitch-spelling: start global tonality estimation");
    status = _table.estimateGlobal();
    if (status == false)
    {
        ERROR("Pitch Spelling: failed to extract global tonality, abort.");
        return false;
    }
    const pse::Ton& gton = _table.global();
    _global = gton;
    DEBUGU("Pitch Spelling: estimated global tonality: {} ({})",
          gton, gton.fifths());

    // extract local tonality for each column of table
    DEBUGU("pitch-spelling: start local tonalities estimation");
    status = _table.estimateLocals();
    if (status == false)
    {
        ERROR("Pitch Spelling: failed to extract local tonalities, abort.");
        return false;
    }

    // will update the lists _names, _accids and _octave
    TRACE("pitch-spelling: start renaming");
    _table.rename();
    return true;
}


//const Ton& Speller::global() const
//{
//    return _global;
//}




// TBR not used
//std::array<const PSState, PSV::NBTONS> PSV::ASTATES =
//{
//    PSState(TONS[0],  false),
//    PSState(TONS[1],  false),
//    PSState(TONS[2],  false),
//    PSState(TONS[3],  false),
//    PSState(TONS[4],  false),
//    PSState(TONS[5],  false),
//    PSState(TONS[6],  false),
//    PSState(TONS[7],  false),
//    PSState(TONS[8],  false),
//    PSState(TONS[9],  false),
//    PSState(TONS[10], false),
//    PSState(TONS[11], false),
//    PSState(TONS[12], false),
//    PSState(TONS[13], false),
//    PSState(TONS[14], false),
//    PSState(TONS[15], false),
//    PSState(TONS[16], false),
//    PSState(TONS[17], false),
//    PSState(TONS[18], false),
//    PSState(TONS[19], false),
//    PSState(TONS[20], false),
//    PSState(TONS[21], false),
//    PSState(TONS[22], false),
//    PSState(TONS[23], false),
//    PSState(TONS[24], false),
//    PSState(TONS[25], false),
//    PSState(TONS[26], false),
//    PSState(TONS[27], false),
//    PSState(TONS[28], false),
//    PSState(TONS[29], false)
//};


// TBR not used
//std::array<const PSState, PSV::NBTONS> PSV::AJOKER =
//{
//    PSState(TONS[0],  true),
//    PSState(TONS[1],  true),
//    PSState(TONS[2],  true),
//    PSState(TONS[3],  true),
//    PSState(TONS[4],  true),
//    PSState(TONS[5],  true),
//    PSState(TONS[6],  true),
//    PSState(TONS[7],  true),
//    PSState(TONS[8],  true),
//    PSState(TONS[9],  true),
//    PSState(TONS[10], true),
//    PSState(TONS[11], true),
//    PSState(TONS[12], true),
//    PSState(TONS[13], true),
//    PSState(TONS[14], true),
//    PSState(TONS[15], true),
//    PSState(TONS[16], true),
//    PSState(TONS[17], true),
//    PSState(TONS[18], true),
//    PSState(TONS[19], true),
//    PSState(TONS[20], true),
//    PSState(TONS[21], true),
//    PSState(TONS[22], true),
//    PSState(TONS[23], true),
//    PSState(TONS[24], true),
//    PSState(TONS[25], true),
//    PSState(TONS[26], true),
//    PSState(TONS[27], true),
//    PSState(TONS[28], true),
//    PSState(TONS[29], true)
//};




} // namespace pse
