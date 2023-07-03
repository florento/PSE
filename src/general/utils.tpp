//
//  utils.cpp
//  pse
//
//  Created by Florent Jacquemard on 02/07/2023.
//


namespace pse {

namespace util {

template<typename E>
void ranks(std::vector<E>& v,
           std::function<bool (E, E)> equal,
           std::function<bool (E, E)> smaller,
           std::vector<size_t>& ranks)
{
    assert(ranks.empty());
    if (v.empty()) return;

    // list of ranks, initialy null vector.
    ranks.insert(ranks.begin(), v.size(), 0);
    
    // list pf pairs index / value
    std::vector<std::pair <size_t, E>> v1;
    for (size_t i = 0; i < v.size(); ++i)
    {
        v1.push_back(std::make_pair(i, v.at(i)));
    }
       
    // sort v1 according to the distance value
    std::sort(v1.begin(), v1.end(),
              [smaller](std::pair<size_t, E> x, std::pair<size_t, E> y)
              { return smaller(x.second, y.second); } );
    
    // v[0] has rank 0. we leave it to 0.
    assert(0 < v.size());
    size_t rank = 0;
    size_t ties = 1;
    E dprec = v1.at(0).second; // dist value of previous
    
    for (size_t i = 1; i < v1.size(); ++i)
    {
        size_t current = v1[i].first;
        assert(current < ranks.size());
        E d = v1[i].second;
        if (equal(d, dprec))
        {
            ranks[current] = rank;
            ties++;
        }
        else
        {
            assert(smaller(dprec, d));
            rank += ties;
            ties = 1;
            dprec = d;
            ranks[current] = rank;
        }
    }
    
    //return ranks;
}

} // namespace util

} // namespace pse
