//
//  PSGrid.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/05/2023.
//

#include "PSGrid.hpp"

namespace pse {


PSG::PSG(const PST& tab, std::vector<bool> mask):
_index(tab.index()),
_content()
{
    assert(mask.size() == _index.size());
    init(tab, mask);
}


PSG::PSG(const PST& tab):
PSG(tab, std::vector<bool>(tab.index().size(), true))
{ }


PSG::~PSG()
{ }


size_t PSG::size() const
{
    return _content.size();
}


size_t PSG::ilocal(size_t i, size_t j) const
{
    assert(j < _content.size());
    const std::vector<size_t>& col = _content.at(j);
    assert(i < col.size());
    return col.at(i);
}


const Ton& PSG::local(size_t i, size_t j) const
{
    size_t it = ilocal(i, j);
    assert(it < _index.size());
    return _index.ton(i);
}


const std::vector<size_t>& PSG::column(size_t j) const
{
    assert(j < _content.size());
    const std::vector<size_t>& col = _content.at(j);
    assert(col.size() == _index.size());
    return col;
}


void PSG::init(const PST& tab, std::vector<bool> mask)
{

    for (size_t j = 0; j < tab.columnNb(); ++j)
    {
        // column of best paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == mask.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        std::set<size_t> cands; // empty
        extract_bests(vec, cands);
        // cands empty in case of empty measure
        
        // add empty column
        assert(_content.size() == j); // current nb of columns
        _content.emplace_back();
        std::vector<size_t>& current = _content.back();

        // compute every row in this new column
        for (size_t i = 0; i < vec.size(); ++i)
        {
            // i is masked : do not estimate local for i
            if (mask[i] == false)
            {
                _content.back().push_back(TonIndex::UNDEF);
            }
            // in the first column (first measure)
            else if (_content.size() == 1)
            {
                _content.back().push_back(estimateLocal(i, i, cands));

            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                _content.back().push_back(estimateLocal(i, iprev, cands));
            }
            assert(_content.back().back() == TonIndex::UNDEF ||
                   _content.back().back() < _index.size());
            //INFO("BUG 102: {} (UNDEF={})", _content.back().back(), TonIndex::UNDEF);
        }
        assert(_content.back().size() == vec.size());
    }
}


void PSG::extract_bests(const PSV& vec, std::set<size_t>& ties)
{
    assert(ties.empty());
    
    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.

    // cost for the current best local tonality.
    std::shared_ptr<Cost> cbest = nullptr;     // WARNING: initialized to 0.

    for (size_t j = 0; j < vec.size(); ++j)
    {
        const PSB& psb = vec.bag(j);

        // occurs iff first() == last(), and in this case all the bags are empty.
        if (psb.empty())
            break; // break
        
        const Cost& cost = psb.cost(); // shared_clone();
        
        // new best cost     // cbest = 0 when j = 0
        if ((j == 0) || ((cbest != nullptr) && (cost < *cbest)))
        {
            ibest = j;
            cbest = cost.shared_clone();
            ties.clear();
            auto ret = ties.insert(j);
            assert(ret.second == true); // j was inserted
        }
        // tie break
        else if ((cbest != nullptr) && (cost == *cbest))
        {
            auto ret = ties.insert(j);
            assert(ret.second == true); // j was inserted
        }
        // otherwisse keep the current best
        else
        {
            assert((cbest == nullptr) || (cost > *cbest)); // we did not forget a case
        }
    }
    //assert(! ties.empty());
}


//to do later : reorganize the loop so that warnings only occur when a real tie break fail happens
size_t PSG::estimateLocal(size_t ig, size_t iprev, std::set<size_t>& cands)
{
    // no candidates in case of empty bar: keep the previous local
    if (cands.empty())
        return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
    
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());
    
    const Ton& pton = _index.ton(iprev);
    const Ton& gton = _index.ton(ig);

    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.

    // cost for the current best local tonality.
    PSCost cbest;  // WARNING: initialized to 0.
    
    // current best distance to previous local ton. for ig
    unsigned int dbest = -1; // initialized to avoid warning

    // current best distance to previous global ton. ig
    unsigned int dgbest = -1; // initialized to avoid warning

    for (size_t j : cands)
    {
        //assert(_psbs[j]);
        //const PSB& psb = *(_psbs[j]);
        //assert(! psb.empty());
        // occurs iff first() == last(), and in this case all the bags are empty.
        //PSCost cost = psb.cost();

        const Ton& jton = _index.ton(j);

        // tie break criteria 1:
        // best distance (of current tonality j) to the previous local tonality for ig
        //unsigned int dist = pton.distDiatonic(jton);
        unsigned int dist = pton.distWeber(jton);

        if (dist < dbest)
        {
            ibest = j;
            dbest = dist;
            continue;
        }
        // tie break criteria 2:
        // best distance (of current tonality j) to the global tonality ig
        else if (dist == dbest)
        {
            //unsigned int distg = gton.distDiatonic(jton);
            unsigned int distg = gton.distWeber(jton);
            if (distg < dgbest)
            {
                ibest = j;
                dgbest = distg;
            }
            // tie break criteria 3:
            // smallest key signature
            // ALT: best distance between the previous local tonality and
            // a config in bag for the current tonality j
            else if (distg == dgbest)
            {
                const Ton& bton = _index.ton(ibest);
                if (std::abs(jton.fifths()) < std::abs(bton.fifths()))
                {
                    ibest = j;
                }
                else if (std::abs(jton.fifths()) == std::abs(bton.fifths()))
                {
                    WARN("PSGrid, estimation local, tie break fail {} vs {} dist prev({})={}, dist global({})={})",
                         jton, _index.ton(ibest), pton, dist, gton, distg);
                }
                // otherwise keep the current best
                else
                {
                    assert(std::abs(jton.fifths()) > std::abs(bton.fifths()));
                }
            }
            // otherwise keep the current best
            else
            {
                assert(distg > dgbest); // we did not forget a case
            }
        }
        // otherwisse keep the current best
        else
        {
            assert(dist > dbest); // we did not forget a case
        }
    }
        
    assert(ibest != TonIndex::FAILED);
    assert(ibest == TonIndex::UNDEF || ibest < _index.size());
    assert(ibest < _index.size() || cands.empty());
    return ibest;
}


} // end namespace pse
