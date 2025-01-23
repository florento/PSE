//
//  Speller.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "Speller.hpp"
#include "PSGridr.hpp"



namespace pse {

//Algo Speller::algo() const
//{
//    return Algo::Undef;
//}


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


Speller::Speller(PSEnum* e, size_t nbton, const Algo& algo, bool dflag):
Spelli(nbton, dflag),
_algo(algo),
_enum(e),
_table(nullptr),
_grid(nullptr),
_global(nullptr)
{ 
    assert(e);
}


Speller::Speller(PSEnum* e, std::shared_ptr<TonIndex> id,
        const Algo& algo, bool dflag):
Spelli(id, dflag),
_algo(algo),
_enum(e),
_table(nullptr),
_grid(nullptr),
_global(nullptr)
{
    assert(e);
}


// copy forbiddend
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


//
// spelling
//

bool Speller::evalTable(CostType ctype, bool tonal, bool chromatic)
{
    if (_table)
    {
        TRACE("deleting current pitch spelling table");
        delete _table;
        _table = nullptr;
    }
    
//    if (_seed == nullptr)
//    {
//        WARN("Speller evalTable: no type of cost specified, using default");
//        _seed = new CostA(true); // with discount
//    }
    
    /// @todo remplacer algo par flag chromatic
    const Algo algo(chromatic?Algo::PSD:Algo::PSE);
    assert(_enum);
    _table = new PST(algo, sampleCost(ctype), index(), *_enum, tonal, _debug);

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
    
    /// @todo suppr. _global et full, remplacé par index de table_pre (flag global)
    /// @todo remplacer algo par flag chromatic
    if (_global)
    {
        _table = new PST(algo, *table_pre, sampleCost(ctype), *_global, *_grid,
                         tonal, _debug);
    }
    else
    {
        PSO full(index(), _debug, true);
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
    
    assert(_index);
    std::vector<bool> mask(_index->size(), true); // all true by default

//    if (_global)
//    {
//        mask = _global->getMask(); // copy
//    }

    _grid = new PSGr(*_table);
    return true;
}


bool Speller::selectGlobals(double d, bool refine)
{
    assert(0 <= d);
    assert(d <= 100);
    
    if (_table == nullptr)
    {
        ERROR("Speller selectGlobals: eval table first");
        return false;
    }
    // if (_debug)
    // {
    //     _table->dump_table();
    // }
    assert(_index);
    _index->selectGlobals(*_table, d, refine);
    return true;
//    if (refine)
//    {
//        if (_global == nullptr)
//        {
//            ERROR("Speller evalGlobal by refinement: no previous global");
//            return false;
//        }
//        PSO* global_pre = _global;
//        _global = new PSO(*global_pre, *_table, d, _debug);
//        assert(global_pre);
//        delete global_pre;
//        return true;
//    }
//    else
//    {
//        if (_global)
//        {
//            delete _global;
//            _global = nullptr;
//        }
//        _global = new PSO(*_table, d, _debug);
//        return true;
//    }
}


bool Speller::selectGlobal()
{
    assert(_index);
    return _index->selectGlobal();
}


bool Speller::rename(size_t i)
{
    assert(i != TonIndex::UNDEF);
    assert(_index);
    assert(i < _index->size());

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
    assert(_enum);
    return _enum->rewritePassing();
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
    assert(_enum);
    return _enum->name(i);
}


enum Accid Speller::accidental(size_t i) const

{
    assert(_enum);
    return _enum->accidental(i);
}


int Speller::octave(size_t i) const
{
    assert(_enum);
    return _enum->octave(i);
}


bool Speller::printed(size_t i) const
{
    assert(_enum);
    return _enum->printed(i);
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
    else if (j >= _grid->nbMeasures())
    {
        ERROR("Speller ilocal: no bar {}", j);
    }
    else if (i >= _grid->nbTons())
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
        assert(_index);
        assert(it < _index->size());
        return _index->ton(it);
    }
}


const Ton& Speller::localNote(size_t i, size_t j) const
{
    assert(_enum);
    assert(_enum->inside(j));
    size_t bar = _enum->measure(j);
    return local(i, bar);
}


// static
double Speller::duration(clock_t start)
{
    return ((double)(clock() - start)/CLOCKS_PER_SEC * 1000);
}


} // namespace pse
