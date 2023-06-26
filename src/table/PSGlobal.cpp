//
//  PSGlobal.cpp
//  pse
//
//  Created by Florent Jacquemard on 25/05/2023.
//

#include "PSGlobal.hpp"
#include "PSTable.hpp"


namespace pse {


PSO::PSO(const PSO& globals, const PST& tab, double d, bool dflag):
_index(tab.index()),
_globals(), // empty
_debug(dflag)
{
    assert(d >= 0);
    assert(globals._index.size() == tab.index().size());
    // assert(globals._index == tab.index());
    init(globals, tab, d);
}


PSO::PSO(const PST& tab, double d, bool dflag):
PSO(PSO(tab.index(), dflag, true), tab, d, dflag)
{ assert(d >= 0); }


PSO::PSO(const TonIndex& id, double dflag, double full):
_index(id),
_globals(), // empty
_debug(dflag)
{
    if (full)
    {
        for (size_t i = 0; i < _index.size(); ++i)
            _globals.push_back(i);
    }
}


PSO::~PSO()
{ }


void PSO::init(const PSO& globals, const PST& tab, double d)
{
    // no former candidates for best global
    if (globals.empty())
    {
        assert(_globals.empty());
        return;
    }
    
    // invariant: all index in _globals have the same RowCost
    // _globals.push_back(0);
    

    // index of row of best cost
    assert(globals.cbegin() != globals.cend());
    size_t ibest = *(globals.cbegin());
    
    // estimate the best tonality wrt costs (nb accidentals)
    for (auto it = globals.cbegin(); it != globals.cend(); ++it)

    {
        size_t i = *it;
        assert(i != TonIndex::FAILED);
        assert(i != TonIndex::UNDEF);
        assert(i < _index.size());
        //     for (size_t i = 1; ; ++i)
        // all elements of cands have same cost
        const Cost& bestCost = tab.rowCost(ibest);
        const Cost& rc = tab.rowCost(i);
                
        // new best ton
        if (rc < bestCost)
        {
            ibest = i;
        }
    }

    assert(ibest < _index.size());
    const Cost& bestCost = tab.rowCost(ibest);
    
    // add to _globals (candidates) all tonality at distance to ibest
    // smaller than d
    for (auto it = globals.cbegin(); it != globals.cend(); ++it)
    {
        size_t i = *it;
        assert(i != TonIndex::FAILED);
        assert(i != TonIndex::UNDEF);
        assert(i < _index.size());
        const Cost& rc = tab.rowCost(i);

        // real tie
        if (rc == bestCost)
        {
            _globals.push_back(i);
        }
        // approx tie
        // we keep this case apart from real tie because of floating approx.
        // (dist is double)
        else if ((d > 0) && (rc.dist(bestCost) <= d))
        {
            _globals.push_back(i);
        }
    }
    
    // at least ibest
    assert(! _globals.empty());
    assert(contains(ibest));
    assert(_globals[0] < _index.size());

    if (_debug)
        DEBUGU("PST: {} candidates global: ", _globals.size());
    
}


size_t PSO::size() const
{
    return _globals.size();
}


bool PSO::empty() const
{
    return _globals.empty();
}


bool PSO::contains(size_t n) const
{
    for (auto it = _globals.cbegin(); it != _globals.cend(); ++it)
    {
        size_t i = *it;
        assert(i != TonIndex::FAILED);
        assert(i != TonIndex::UNDEF);
        assert(i < _index.size());
        if (i == n)
            return true;
    }
    
    return false;
}

size_t PSO::iglobal(size_t i) const
{
    assert(i < _globals.size());
    return _globals.at(i);
}


const Ton& PSO::global(size_t i) const
{
    size_t ig = iglobal(i);
    assert(ig < _index.size());
    return _index.ton(ig);
}


std::vector<size_t>::const_iterator PSO::cbegin() const
{
    return _globals.cbegin();
}


std::vector<size_t>::const_iterator PSO::cend() const
{
    return _globals.cend();
}


bool PSO::member(size_t ig) const
{
    if (ig < _index.size())
    {
        for (size_t it : _globals)
        {
            if (it == ig)
                return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}


std::vector<bool> PSO::getMask() const
{
    std::vector<bool> m(_index.size(), false);
    for (size_t it : _globals)
    {
        assert(it < _index.size());
        m[it] = true;
    }
    return m;
}


void PSO::setGlobal(size_t ig)
{
    assert(ig < _index.size());
    _globals.push_back(ig);
}





} // namespace pse
