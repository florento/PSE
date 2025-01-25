//
//  PSGridr.cpp
//  pse
//
//  Created by Florent Jacquemard on 22/01/2025.
//

#include "PSGridr.hpp"


namespace pse {

PSGr::PSGr(const PST& tab): // std::vector<bool> mask
PSGy(tab, false) // do not compute table (yet empty)
{
    // assert(mask.size() == _index.size());
    assert(_content.empty());
    init(tab);
}


PSGr::~PSGr()
{ }


// selection of best local by mean of 3 ranks, according to
// - costs
// - distance to previous local
// - distance to assumed global (current row)
void PSGr::init(const PST& tab)
{
    // for each bar (column)
    for (size_t j = 0; j < tab.size(); ++j)
    {
        // column of best paths for measure j
        const PSV& vec = tab.column(j);
        assert(vec.size() == _index.size());

        // set of index of elements in vec with a best cost.
        // (there are several in case of tie).
        std::vector<size_t> cands; // empty
        
        // add empty column to the grid
        assert(_content.size() == j); // current nb of columns
        _content.emplace_back();
        std::vector<size_t>& current = _content.back();

        // compute every row in this new column of grid
        for (size_t i = 0; i < vec.size(); ++i)
        {
            // i is masked (not global): do not estimate local for i
            if (!_index.isGlobal(i)) // (mask[i] == false)
            {
                current.push_back(TonIndex::UNDEF);
            }
            // in the first column (first measure)
            else if (_content.size() == 1)
            {
                // estimation locals by mean of ranks
                std::vector<size_t> ties; // empty
                extractRank(vec, ties, i, i); // global and prev
                current.push_back(breakTieRank(vec, ties, i, i));
            }
            // otherwise, consider previous column
            else
            {
                assert(j > 0);
                assert(j-1 < _content.size());
                assert(i < _content.at(j-1).size());
                size_t iprev = _content.at(j-1).at(i);
                // estimation locals by mean of ranks
                std::vector<size_t> ties; // empty
                extractRank(vec, ties, i, iprev); // global = i
                current.push_back(breakTieRank(vec, ties, i, iprev));
            }
            assert(current.back() == TonIndex::UNDEF ||
                   current.back() < _index.size());
        }
        assert(current.size() == vec.size());
    }
}


void PSGr::extractRank(const PSV& vec, std::vector<size_t>& ties,
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


size_t PSGr::findbyKS(int ks, const std::vector<size_t>& cands)
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
size_t PSGr::breakTieRank(const PSV& vec, const std::vector<size_t>& cands,
                      size_t ig, size_t iprev)
{
    // no candidates in case of empty bar: keep the previous local
    if (cands.empty())
        return iprev;  // TonIndex::FAILED; // TonIndex::UNDEF
    // no ties: return the single candidate
    else if (cands.size() == 1)
        return cands.front();
    
    assert(iprev != TonIndex::UNDEF);
    assert(iprev != TonIndex::FAILED);
    assert(iprev < _index.size());

    // const Ton& pton = _index.ton(iprev);
    // const Ton& gton = _index.ton(ig);
    
    // tie break
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
    size_t choice = breakTieBest(vec, cands_majmin, ig, iprev);
    // WARN_TIE(vec.bar(), cands_majmin, choice);
    return choice;
}


} // end namespace pse


/*
 
 // real function using ranks, used when the init flag is true
 /// #todo AV not used?
 size_t PSGr::estimateLocalRank(const PSV& vec, size_t ig, size_t iprev)
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
 
 */
