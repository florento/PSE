//
//  PSGrid.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/05/2023.
//

#include <sstream>      // std::stringbuf
#include <string>

#include "PSGrid.hpp"
#include "PSTable.hpp"


namespace pse {

PSG::PSG(const PST& tab, std::vector<bool> mask):
_index(tab.index()),
_content()
{
    assert(mask.size() == _index.size());
    init(tab, mask, true); // true for using the ranks
}


// not used
PSG::PSG(const PST& tab):
PSG(tab, std::vector<bool>(tab.index().size(), true))
{ }


PSG::~PSG()
{ }


size_t PSG::size() const
{
    return _content.size();
}


// accessor
size_t PSG::ilocal(size_t i, size_t j) const
{
    assert(j < _content.size());
    const std::vector<size_t>& col = _content.at(j);
    assert(i < col.size());
    return col.at(i);
}


// accessor
const Ton& PSG::local(size_t i, size_t j) const
{
    size_t it = ilocal(i, j);
    assert(it < _index.size());
    return _index.ton(i);
}


// accessor
const std::vector<size_t>& PSG::column(size_t j) const
{
    assert(j < _content.size());
    const std::vector<size_t>& col = _content.at(j);
    assert(col.size() == _index.size());
    return col;
}


// 2 modes of selection of best local, according to flag :
// - true for selection with rank
// - false for selection lexico
void PSG::init(const PST& tab, std::vector<bool> mask, bool flag)
{
    // for each bar (column)
    for (size_t j = 0; j < tab.size(); ++j)
    {
        // column of best paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == _index.size());
        assert(vec.size() == mask.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        std::vector<size_t> cands; // empty
        
        if (flag == false)
            extractBests(vec, cands, 50);  // printf("%lu",cands.size());
        // cands empty in case of empty measure
        
        // add empty column to the grid
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
                // estimation locals by extractbests then distances
                if (flag == false)
                {
                    current.push_back(breakTieBests(vec, cands, i, i));
                }
                // estimation locals by mean of ranks
                else
                {
                    std::vector<size_t> ties; // empty
                    extractRank(vec, ties, i, i);
                    current.push_back(breakTieRank(vec, ties, i, i));
                }
            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                // estimation locals by extractbests then distances
                if (flag == false)
                {
                    current.push_back(breakTieBests(vec, cands, i, iprev));
                }
                // estimation locals by mean of ranks
                else
                {
                    std::vector<size_t> ties; // empty
                    extractRank(vec, ties, i, iprev);
                    current.push_back(breakTieRank(vec, ties, i, iprev));
                }
            }
            assert(current.back() == TonIndex::UNDEF ||
                   current.back() < _index.size());
        }
        assert(current.size() == vec.size());
    }
}


// this function determines the best local tonalities
// according to the bags of the studied measure
// However it has an important flaw : no tonal context is taken into account
// to determine the local tones.
void PSG::extractBests(const PSV& vec, std::vector<size_t>& ties, double d)
{
    assert(ties.empty());
    vec.bests(ties, d);
}


void PSG::extractRank(const PSV& vec, std::vector<size_t>& ties,
                      size_t ig, size_t iprev)
{
    // case of empty bar: keep the previous local
    if (vec.first() == vec.stop())
    {
        ties.push_back(iprev); // singleton
        return;
    }
    
    // coefficients for the compution of means of ranks.
    // 0. rank in column of spelling table.
    // 1. rank for distance to previous in row.
    // 2. rank for distance to global in row.
    const std::array<size_t, 3> COEFF{1, 1, 1};
    
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
    std::vector<size_t> means; // empty
    for (size_t j = 0; j < _index.size(); ++j)
    {
        means.push_back(COEFF[0] * rank_bags.at(j) +
                        COEFF[1] * rank_prev.at(j) +
                        COEFF[2] * rank_glob.at(j));
    }
    
    std::vector<size_t> rank_mean; // empty

    /// ranks of the means
    /// we could also simply sort the list of means
    util::ranks<size_t>(means,
             [](size_t a, size_t b) { return (a == b); },
             [](size_t a, size_t b) { return (a <  b); }, rank_mean);
    assert(rank_mean.size() == _index.size());

    /// extract indices of tons with best mean rank (there can be ties)
    // std::vector<size_t> ties; // empty
    bool found1 = false; // only for debugging
    // size_t ibest = 0;
    for (size_t j = 0; j < rank_mean.size(); ++j)
    {
        if (rank_mean.at(j) == 0)
        {
            ties.push_back(j);
            // ibest=j;
        }
        else if (rank_mean.at(j) == 1)
        {
            found1 = true;
        }
    }
    assert(!ties.empty());
    assert((ties.size() == 1) || (found1 == false));
    
//    if (ties.size() == 1)
//    {
//        return ties.front();
//    }
//    else
//    {
//        WARN_TIE(vec.bar(), ties, ibest);
//        return ibest;
//        //return estimateLocal(ig, iprev, ties);
//        //renvoie de moins bons résultats quand décommentée, bizarre
//    }
}


// this function chooses among the best possible local tonalities
// (obtained with extract_bests)
// the closest one to the previous local tone and to the global tone.
size_t PSG::breakTieBests(const PSV& vec, const std::vector<size_t>& cands,
                      size_t ig, size_t iprev)
{
    // no candidates in case of empty bar: keep the previous local
    if (cands.empty())
        return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
 
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());

    // const Ton& pton = _index.ton(iprev);
    // const Ton& gton = _index.ton(ig);

    // index of the current best local tonality.
    size_t ibest = TonIndex::UNDEF; // out of range. initialized to avoid warning.

    // cost for the current best local tonality.
    std::shared_ptr<Cost> cbest = nullptr;     // WARNING: initialized to 0.
    
    // rank for distance to previous local ton. iprev for current best
    size_t bestrankp = 999; // initialized to avoid warning

    // rank for distance to global ton. ig current best
    size_t bestrankg = 999; // initialized to avoid warning

    // absolute number of fifth of current best
    int bestfifth = 999; // initialized to avoid warning

    for (size_t j : cands)
    {
        const PSB& psb = vec.bag(j);
        // occurs iff first() == last(), and in this case all the bags are empty.
        if (psb.empty()) break; // break
        const Cost& cost = psb.cost(); // shared_clone();
        const Ton& jton = _index.ton(j);
        size_t rankp = _index.rankWeber(iprev, j);  // dist = pton.distWeber(jton);
        size_t rankg = _index.rankWeber(ig, j); // gton.distWeber(jton);
        int fifths = std::abs(jton.fifths());
        
        if ((cbest == nullptr) || (cost < *cbest))
        {
            ibest = j;
            cbest = cost.shared_clone();
            bestrankp = rankp;
            bestrankg = rankg;
            bestfifth = fifths;
        }
        else if (cost == *cbest)
        {
            // tie break criteria 1:
            // best distance (of current tonality j)
            // to the previous local tonality for ig
            if (rankp < bestrankp)
            {
                ibest = j; // new best
                bestrankp = rankp;
                bestrankg = rankg;
                bestfifth = fifths;
            }
            // tie break criteria 2:
            // best distance (of current tonality j) to the global tonality ig
            else if (rankp == bestrankp)
            {
                if (rankg < bestrankg)
                {
                    ibest = j; // new best
                    bestrankg = rankg;
                    bestfifth = fifths;
                }
                // tie break criteria 3:
                // smallest key signature
                // ALT: best distance between the previous local tonality and
                // a config in bag for the current tonality j
                else if (rankg == bestrankg)
                {
                    const Ton& bton = _index.ton(ibest);
                    if (fifths < bestfifth)
                    {
                        ibest = j; // new best
                        bestfifth = fifths;
                    }
                    // tie break fail
                    else if ((fifths == bestfifth) && (j != ibest))
                    {
                        WARN("PSGrid: tie break1 fail {}:{} vs {}:{}",
                             j, jton, ibest, bton);
                        // WARN_TIE(vec.bar(), cands, ibest);
                        // keep the smallest ibest
                    }
                }
            }
        }
    }
    assert(ibest != TonIndex::FAILED);
    assert(ibest == TonIndex::UNDEF || ibest < _index.size());
    assert(ibest < _index.size() || cands.empty());
    return ibest;
}


size_t PSG::findbyKS(int ks, const std::vector<size_t>& cands)
{
    assert(-7 <= ks);
    assert(ks <= 7);
    for (size_t j : cands)
    {
        assert(j != TonIndex::UNDEF);
        const Ton& jton = _index.ton(j);
        if (jton.fifths() == ks)
            return j;
    }
    
    return TonIndex::UNDEF; // not found
}


// this function filters out the set of candidates local tonality.
size_t PSG::breakTieRank(const PSV& vec, const std::vector<size_t>& cands,
                      size_t ig, size_t iprev)
{
    // no candidates in case of empty bar: keep the previous local
    if (cands.empty())
        return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
    else if (cands.size() == 1)
        return cands.front();
    
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());

    // const Ton& pton = _index.ton(iprev);
    // const Ton& gton = _index.ton(ig);
    
    // first pass to extract the sublist of candidates not with church mode.
    std::vector<size_t> cands_majmin; // empty
    for (size_t j : cands)
    {
        //assert(_psbs[j]);
        //const PSB& psb = *(_psbs[j]);
        //assert(! psb.empty());
        // occurs iff first() == last(),
        // and in this case all the bags are empty.
        //PSCost cost = psb.cost();
        assert(j != TonIndex::UNDEF);
        const Ton& jton = _index.ton(j);
        // for ionan, we assume that major is also represented
        if ((jton.getMode() == ModeName::Major) ||
            (jton.getMode() == ModeName::Minor))
            cands_majmin.push_back(j);
    }
    
    // second pass to extract the sublist of candidates with church mode
    // whose KS is not in represented in major/minor modes.
    std::vector<size_t> cands_church; // empty
    for (size_t j : cands)
    {
        assert(j != TonIndex::UNDEF);
        const Ton& jton = _index.ton(j);
        if (((jton.getMode() == ModeName::Ionian) ||
             (jton.getMode() == ModeName::Dorian) ||
             (jton.getMode() == ModeName::Phrygian) ||
             (jton.getMode() == ModeName::Lydian) ||
             (jton.getMode() == ModeName::Mixolydian) ||
             (jton.getMode() == ModeName::Aeolian) ||
             (jton.getMode() == ModeName::Locrian) ||
             (jton.getMode() == ModeName::MinorNat)) &&
            (findbyKS(jton.fifths(), cands_majmin) == TonIndex::UNDEF))
        {
            cands_church.push_back(j);
        }
        // min mel if minor is not present (with same KS)
        else if ((jton.getMode() == ModeName::MinorMel) &&
                 (findbyKS(jton.fifths(), cands_majmin) == TonIndex::UNDEF))
        {
            cands_church.push_back(j);
        }
    }
    // concatenate cands_church at the end of cands_majmin
    cands_majmin.insert(cands_majmin.end(),
                        cands_church.begin(), cands_church.end() );
    

    if (cands_majmin.empty()) // should not happen
    {
        ERROR("PSG breakTie2: filtering error");
        return iprev;
    }
    else if (cands_majmin.size() == 1)
    {
        return cands_majmin.front();
    }
    
    // still a tie.
    size_t choice = breakTieBests(vec, cands_majmin, ig, iprev); // cands_majmin.front();
    // WARN_TIE(vec.bar(), cands_majmin, choice);
    return choice;
}


void PSG::WARN_TIE(size_t barnb, const std::vector<size_t>& ties,
                   size_t ibest) const
{
    std::stringstream buffer;
    for (size_t i : ties)
        buffer << std::to_string(i) << ": "<< _index.ton(i) << "  ";
    if (ibest != TonIndex::UNDEF)
    {
        DEBUGU("estimateLocal: {} ties bar {} (selected={}): {}",
             ties.size(), barnb, ibest, buffer.str());
    }
    else
    {
        DEBUGU("estimateLocal: {} ties bar {}: {}",
             ties.size(), barnb, buffer.str());
    }
}


// this alternative function determines the best local tonality by restraining
// its search only on tones close to the previous or global one
// and then choosing the one minimizing accidents
size_t PSG::estimateLocalLexico(const PSV& vec, size_t ig, size_t iprev,
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
        // éventuellement remplacer Weber par la simple distance du cycle des quintes
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


//real function using ranks, used when the init flag is true
size_t PSG::estimateLocalRank(const PSV& vec, size_t ig, size_t iprev)
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
    std::vector<size_t> means; // empty
    for (size_t j = 0; j < _index.size(); ++j)
    {
        means.push_back(3*rank_bags.at(j) +
                        3*rank_prev.at(j) +
                        2*rank_glob.at(j));
    }
    
    std::vector<size_t> rank_mean; // empty

    /// ranks of the means
    /// we could also simply sort the list of means
    util::ranks<size_t>(means,
             [](size_t a, size_t b) { return (a == b); },
             [](size_t a, size_t b) { return (a <  b); }, rank_mean);
    assert(rank_mean.size() == _index.size());
    bool found1 = false;
    size_t ibest = 0;
    /// index of tons with best rank
    std::vector<size_t> ties; // empty
    for (size_t j = 0; j < rank_mean.size(); ++j)
    {
        if (rank_mean.at(j) == 0)
        {
            ties.push_back(j);
            ibest=j;
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
        return ties.front();
    }
    else
    {
        WARN("estimateLocal: ties bar {}", vec.bar());
        return ibest;
        //return estimateLocal(ig, iprev, ties);
    }
}

} // end namespace pse







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
        //std::vector<size_t> ties; // empty
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
