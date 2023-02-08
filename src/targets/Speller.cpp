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
    return _table.nbTons();
}


const Ton& Speller::ton(size_t i) const
{
    return _table.ton(i);
}


void Speller::resetTons()
{
    _table.resetTons();
}


void Speller::addTon(const Ton ton)
{
    TRACE("Speller: add tonality {}", ton);
    _table.addTon(ton);
}


void Speller::addTon(int ks, Ton::Mode mode)
{
    if (ks < -7 || 7 < ks)
    {
        ERROR("Speller addTon: wrong key signature value {}", ks);
    }
    TRACE("Speller: add tonality {} {}", ks, mode);
    _table.addTon(ks, mode);
}


bool Speller::respell()
{
    //DEBUGU("Speller respell: nb tonalities in table: {}", _table.nbTons());
    if (_table.nbTons() == 0)
    {
        TRACE("Speller respell: no tonality added, use default tonality array");
        for (int ks = -7; ks <= 7; ++ks)
            _table.addTon(ks, Ton::Mode::Maj);
        for (int ks = -7; ks <= 7; ++ks)
            _table.addTon(ks, Ton::Mode::Min);
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



// TONS30
const std::vector<const Ton> Speller::TONS =
{
    Ton(-7, Ton::Mode::Maj),
    Ton(-6, Ton::Mode::Maj),
    Ton(-5, Ton::Mode::Maj),
    Ton(-4, Ton::Mode::Maj),
    Ton(-3, Ton::Mode::Maj),
    Ton(-2, Ton::Mode::Maj),
    Ton(-1, Ton::Mode::Maj),
    Ton(0,  Ton::Mode::Maj),
    Ton(1,  Ton::Mode::Maj),
    Ton(2,  Ton::Mode::Maj),
    Ton(3,  Ton::Mode::Maj),
    Ton(4,  Ton::Mode::Maj),
    Ton(5,  Ton::Mode::Maj),
    Ton(6,  Ton::Mode::Maj),
    Ton(7,  Ton::Mode::Maj),
    Ton(-7, Ton::Mode::Min),
    Ton(-6, Ton::Mode::Min),
    Ton(-5, Ton::Mode::Min),
    Ton(-4, Ton::Mode::Min),
    Ton(-3, Ton::Mode::Min),
    Ton(-2, Ton::Mode::Min),
    Ton(-1, Ton::Mode::Min),
    Ton(0,  Ton::Mode::Min),
    Ton(1,  Ton::Mode::Min),
    Ton(2,  Ton::Mode::Min),
    Ton(3,  Ton::Mode::Min),
    Ton(4,  Ton::Mode::Min),
    Ton(5,  Ton::Mode::Min),
    Ton(6,  Ton::Mode::Min),
    Ton(7,  Ton::Mode::Min)
};


const std::vector<const Ton> Speller::TONS26 =
{
    Ton(-6, Ton::Mode::Maj),
    Ton(-5, Ton::Mode::Maj),
    Ton(-4, Ton::Mode::Maj),
    Ton(-3, Ton::Mode::Maj),
    Ton(-2, Ton::Mode::Maj),
    Ton(-1, Ton::Mode::Maj),
    Ton(0,  Ton::Mode::Maj),
    Ton(1,  Ton::Mode::Maj),
    Ton(2,  Ton::Mode::Maj),
    Ton(3,  Ton::Mode::Maj),
    Ton(4,  Ton::Mode::Maj),
    Ton(5,  Ton::Mode::Maj),
    Ton(6,  Ton::Mode::Maj),
    Ton(-6, Ton::Mode::Min),
    Ton(-5, Ton::Mode::Min),
    Ton(-4, Ton::Mode::Min),
    Ton(-3, Ton::Mode::Min),
    Ton(-2, Ton::Mode::Min),
    Ton(-1, Ton::Mode::Min),
    Ton(0,  Ton::Mode::Min),
    Ton(1,  Ton::Mode::Min),
    Ton(2,  Ton::Mode::Min),
    Ton(3,  Ton::Mode::Min),
    Ton(4,  Ton::Mode::Min),
    Ton(5,  Ton::Mode::Min),
    Ton(6,  Ton::Mode::Min),
};


// TBR not used
//std::array<const AccidState, PSV::NBTONS> PSV::ASTATES =
//{
//    AccidState(TONS[0],  false),
//    AccidState(TONS[1],  false),
//    AccidState(TONS[2],  false),
//    AccidState(TONS[3],  false),
//    AccidState(TONS[4],  false),
//    AccidState(TONS[5],  false),
//    AccidState(TONS[6],  false),
//    AccidState(TONS[7],  false),
//    AccidState(TONS[8],  false),
//    AccidState(TONS[9],  false),
//    AccidState(TONS[10], false),
//    AccidState(TONS[11], false),
//    AccidState(TONS[12], false),
//    AccidState(TONS[13], false),
//    AccidState(TONS[14], false),
//    AccidState(TONS[15], false),
//    AccidState(TONS[16], false),
//    AccidState(TONS[17], false),
//    AccidState(TONS[18], false),
//    AccidState(TONS[19], false),
//    AccidState(TONS[20], false),
//    AccidState(TONS[21], false),
//    AccidState(TONS[22], false),
//    AccidState(TONS[23], false),
//    AccidState(TONS[24], false),
//    AccidState(TONS[25], false),
//    AccidState(TONS[26], false),
//    AccidState(TONS[27], false),
//    AccidState(TONS[28], false),
//    AccidState(TONS[29], false)
//};


// TBR not used
//std::array<const AccidState, PSV::NBTONS> PSV::AJOKER =
//{
//    AccidState(TONS[0],  true),
//    AccidState(TONS[1],  true),
//    AccidState(TONS[2],  true),
//    AccidState(TONS[3],  true),
//    AccidState(TONS[4],  true),
//    AccidState(TONS[5],  true),
//    AccidState(TONS[6],  true),
//    AccidState(TONS[7],  true),
//    AccidState(TONS[8],  true),
//    AccidState(TONS[9],  true),
//    AccidState(TONS[10], true),
//    AccidState(TONS[11], true),
//    AccidState(TONS[12], true),
//    AccidState(TONS[13], true),
//    AccidState(TONS[14], true),
//    AccidState(TONS[15], true),
//    AccidState(TONS[16], true),
//    AccidState(TONS[17], true),
//    AccidState(TONS[18], true),
//    AccidState(TONS[19], true),
//    AccidState(TONS[20], true),
//    AccidState(TONS[21], true),
//    AccidState(TONS[22], true),
//    AccidState(TONS[23], true),
//    AccidState(TONS[24], true),
//    AccidState(TONS[25], true),
//    AccidState(TONS[26], true),
//    AccidState(TONS[27], true),
//    AccidState(TONS[28], true),
//    AccidState(TONS[29], true)
//};




} // namespace pse
