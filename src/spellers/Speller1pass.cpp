//
//  Speller1pass.cpp
//  pse
//
//  Created by Florent Jacquemard on 31/05/2023.
//

#include "Speller1pass.hpp"

namespace pse {

Speller1Pass::Speller1Pass(const Algo& algo, size_t nbTons, bool dflag):
Speller(algo, nbTons, dflag),
_table0(nullptr),
_global0(nullptr),
_locals0(nullptr),
_uton(new Ton())
{ }


Speller1Pass::~Speller1Pass()
{
    if (_table0)
        delete _table0;
    if (_global0)
        delete _global0;
    if (_locals0)
        delete _locals0;
    assert(_uton);
    delete _uton;
}


void Speller1Pass::setGlobal(size_t i, PSO* g) // std::shared_ptr<PSO>
{
    if (i >= _index.size())
    {
        WARN("Speller1Pass: set global {}: not a ton (ignored)", i);
        return;
    }
    if (g == nullptr)
    {
        g = new PSO(_index, _debug); // empty
    }
    else
    {
        WARN("Speller1Pass: set global {}: there are already () global candidates",
             i, g->size());
    }
    g->setGlobal(i);
}
 

size_t Speller1Pass::globals(const PSO* g) const // std::shared_ptr<PSO>
{
    if (g == nullptr)
    {
        return 0;
    }
    else
    {
        return g->size();
    }
}

 
const Ton& Speller1Pass::globalCand(size_t i, const PSO* g) const // std::shared_ptr<PSO>
{
    // size_t it = iglobalCand(i);
    // if (it != TonIndex::UNDEF)
    if ((g != nullptr) && (i <  g->size()))
    {
        assert(_index.size() == g->size());
        return g->global(i);
    }
    else
    {
        ERROR("Speller1Pass: iglobal {}: no such global ton candidate", i);
        //std::shared_ptr<Ton> uton(new Ton()); // undefined tonality
        assert(_uton);
        return *_uton;
    }
}
 

size_t Speller1Pass::iglobalCand(size_t i, const PSO* g) const // std::shared_ptr<PSO>
{
    if (i < globals())
    {
        return g->iglobal(i);
    }
    else
    {
        WARN("Speller1Pass: iglobal {}: no such global ton candidates (only {})",
             i, globals());
        return TonIndex::UNDEF;
    }

}


void Speller1Pass::setGlobal(size_t i)
{
    setGlobal(i, _global0);
}


size_t Speller1Pass::globals() const
{
    return globals(_global0);
}


const Ton& Speller1Pass::globalCand(size_t i) const
{
    return globalCand(i, _global0);
}


size_t Speller1Pass::iglobalCand(size_t i) const
{
    return iglobalCand(i, _global0);
}


const Ton& Speller1Pass::localCandBar(size_t i, size_t j) const
{
    if (_locals0 == nullptr)
    {
        ERROR("Speller1Pass local: call spell() first");
    }
    else if (j >= _locals0->columnNb())
    {
        ERROR("Speller1Pass local: no bar {}", j);
    }
    else
    {
        assert(i < _locals0->rowNb());
        size_t it = _locals0->ilocal(i, j);
        assert(it < _index.size());
        return _index.ton(it);
    }

    // in case or error return undefined tonality
    std::shared_ptr<Ton> uton(new Ton());
    return *uton;
}


const Ton& Speller1Pass::local(size_t j) const
{
    return localCandBar(iglobal(), j);
}


const Ton& Speller1Pass::localNote(size_t i) const
{
    assert(_enum.inside(i));
    size_t j = _enum.measure(i);
    return local(j);
}


bool Speller1Pass::spell(const Cost& seed0, double diff0, bool rewrite_flag0)
{
    bool status = true;
    
    TRACE("pitch-spelling: building first pitch-spelling table");
    if (_table0 != nullptr)
    {
        WARN("PSE: re-spelling, PS table overrided");
        delete _table0;
    }
    _table0 = new PST(_algo, seed0, _index, _enum, _debug); // std::unique_ptr<PST>
       
    TRACE("pitch-spelling: {} bars", _table0->size());

    TRACE("pitch-spelling: estimate first list of global tonality candidates");
    if (_global0 != nullptr)
    {
        WARN("PSE: re-spelling, PS global set overrided");
        delete _global0;
    }
    assert(diff0 >= 0);
    _global0 = new PSO(*_table0, diff0, _debug); // std::unique_ptr<PSO>

    // extract local tonality for each column of table
    TRACE("pitch-spelling: start local tonalities estimation");
    if (_locals0 != nullptr)
    {
        WARN("PSE: re-spelling, PS local grid overrided");
        delete _locals0;
    }
    _locals0 = new PSG(*_table0, _global0->getMask()); // std::unique_ptr<PSG>

    //    if (status == false)
    //    {
    //        ERROR("Pitch Spelling: failed to extract local tonalities, abort.");
    //        return false;
    //    }


    TRACE("Pitch Spelling: estimated global tonality: {} ({})",
          global(), global().fifths());

//    if (status == false)
//    {
//        ERROR("Speller: failed to compute spelling table {}-{}",
//              _enum.first(), _enum.stop());
//    }

    if (rewrite_flag0)
    {
        TRACE("pitch-spelling: rewrite passing notes");
        _table0->enumerator().rewritePassing();
    }

    // will update the lists _names, _accids and _octave
    TRACE("pitch-spelling: start renaming");
    _table0->rename(iglobal());

    return status;
}


} // namespace pse
