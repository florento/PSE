//
//  PSGlobal.cpp
//  pse
//
//  Created by Florent Jacquemard on 25/05/2023.
//

#include "PSGlobal.hpp"


namespace pse {


PSO::PSO(const PST& tab, double d, bool dflag):
_index(tab.index()),
_globals(), // empty
_debug(dflag)
{
    assert(d > 0);
    init(tab, d);
}


PSO::PSO(const TonIndex& id, double dflag):
_index(id),
_globals(), // empty
_debug(dflag)
{ }


PSO::~PSO()
{ }


void PSO::init(const PST& tab, double d)
{
    // candidate list for best global
    // _globals is empty
    // invariant: all index in _globals have the same RowCost
    _globals.push_back(0);
    
    // estimate the best tonality wrt costs (nb accidentals)
    for (size_t i = 1; i < _index.size(); ++i)
    {
        assert(! _globals.empty());
        assert(_globals[0] < _index.size());
        // all elements of cands have same cost
        const Cost& bestCost = tab.rowCost(_globals[0]);
        const Cost& rc = tab.rowCost(i);

        // tie
        if (rc == bestCost)
        {
            _globals.push_back(i);
        }
        // approx tie
        else if ((d > 0) && (rc.dist(bestCost) <= d))
        {
            _globals.push_back(i);
        }
        // new best ton
        else if (rc < bestCost)
        {
            // rc far under bestCost
            assert((d == 0) || (rc.dist(bestCost) > d));
            _globals.clear();
            // push front
            _globals.insert(_globals.begin(), i);
            //_globals.push_back(i);
        }
        // otherwise do nothing
        else
        {
            assert(rc > bestCost);
        }
    }
    assert(! _globals.empty());

    if (_debug)
        INFO("PST: {} candidates global: ", _globals.size());
}


size_t PSO::size() const
{
    return _globals.size();
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
