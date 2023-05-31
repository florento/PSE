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
    // free pointers
}


void Speller1Pass::setGlobal(size_t i, std::shared_ptr<PSO> g)
{
    if (i >= _index.size())
    {
        WARN("Speller1Pass: set global {}: not a ton (ignored)", i);
        return;
    }
    if (g == nullptr)
    {
        g = std::unique_ptr<PSO>(new PSO(_index, _debug)); // empty
    }
    else
    {
        WARN("Speller1Pass: set global {}: already global candidates", i);
    }
    g->setGlobal(i);
}
 

size_t Speller1Pass::globals(const std::shared_ptr<PSO> g) const
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

 
const Ton& Speller1Pass::globalCand(size_t i, const std::shared_ptr<PSO> g) const
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
 

size_t Speller1Pass::iglobalCand(size_t i, const std::shared_ptr<PSO> g) const
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




} // namespace pse
