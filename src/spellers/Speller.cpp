//
//  Speller.cpp
//  pypse
//
//  Created by Florent Jacquemard on 22/11/2022.
//

#include "Speller.hpp"
#include "PSGridy.hpp"
#include "PSGridr.hpp"
#include "PSGridx.hpp"


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


Speller::Speller(PSEnum* e, PSEnum* e_aux,
                 size_t nbton, const Algo& algo, bool dflag):
Spelli(nbton, dflag),
_algo(algo),
_enum(e),
_enum_aux(e_aux),
_table(nullptr),
_grid(nullptr)
{
    assert(e);
}


Speller::Speller(PSEnum* e, std::shared_ptr<TonIndex> id,  PSEnum* e_aux,
                 const Algo& algo, bool dflag):
Spelli(id, dflag),
_algo(algo),
_enum(e),
_enum_aux(e_aux),
_table(nullptr),
_grid(nullptr)
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
    
    if (_table)
    {
        delete _table;
        _table = nullptr;
    }

    if (_grid)
    {
        delete _grid;
        _grid = nullptr;
    }
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
// parameters
//


size_t Speller::size(bool aux) const
{
    assert(not aux or hasAuxEnumerator());
    return enumerator(aux).size();
}


bool Speller::hasAuxEnumerator() const
{
    return (_enum_aux != nullptr);
}


bool Speller::setAuxEnumerator(PSEnum* aux)
{
    assert(aux);
    if (_enum_aux != nullptr)
    {
        ERROR("Speller addAuxEnumerator: there is already an auxiliary enumerator.");
        return false;
    }
    else
    {
        _enum_aux = aux;
        return true;
    }
}


//
// spelling
//

bool Speller::evalTable(CostType ctype, bool tonal, bool octave, 
                        bool chromatic, bool aux)
{
    TRACE("Speller: eval table with {}, unlead={}, det={}, {} enumerator",
          ctype, tonal, chromatic, (aux?"auxiliary":"main"));

    if (ctype == CostType::UNDEF)
    {
        ERROR("Speller eval Table: undefined cost type, ignored.");
        return false;
    }

    if (_table)
    {
        WARN("Speller evalTable: deleting current pitch spelling table");
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
    std::unique_ptr<Cost> seed = unique_zero(ctype); // was sampleCost(ctype)
    assert(seed);
    _table = new PST(algo, *seed, index(), enumerator(aux),
                     tonal, octave, _debug);
    return true;
}


bool Speller::revalTable(CostType ctype, bool tonal, bool octave,
                         bool chromatic, bool aux)
{
    TRACE("Speller: reval table with {}, unlead={}, det={}, {} enumerator",
          ctype, tonal, chromatic, (aux?"auxiliary":"main"));
    if (ctype == CostType::UNDEF)
    {
        ERROR("Speller reval Table: undefined cost type, ignored.");
        return false;
    }

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
    // PST* table_pre = _table;
    const Algo algo(chromatic?Algo::PSD:Algo::PSE);
    
    assert(_enum);
    std::unique_ptr<Cost> seed = unique_zero(ctype); // was sampleCost(ctype)
    assert(seed);
    _table = new PST(algo, // *table_pre,
                     *seed, index(), enumerator(aux), *_grid,
                     tonal, octave, _debug);
    //assert(table_pre);
    //delete table_pre;
    return true;
}


bool Speller::evalGrid(const GridAlgo& algo)
{
    if (_grid)
    {
        WARN("Speller evalGrid: deleting current grid");
        delete _grid;
        _grid = nullptr;
    }
    if (_table == nullptr)
    {
        ERROR("Speller evalGrid: eval table first");
        return false;
    }
    
    assert(_index);
    //std::vector<bool> mask(_index->size(), true); // all true by default

    switch (algo)
    {
        case GridAlgo::Best:
            _grid = new PSGy(*_table);
            break;

        case GridAlgo::Rank:
            _grid = new PSGr(*_table);
            break;

        case GridAlgo::Exhaustive:
            _grid = new PSGx(*_table);
            break;

        default:
        {
            ERROR("unexpected Grid algorithm");
            break;
        }
    }

    return true;
}


size_t Speller::selectGlobals(double d, bool refine)
{
    assert(0 <= d);
    assert(d <= 100);
    
    if (_table == nullptr)
    {
        ERROR("Speller selectGlobals: eval table first");
        return 0;
    }
    // if (_debug)
    // {
    //     _table->print(std::cout);
    // }
    assert(_index);
    return _index->selectGlobals(*_table, d, refine);
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

    TRACE("rename with ton={} in enumerator of size={}",
          i, _table->enumerator().size());

    return _table->rename(i);
}


size_t Speller::rewritePassing(bool aux)
{
    TRACE("Rewriting passing notes");
    return enumerator(aux).rewritePassing();
}


bool Speller::spell()
{
    ERROR("Speller spell should not be called");
    return false;
}


void Speller::resetTable()
{
    if (_table)
    {
        TRACE("Speller: deleting current pitch spelling table");
        delete _table;
        _table = nullptr;
    }
}


void Speller::resetGrid()
{
    if (_grid)
    {
        TRACE("Speller evalGrid: deleting current grid");
        delete _grid;
        _grid = nullptr;
    }
}


//
// results feedback
//


size_t Speller::measures(bool aux) const
{
    const PSEnum& psenum(enumerator(aux));
    size_t m;
    if (psenum.empty())
    {
        m = 0;
    }
    else
    {
        size_t last = psenum.stop();
        if (last == 0)
        {
            m = 0;
        }
        else
        {
            m = psenum.measure(last-1)+1;
        }
    }
        
    assert(_table == nullptr or _table->size() == m);
    // not true if the last measure is empty!
    // assert(_grid == nullptr or _grid->size() == m);
    return m;
}


enum NoteName Speller::name(size_t i, bool aux) const
{
    return enumerator(aux).name(i);
}


enum Accid Speller::accidental(size_t i, bool aux) const
{
    return enumerator(aux).accidental(i);
}


int Speller::octave(size_t i, bool aux) const
{
    return enumerator(aux).octave(i);
}


bool Speller::printed(size_t i, bool aux) const
{
    return enumerator(aux).printed(i);
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
    else if (j >= _grid->measures())
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
    
    // in case of error return undefined tonality index
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
    assert(_grid);
    const PSEnum& psenum(_grid->enumerator());
    assert(psenum.inside(j));
    size_t bar = psenum.measure(j);
    return local(i, bar);
}


PSEnum& Speller::enumerator(bool aux) const
{
    if (aux)
    {
        assert(_enum_aux);
        return *_enum_aux;
    }
    else
    {
        assert(_enum);
        return *_enum;
    }
}


// static
double Speller::duration(clock_t start)
{
    return ((double)(clock() - start)/CLOCKS_PER_SEC * 1000);
}


void Speller::printGrid(std::ostream& o) const
{
    if (_grid)
        _grid->print(o);
}


void Speller::printGrid_pybind() const
{
    if (_grid)
        _grid->print(std::cout);
}


void Speller::writeGrid(const std::string& filename) const
{
    if (_grid)
    {
        std::ofstream file;
        file.open(filename);
        if (file.is_open())
        {
            _grid->print(file);
            file.close();
        }
        else
            ERROR("Speller: Unable to open file {}", filename);
    }
}

void Speller::printTable(std::ostream& o) const
{
    if (_table)
        _table->print(o);
}

void Speller::printTable_pybind() const
{
    if (_table)
        _table->print(std::cout);
}

void Speller::writeTable(const std::string& filename) const
{
    if (_table)
    {
        std::ofstream file;
        file.open(filename);
        if (file.is_open())
        {
            _table->print(file);
            file.close();
        }
        else
            ERROR("Speller: Unable to open file {}", filename);
    }
}


} // namespace pse
