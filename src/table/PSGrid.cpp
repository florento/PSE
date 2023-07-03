//
//  PSGrid.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/05/2023.
//

#include "PSGrid.hpp"
#include "PSTable.hpp"

namespace pse {


PSG::PSG(const PST& tab, std::vector<bool> mask):
_index(tab.index()),
_content()
{
    assert(mask.size() == _index.size());
    init(tab, mask, true); // true for using the
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


void PSG::init(const PST& tab, std::vector<bool> mask, bool flag)
{
    // for each bar (column)
    for (size_t j = 0; j < tab.columnNb(); ++j)
    {
        // column of best paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == mask.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        std::set<size_t> cands; // empty
        
        if (flag == false)
            extract_bests(vec, cands, 50);  // printf("%lu",cands.size());
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
                current.push_back(TonIndex::UNDEF);
            }
            // in the first column (first measure)
            else if (_content.size() == 1)
            {
                if (flag == true) // estimation locals by mean of ranks
                    current.push_back(estimateLocal(vec,i, i));
                else              // estimation locals by extractbests then distances
                    current.push_back(estimateLocal(i, i, cands));
            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                if (flag == true) // estimation locals by mean of ranks
                    current.push_back(estimateLocal(vec, i, iprev));
                else              // estimation locals by extractbests then distances
                    current.push_back(estimateLocal(i, iprev, cands));
            }
            assert(current.back() == TonIndex::UNDEF ||
                   current.back() < _index.size());
            //INFO("BUG 102: {} (UNDEF={})", _content.back().back(), TonIndex::UNDEF);
        }
        assert(current.size() == vec.size());
    }
}


// this function determines the best local tonalities
// according to the bags of the studied measure
// However it has an important flaw : no tonal context is taken into account
// to determine the local tones.
void PSG::extract_bests(const PSV& vec, std::set<size_t>& ties, double d)
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
            //ties.clear();
            //auto ret = ties.insert(j);
            //assert(ret.second == true); // j was inserted
        }
    }
    
    for (size_t j = 0; j < vec.size(); ++j)
    {
        const PSB& psb = vec.bag(j);
        
        // occurs iff first() == last(), and in this case all the bags are empty.
        if (psb.empty())
            break; // break
        
        const Cost& cost = psb.cost(); // shared_clone();
        // tie break
        if ((cbest != nullptr) && (cost.dist(*cbest) <= d))
        {
            //printf("%f \n",cost.dist(*cbest));
            auto ret = ties.insert(j);
            assert(ret.second == true); // j was inserted
        }
        // otherwise keep the current best
        else
        {
            assert((cbest == nullptr) || (cost > *cbest)); // we did not forget a case
        }
    }
    //printf("nombre de tonalités candidates : %lu",ties.size());
    //assert(! ties.empty());
}


// this function chooses among the best possible local tonalities
// (obtained with extract_bests)
// the closest one to the previous local tone and to the global tone.
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
    // PSCost cbest;  // WARNING: initialized to 0.
    
    // current best distance to previous global ton. ig
    unsigned int dgbest = 30; // initialized to avoid warning

    // current best distance to previous local ton. for ig
    unsigned int dlbest = 30; // initialized to avoid warning

    for (size_t j : cands)
    {
        //assert(_psbs[j]);
        //const PSB& psb = *(_psbs[j]);
        //assert(! psb.empty());
        // occurs iff first() == last(), and in this case all the bags are empty.
        //PSCost cost = psb.cost();
        
        const Ton& jton = _index.ton(j);
        //unsigned int dist = pton.distDiatonic(jton);
        unsigned int dist = pton.distWeber(jton);
        //unsigned int distg = gton.distDiatonic(jton);
        unsigned int distg = gton.distWeber(jton);

        // tie break criteria 1:
        // best distance (of current tonality j) to the previous local tonality for ig
        if (dist < dlbest)
        {
            ibest = j;
            dlbest = dist;
            dgbest = distg;
        }
        // tie break criteria 2:
        // best distance (of current tonality j) to the global tonality ig
        else if (dist == dlbest)
        {
            if (distg < dgbest)
            {
                ibest = j;
                dlbest = dist;
                dgbest = distg;
            }
            // tie break criteria 3:
            // smallest key signature
            // ALT: best distance between the previous local tonality and
            // a config in bag for the current tonality j
            else if (distg == dgbest)
            {
                const Ton& bton = _index.ton(ibest);
                size_t besty = std::abs(bton.fifths());
                if (std::abs(jton.fifths()) < std::abs(bton.fifths()))
                {
                    ibest = j;
                    dlbest = dist;
                    dgbest = distg;
                }
                // tie break fail
                else if ((std::abs(jton.fifths()) == besty) && (j != ibest))
                {
                    WARN("PSGrid, estimation local, tie break fail {} vs {} dist prev({})={}, dist global({})={})",
                         jton, _index.ton(ibest), pton, dist, gton, distg);
                }
            }
        }
    }
    assert(ibest != TonIndex::FAILED);
    assert(ibest == TonIndex::UNDEF || ibest < _index.size());
    assert(ibest < _index.size() || cands.empty());
    return ibest;
}


//______________________________________________________________________________//
/*


void PSG::init(const PST& tab, std::vector<bool> mask)
{

    for (size_t j = 0; j < tab.columnNb(); ++j)
    {
        // column of best paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == mask.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        //std::set<size_t> ties; // empty
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
                current.push_back(TonIndex::UNDEF);
            }
            // in the first column (first measure)
            else if (_content.size() == 1)
            {
                current.push_back(estimateLocalalt(vec, i, i, 4));
            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                current.push_back(estimateLocalalt(vec, i, iprev, 4));
            }
            assert(current.back() == TonIndex::UNDEF ||
                   current.back() < _index.size());
            //INFO("BUG 102: {} (UNDEF={})", _content.back().back(), TonIndex::UNDEF);
        }
        assert(current.size() == vec.size());
    }
}

*/


// this alternative function determines the best local tonality by restraining
// its search only on tones close to the previous or global one
// and then choosing the one minimizing accidents
size_t PSG::estimateLocalalt(const PSV& vec, size_t ig, size_t iprev,
                             unsigned int d)
{
    // no candidates in case of empty bar: keep the previous local
    //if (cands.empty())
    //    return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
    std::set<size_t> cands ;
    std::set<size_t> ties ;
    
    
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());
    
    const Ton& pton = _index.ton(iprev);
    const Ton& gton = _index.ton(ig);

    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.
    
    // current best distance to previous local ton. for ig
    //unsigned int dbest = 30; // initialized to avoid warning

    // current best distance to previous global ton. ig
    //unsigned int dgbest = 30; // initialized to avoid warning

    for (size_t j=0; j < _index.size(); ++j)
    {
        
        const Ton& jton = _index.ton(j);
        unsigned int dist = pton.distWeber(jton);
        unsigned int distg = gton.distWeber(jton);
        //éventuellement remplacer Weber par la simple distance du cycle des quintes
        if ((dist <= d) || (distg <= d))
        {
            cands.insert(j);
            //ibest = j;
            //continue;
        }
    }
    
    std::shared_ptr<Cost> cbest = nullptr;
    int cpt = 0;
    for (size_t j : cands)
    {
        const PSB& psb = vec.bag(j);
        
        // occurs iff first() == last(), and in this case all the bags are empty.
        if (psb.empty())
            break; // break
        
        const Cost& cost = psb.cost(); // shared_clone();
        
        // new best cost     // cbest = 0 when j = 0
        if ((cpt == 0) || ((cbest != nullptr) && (cost < *cbest)))
        {
            ibest = j;
            cbest = cost.shared_clone();
            cpt++;
            //ties.clear();
            //auto ret = ties.insert(j);
            //assert(ret.second == true); // j was inserted
        }
    }
    
    for (size_t j : cands)
    {
        const PSB& psb = vec.bag(j);
        
        // occurs iff first() == last(), and in this case all the bags are empty.
        if (psb.empty())
            break; // break
        
        const Cost& cost = psb.cost(); // shared_clone();
        // tie break
        if ((cbest != nullptr) && (cost == *cbest) && (j != ibest))
        {
            auto ret = ties.insert(j);
            assert(ret.second == true); // j was inserted
            const Ton& jton=_index.ton(j);
            unsigned int dist = pton.distWeber(jton);
            unsigned int distg = gton.distWeber(jton);
            WARN("PSGrid, estimation local ALT, tie break fail {} vs {} dist prev({})={}, dist global({})={})",
                 jton, _index.ton(ibest), pton, dist, gton, distg);
        }
        // otherwise keep the current best
        else
        {
            assert((cbest == nullptr) || (cost > *cbest)); // we did not forget a case
        }
    }
    assert(ibest != TonIndex::FAILED);
    assert(ibest == TonIndex::UNDEF || ibest < _index.size());
    assert(ibest < _index.size() || cands.empty());
    
    return ibest;
}


size_t PSG::estimateLocal(const PSV& vec, size_t ig, size_t iprev)
{
    // case of empty bar: keep the previous local
    if (vec.first() == vec.stop())
        return iprev;
    
    assert(vec.size() == _index.size());
    
    assert(ig != TonIndex::UNDEF);
    assert(ig != TonIndex::FAILED);
    assert(ig < _index.size());

    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());
            
    /// ranks for costs of bags
    std::vector<size_t> rank_bags; // empty
    vec.ranks(rank_bags);
    assert(rank_bags.size() == _index.size());

    /// ranks for distance to prev local.
    std::vector<size_t> rank_prev; // empty
    /// ranks for distance to global.
    std::vector<size_t> rank_glob; // empty

    for (size_t j = 0; j < _index.size(); ++j)
    {
        rank_prev.push_back(_index.rankWeber(iprev, j));
        rank_glob.push_back(_index.rankWeber(ig, j));
    }

    /// list of means of the ranks in the 3 lists (unweighted)
    std::vector<double> means; // empty
    for (size_t j = 0; j < _index.size(); ++j)
    {
        means.push_back(((double) rank_bags.at(j) +
                             (double) rank_prev.at(j) +
                             (double) rank_glob.at(j)) / 3);
    }
    
    std::vector<size_t> rank_mean; // empty

    /// ranks of the means
    /// we could also simply sort the list of means
    util::ranks<double>(means,
             [](double a, double b) { return (a == b); },
             [](double a, double b) { return (a <  b); }, rank_mean);
    assert(rank_mean.size() == _index.size());
    bool found1 = false;

    /// index of tons with best rank
    std::set<size_t> ties; // empty
    for (size_t j = 0; j < rank_mean.size(); ++j)
    {
        if (rank_mean.at(j) == 0)
        {
            ties.insert(j);
        }
        if (rank_mean.at(j) == 1)
        {
            found1 = true;
        }
    }
    assert(!ties.empty());
    assert((ties.size() == 1) || (found1 == false));
    if (ties.size() == 1)
    {
        return *(ties.begin());
    }
    else
    {
        WARN("estimateLocal: ties bar {}", vec.bar());
        return estimateLocal(ig, iprev, ties);
    }
}

} // end namespace pse
