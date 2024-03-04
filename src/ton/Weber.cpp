//
//  Weber.cpp
//  pse
//
//  Created by Florent Jacquemard on 17/12/2022.
//

#include "Weber.hpp"
#include "Weber_static.hpp"


namespace pse {

// static 
const int Weber::WS_UNDEFDIST = Weber_static::UNDEF_DIST;


Weber::Weber():
DIST()
{
    for (size_t i = 0; i < WS_NBTONS; ++i)
        DIST[i].fill(WS_UNDEFDIST);
    init();
}


Weber::~Weber()
{ }


int Weber::dist(const Ton& ton1, const Ton& ton2)
{
    size_t i1 = Weber_static::index(ton1);
    assert(i1 < WS_NBTONS);
    size_t i2 = Weber_static::index(ton2);
    assert(i2 < WS_NBTONS);
    return DIST[i1][i2];
}


// static
size_t Weber::extractMin(std::array<bool, WS_NBTONS>& heap,
                         const std::array<int, WS_NBTONS>& dist)
{
    size_t m = WS_NBTONS; // undef
    for (size_t i = 0; i < WS_NBTONS; ++i)
    {
        assert(m <= WS_NBTONS);
        if (heap[i] && (m == WS_NBTONS || Weber_static::inf(dist[i], dist[m])))
            m = i;
    }
    // extract
    if (m < WS_NBTONS)
        heap[m] = false;
    
    return m;
}


bool Weber::isEmpty(const std::array<bool, WS_NBTONS>& heap)
{
    for (size_t i = 0; i < WS_NBTONS; ++i)
    {
        if (heap[i])
            return false;
    }
    return true;
}


void Weber::init()
{
    // row i
    for (size_t i = 0; i < WS_NBTONS; i++)
    {
        //typedef std::priority_queue<size_t, std::vector<size_t>, decltype(comparinfi)> q;
        //q pqueue(comparinfi);
        std::array<bool, WS_NBTONS> heap;
        heap.fill(true); // all in heap
        
        // null diagonal
        DIST[i][i] = 0;
        //for (size_t j = 0; j < WS_NBTONS; ++j)
        //    pqueue.push(j); // std::make_pair(j, WS_UNDEFDIST));

        // complete the lower part of triangle (in column i)
        while (! isEmpty(heap))
        {
            //size_t k = pqueue.top();
            size_t k = extractMin(heap, DIST[i]);
            assert(k < WS_NBTONS);
            int distik = DIST[i][k];
            assert(distik != WS_UNDEFDIST);
            
            // explore the neighbour tonalities of k
            for (size_t l = 0; l < 4; ++l)
            {
                size_t nl = neighbour_tonal(k, l);
                assert(nl <= WS_NBTONS);
                assert(nl == WS_NBTONS ||
                       DIST[i][nl] == WS_UNDEFDIST ||
                       DIST[i][nl] <= distik + 1);
                if ((nl < WS_NBTONS) &&
                    (DIST[i][nl] == WS_UNDEFDIST || (distik + 1) < DIST[i][nl]))
                {
                    DIST[i][nl] = distik + 1;
                    //pqueue.push(std::make_pair(nl, distik + 1));
                }
            }
            //pqueue.pop();
        }
    }
}


void Weber::dump() const
{
    for (size_t i = 0; i < WS_NBTONS; ++i)
    {
        std::cout << "{ ";
        for (size_t j = 0; j < WS_NBTONS; ++j)
        {
            assert(DIST[i][j] != WS_UNDEFDIST);
            if (DIST[i][j] < 10)
                std::cout << ' ';
            std::cout << DIST[i][j];
            if (j < WS_NBTONS-1)
                std::cout << ", ";
        }
        std::cout << " }";
        if (i < WS_NBTONS-1)
            std::cout << ", ";
        std::cout << std::endl;
    }
}


// static
size_t Weber::neighbour_tonal(size_t i, size_t n)
{
    assert(n < 4);
    size_t FIRST_MIN = WS_NBTONS/2;
    
    long k = i; // signed
    assert(0 <= k);
    assert(k < WS_NBTONS);
    
    // major tonality
    if (0 <= k && k < FIRST_MIN)
    {
        switch (n)
        {
                // up : next major after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIN)?(k+1):WS_NBTONS);
                // left : next minor after right in array of fifths
                //        = parallel minor of k
            case 1:
                return ((k-3 >= 0)?(k-3+FIRST_MIN):WS_NBTONS);
                // right : relative minor of k
            case 2:
                assert(k+FIRST_MIN < WS_NBTONS);
                return (k+FIRST_MIN);
                // down : previous major before k in array of fifths
            case 3:
                return ((k-1 >= 0)?(k-1):WS_NBTONS);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WS_NBTONS;
        }
    }
    // minor tonality
    else if (FIRST_MIN <= k && k < WS_NBTONS)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < WS_NBTONS)?(k+1):WS_NBTONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN >= 0);
                return (k-FIRST_MIN);
                // right : next major after left in array of fifths
                //        = parallel major of k
            case 2:
                return ((k+3 < WS_NBTONS)?(k+3-FIRST_MIN):WS_NBTONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN)?(k-1):WS_NBTONS);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WS_NBTONS;
        }
    }
    else
    {
        ERROR("Weber neighbour: unexpected ton index {}", k);
        return WS_NBTONS;
    }
}



} // end namespace pse


//        struct orderi
//        {
//            constexpr bool operator()(const int& x, const int& y) const noexcept
//            {
//                if (DIST[i][x] == WS_UNDEFDIST)
//                {
//                    return false;
//                }
//                else if (y.second != WS_UNDEFDIST)
//                {
//                    return (x.second != WS_UNDEFDIST);
//                }
//                else
//                {
//                    return x.second < y.second; // min
//                }
//            }
//        };


// ordering function for priority queue (lambda)
// this (for DIST) and local variable i are captured
//        auto comparsupi = [this, i](size_t a, size_t b) // a > b
//        {
//            assert(a < WS_NBTONS);
//            assert(b < WS_NBTONS);
//            if (DIST[i][a] == WS_UNDEFDIST)
//            {
//                return (DIST[i][b] != WS_UNDEFDIST);
//            }
//            else if (DIST[i][b] == WS_UNDEFDIST)
//            {
//                return false;
//            }
//            else
//            {
//                assert(DIST[i][a] >= 0);
//                assert(DIST[i][b] >= 0);
//                return DIST[i][a] > DIST[i][b]; // min
//            }
//        };
//
//        auto comparinfi = [this, i](size_t a, size_t b) // a < b
//        {
//            assert(a < WS_NBTONS);
//            assert(b < WS_NBTONS);
//            if (DIST[i][a] == WS_UNDEFDIST)
//            {
//                return false;
//            }
//            else if (DIST[i][b] == WS_UNDEFDIST)
//            {
//                return true;
//            }
//            else
//            {
//                assert(DIST[i][a] >= 0);
//                assert(DIST[i][b] >= 0);
//                return DIST[i][a] < DIST[i][b]; // min
//            }
//        };
        
// copy the lower part of triangle (in column i)
// NO! the relation is not symetric!
//for (size_t j = 0; j < i; ++j)
//{
//    assert(DIST[j][i] != WS_UNDEFDIST);
//    DIST[i][j] = DIST[j][i];
//}
