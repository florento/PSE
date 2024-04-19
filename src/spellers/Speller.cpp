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
//_seed(nullptr),
//_initial_state(0), // default
//_chromatic(false),
_seedAdiscount(true),
_seedAnodiscount(false),
_seedADplus(),
_seedADlex(),
_table(nullptr),
_grid(nullptr),
_global(nullptr),
_debug(dflag),
_uton(new Ton()) // undef
{
    spdlog_setVerbosity(4);
    debug(dflag);
    spdlog_setPattern();
}


//Speller::Speller(const Speller& rhs):
//_algo(rhs._algo),
//_enum(rhs._enum),  /// @todo AV copy
//_index(rhs._index), /// @todo AV copy
//_debug(rhs._debug)
//{
//    TRACE("Speller copy");
//}


Speller::~Speller()
{
    TRACE("delete Speller of type {}", _algo);
    
    if (_grid)
        delete _grid;
    if (_table)
        delete _table;
 
    assert(_uton);
    delete _uton;
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
    _debug = flag;
    if (flag)
        spdlog_setVerbosity(5);
    else
        spdlog_setVerbosity(4);
}


//
// notes
//


size_t Speller::size() const
{
    //TRACE("Speller::size");
    return _enum.size();
}


void Speller::reset()
{
    _enum.reset();
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


//
// array of tonalities
//


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

void Speller::WeberTonal()
{
    _index.setTonal();
}


void Speller::WeberModal()
{
    _index.setModal();
}


void Speller::closeTons(bool tonal_flag)
{
    _index.close(tonal_flag);
}


bool Speller::closedTons() const
{
    return _index.closed();
}


//
// prepare spelling
//

Cost& Speller::sampleCost(CostType ct)
{
    switch (ct)
    {
        case CostType::ACCIDlead:
            return _seedAdiscount;
            break;
            
        case CostType::ACCID:
            return _seedAnodiscount;
            break;
            
        case CostType::ADplus:
            return _seedADplus;
            break;
            
        case CostType::ADlex:
            return _seedADlex;
            break;
            
        default:
            ERROR("Speller sampleCost unexpected code {}", ct);
            return _seedAdiscount;
    }
}


//void Speller::setTonal()
//{
//    _initial_state = 1;
//}


//void Speller::setModal()
//{
//    _initial_state = 2;
//}


//void Speller::setChromatic(bool flag)
//{
//    _chromatic = flag;
//}


//
// spelling
//


bool Speller::evalTable(CostType ctype, bool tonal, bool chromatic)
{
    if (_table)
    {
        delete _table;
        _table = nullptr;
    }
    
//    if (_seed == nullptr)
//    {
//        WARN("Speller evalTable: no type of cost specified, using default");
//        _seed = new CostA(true); // with discount
//    }
    
    const Algo algo(chromatic?Algo::PSD:Algo::PSE);
    _table = new PST(algo, sampleCost(ctype), _index, _enum, tonal, _debug);

    return true;
}


bool Speller::revalTable(CostType ctype, bool tonal, bool chromatic)
{
    if (_table == nullptr)
    {
        ERROR("Speller revalTable: call evalTable first");
        return false;
    }

    if (_grid == nullptr)
    {
        ERROR("Speller revalTable: call evalGrid first");
        return false;
    }

//  if (_seed == nullptr)
//  {
//      WARN("Speller revalTable: no type of cost specified, using default");
//      _seed = new CostA(true); // with discount
//  }
    
    assert(_table);
    PST* table_pre = _table;
    const Algo algo(chromatic?Algo::PSD:Algo::PSE);
    
    
    if (_global)
    {
        _table = new PST(algo, *table_pre, sampleCost(ctype), *_global, *_grid,
                         tonal, _debug);
    }
    else
    {
        PSO full(_index, _debug, true);
        _table = new PST(algo, *table_pre, sampleCost(ctype), full, *_grid,
                         tonal, _debug);
    }

    assert(table_pre);
    delete table_pre;
    return true;
}


bool Speller::evalGrid()
{
    if (_grid)
    {
        delete _grid;
        _grid = nullptr;
    }
    if (_table == nullptr)
    {
        ERROR("Speller evalGrid: eval table first");
        return false;
    }
    
    std::vector<bool> mask(_index.size(), true); // all true by default

    if (_global)
    {
        mask = _global->getMask(); // copy
    }

    _grid = new PSG(*_table, mask);
    return true;
}


bool Speller::evalGlobal(double d, bool refine)
{
    assert(0 <= d);
    assert(d <= 100);
    
    if (_table == nullptr)
    {
        ERROR("Speller evalGlobal: eval table first");
        return false;
    }
    if (_debug)
        _table->dump_table();
    if (refine)
    {
        if (_global == nullptr)
        {
            ERROR("Speller evalGlobal by refinement: no previous global");
            return false;
        }
        PSO* global_pre = _global;
        _global = new PSO(*global_pre, *_table, d, _debug);
        assert(global_pre);
        delete global_pre;
        return true;
    }
    else
    {
        if (_global)
        {
            delete _global;
            _global = nullptr;
        }
        _global = new PSO(*_table, d, _debug);
        return true;
    }
}


bool Speller::rename(size_t i)
{
    assert(i != TonIndex::UNDEF);
    assert(i < _index.size());

    if (_table == nullptr)
    {
        ERROR("Speller rename: eval table first");
        return false;
    }
    
    return _table->rename(i);
}


size_t Speller::rewritePassing()
{
    TRACE("Rewriting passing notes");
    return _enum.rewritePassing();
}


bool Speller::spell()
{
    ERROR("Speller spell should not be called");
    return false;
}


//
// results feedback
//

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


size_t Speller::globals() const
{
    if (_global == nullptr)
    {
        WARN("Speller globals: evalGlobal not called");
        return 0;
    }
    else
    {
        return _global->size();
    }
}


const Ton& Speller::global(size_t n) const
{
    if (_global == nullptr)
    {
        ERROR("Speller global: evalGlobal not called");
        assert(_uton);
        return *_uton;
    }
    else
    {
        return _global->global(n);
    }
}


size_t Speller::iglobal(size_t n) const
{
    if (_global == nullptr)
    {
        ERROR("Speller iglobal: evalGlobal not called");
        return TonIndex::UNDEF;
    }
    else
    {
        return _global->iglobal(n);
    }
}


bool Speller::locals() const
{
    return (_grid != nullptr);
}


size_t Speller::ilocal(size_t i, size_t j) const
{
    if (_grid == nullptr)
    {
        ERROR("Speller ilocal: eval grid first");
    }
    else if (j >= _grid->columnNb())
    {
        ERROR("Speller ilocal: no bar {}", j);
    }
    else if (i >= _grid->rowNb())
    {
        ERROR("Speller ilocal: no ton of index {}", i);
    }
    else
    {
        return _grid->ilocal(i, j);
    }
    
    // in case or error return undefined tonality index
    return TonIndex::UNDEF;
}


const Ton& Speller::local(size_t i, size_t j) const
{
    size_t it = ilocal(i, j);
    if (it == TonIndex::UNDEF)
    {
        // in case or error return undefined tonality
        assert(_uton);
        return *_uton;
    }
    else
    {
        assert(it < _index.size());
        return _index.ton(it);
    }
}


const Ton& Speller::localNote(size_t i, size_t j) const
{
    assert(_enum.inside(j));
    size_t bar = _enum.measure(j);
    return local(i, bar);
}


// static
double Speller::duration(clock_t start)
{
    return ((double)(clock() - start)/CLOCKS_PER_SEC * 1000);
}


} // namespace pse
