//
//  WeberModal.cpp
//  pse
//
//  Created by Florent Jacquemard on 27/02/2024.
//


#include "WeberModal.hpp"
#include "Weber_static.hpp"
#include "WeberModal_static.hpp"


namespace pse {


// static
const int WeberModal::WS_UNDEFDIST = Weber_static::UNDEF_DIST;


WeberModal::WeberModal():
DIST()
{
    
    for (size_t i = 0; i < WMS_NBTONS; ++i)
        DIST[i].fill(WS_UNDEFDIST);
    init();
}


WeberModal::~WeberModal()
{ }


int WeberModal::dist(const Ton& ton1, const Ton& ton2)
{
    size_t i1 = WeberModal_static::index(ton1);
    assert(i1 < WMS_NBTONS);
    size_t i2 = WeberModal_static::index(ton2);
    assert(i2 < WMS_NBTONS);
    return DIST[i1][i2];
}


// static
size_t WeberModal::extractMin(std::array<bool, WMS_NBTONS>& heap,
                         const std::array<int, WMS_NBTONS>& dist)
{
    size_t m = WMS_NBTONS; // undef
    for (size_t i = 0; i < WMS_NBTONS; ++i)
    {
        assert(m <= WMS_NBTONS);
        if (heap[i] && (m == WMS_NBTONS || Weber_static::inf(dist[i], dist[m])))
            m = i;
    }
    // extract
    if (m < WMS_NBTONS)
        heap[m] = false;
    
    return m;
}


bool WeberModal::isEmpty(const std::array<bool, WMS_NBTONS>& heap)
{
    for (size_t i = 0; i < WMS_NBTONS; ++i)
    {
        if (heap[i])
            return false;
    }
    return true;
}


void WeberModal::init()
{
    // row i
    for (size_t i = 0; i < WMS_NBTONS; i++)
    {
        //typedef std::priority_queue<size_t, std::vector<size_t>, decltype(comparinfi)> q;
        //q pqueue(comparinfi);
        std::array<bool, WMS_NBTONS> heap;
        heap.fill(true); // all in heap
        
        // null diagonal
        DIST[i][i] = 0;
        //for (size_t j = 0; j < WMS_NBTONS; ++j)
        //    pqueue.push(j); // std::make_pair(j, WS_UNDEFDIST));

        // complete the lower part of triangle (in column i)
        while (! isEmpty(heap))
        {
            //size_t k = pqueue.top();
            size_t k = extractMin(heap, DIST[i]);
            assert(k < WMS_NBTONS);
            int distik = DIST[i][k];
            assert(distik != WS_UNDEFDIST);
            
            // explore the neighbour tonalities of k
            for (size_t l = 0; l < 9; ++l)
            {
                size_t nl = neighbour_modal(k, l);
                assert(nl <= WMS_NBTONS);
                assert(nl == WMS_NBTONS ||
                       DIST[i][nl] == WS_UNDEFDIST ||
                       DIST[i][nl] <= distik + 1);
                if ((nl < WMS_NBTONS) &&
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


void WeberModal::dump() const
{
    for (size_t i = 0; i < WMS_NBTONS; ++i)
    {
        std::cout << "{ ";
        for (size_t j = 0; j < WMS_NBTONS; ++j)
        {
            assert(DIST[i][j] != WS_UNDEFDIST);
            if (DIST[i][j] < 10)
                std::cout << ' ';
            std::cout << DIST[i][j];
            if (j < WMS_NBTONS-1)
                std::cout << ", ";
        }
        std::cout << " }";
        if (i < WMS_NBTONS-1)
            std::cout << ", ";
        std::cout << std::endl;
    }
}


// static
size_t WeberModal::neighbour_modal(size_t i, size_t n)
{
    assert(n < 8);
    size_t FIRST_MIN_HARM = WMS_NBTONS/11;
    size_t FIRST_MIN_MEL = 2*WMS_NBTONS/11;
    size_t FIRST_DOR = 3*WMS_NBTONS/11;
    size_t FIRST_PHRY = 4*WMS_NBTONS/11;
    size_t FIRST_LYD = 5*WMS_NBTONS/11;
    size_t FIRST_MIXO = 6*WMS_NBTONS/11;
    size_t FIRST_AEOL = 7*WMS_NBTONS/11;
    size_t FIRST_LOC = 8*WMS_NBTONS/11;
    size_t FIRST_BLMAJ = 9*WMS_NBTONS/11;
    size_t FIRST_BLMIN = 10*WMS_NBTONS/11;
    
    long k = i; // signed
    assert(0 <= k);
    assert(k < WMS_NBTONS);
    
    // major tonality
    if (0 <= k && k < FIRST_MIN_HARM)
    {
        switch (n)
        {
                // up : next major after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIN_HARM)?(k+1):WMS_NBTONS);
                // left = parallel minor of k : "ton homonyme"
            case 1:
                return ((k-3 >= 0)?(k-3+FIRST_MIN_HARM):WMS_NBTONS);
                // right : relative minor of k
            case 2:
                assert(k+FIRST_MIN_HARM < FIRST_MIN_MEL);
                return (k+FIRST_MIN_HARM);
                // down : previous major before k in array of fifths
            case 3:
                return ((k-1 >= 0)?(k-1):WMS_NBTONS);
                // back : lydian mode with the same key signature
            case 4:
                return ((k-1 >= 0)?(k-1+FIRST_LYD):WMS_NBTONS);
                // front : mixolydian mode with the same key signature
            case 5:
                return ((k+1+FIRST_MIXO < FIRST_AEOL)?(k+1+FIRST_MIXO):WMS_NBTONS);
                // relative min_nat
            case 6:
                assert(k+FIRST_AEOL < WMS_NBTONS);
                return (k+FIRST_AEOL);
                // relative min_mel
            case 7:
                assert(k+FIRST_MIN_MEL < WMS_NBTONS);
                return (k+FIRST_MIN_MEL);
                // homonymous blues scale
            case 8:
                assert(k+FIRST_BLMAJ < WMS_NBTONS);
                return (k+FIRST_BLMAJ);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    // minor tonality
    else if (FIRST_MIN_HARM <= k && k < FIRST_MIN_MEL)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIN_MEL)?(k+1):WMS_NBTONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM);
                // right : next major after left in array of fifths
                //        = parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_MIN_MEL)?(k+3-FIRST_MIN_HARM):WMS_NBTONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN_HARM)?(k-1):WMS_NBTONS);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_MIN_HARM)?((k-1)+FIRST_DOR-FIRST_MIN_HARM):WMS_NBTONS);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_MIN_MEL)?(k+1+FIRST_PHRY-FIRST_MIN_HARM):WMS_NBTONS);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM+FIRST_MIN_MEL);
            case 7:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM+FIRST_AEOL);
                // homonymous blues scale
            case 8:
                assert(k+FIRST_BLMIN < WMS_NBTONS);
                return (k+FIRST_BLMIN);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_MIN_MEL <= k && k < FIRST_DOR)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < FIRST_DOR)?(k+1):WMS_NBTONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL);
                // right : next major after left in array of fifths
                //        = parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_DOR)?(k+3-FIRST_MIN_MEL):WMS_NBTONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN_MEL)?(k-1):WMS_NBTONS);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_MIN_MEL)?((k-1)+FIRST_DOR-FIRST_MIN_MEL):WMS_NBTONS);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_DOR)?(k+1+FIRST_PHRY-FIRST_MIN_MEL):WMS_NBTONS);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL+FIRST_MIN_HARM);
            case 7:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL+FIRST_AEOL);
                // homonymous blues scale
            case 8:
                assert(k+FIRST_BLMIN < WMS_NBTONS);
                return (k+FIRST_BLMIN);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_DOR <= k && k < FIRST_PHRY)
    {
        switch (n)
        {
                // up : next dorian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_PHRY)?(k+1):WMS_NBTONS);
                // left : relative lydian of k
            case 1:
                assert(k-FIRST_DOR >= 0);
                return (k-FIRST_DOR+FIRST_LYD);
                // right = parallel lydian of k
            case 2:
                return ((k+3 < FIRST_PHRY)?(k+3-FIRST_DOR+FIRST_LYD):WMS_NBTONS);
                // down : previous dorian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_DOR)?(k-1):WMS_NBTONS);
                // back : mixolydian mode with the same key sig
            case 4:
                return ((k+2 < FIRST_PHRY)?((k+2)-FIRST_DOR+FIRST_MIXO):WMS_NBTONS);
                // front : minor mode with the same key sig
            case 5:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_MIN_HARM):WMS_NBTONS);
                // the other minor modes have to be close too...
            case 6:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_MIN_MEL):WMS_NBTONS);
            case 7:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_AEOL):WMS_NBTONS);
                // homonymous blues scale
            case 8:
                assert(k+FIRST_BLMIN < WMS_NBTONS);
                return (k+FIRST_BLMIN);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_PHRY <= k && k < FIRST_LYD)
    {
        switch (n)
        {
                // up : next phrygian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_LYD)?(k+1):WMS_NBTONS);
                // left : relative mixolydian of k
            case 1:
                assert(k-FIRST_PHRY >= 0);
                return (k-FIRST_PHRY+FIRST_MIXO);
                // right = parallel mixolydian of k
            case 2:
                return ((k+3 < FIRST_LYD)?(k+3-FIRST_PHRY+FIRST_MIXO):WMS_NBTONS);
                // down : previous phrygian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_PHRY)?(k-1):WMS_NBTONS);
                // back : minor mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_MIN_HARM):WMS_NBTONS);
                // front : locrian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_LYD)?(k+1-FIRST_PHRY+FIRST_LOC):WMS_NBTONS);
                // the other minor modes have to be close too...
            case 6:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_MIN_MEL):WMS_NBTONS);
            case 7:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_AEOL):WMS_NBTONS);
            case 8:
                return WMS_NBTONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_LYD <= k && k < FIRST_MIXO)
    {
        switch (n)
        {
                // up : next lydian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIXO)?(k+1):WMS_NBTONS);
                // right : relative dorian of k
            case 1:
                return ((k-FIRST_LYD >= 0)?(k-FIRST_LYD+FIRST_DOR):WMS_NBTONS);
                // left : next dorian after right in array of fifths
                //        = parallel dorian of k
            case 2:
                return ((k-3>=FIRST_LYD)?(k-3-FIRST_LYD+FIRST_DOR):WMS_NBTONS);
                // down : previous lydian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_LYD)?(k-1):WMS_NBTONS);
                // front : Major tone with the same key signature
            case 4:
                return ((k+1 < FIRST_MIXO)?(k+1-FIRST_LYD):WMS_NBTONS);
            case 5:
                return WMS_NBTONS;
            case 6:
                return WMS_NBTONS;
            case 7:
                return WMS_NBTONS;
            case 8:
                return WMS_NBTONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_MIXO <= k && k < FIRST_AEOL)
    {
        switch (n)
        {
                // up : next lydian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_AEOL)?(k+1):WMS_NBTONS);
                // right : relative phrygian of k
            case 1:
                return ((k-FIRST_MIXO >= 0)?(k-FIRST_MIXO+FIRST_PHRY):WMS_NBTONS);
                // left = parallel phrygian of k
            case 2:
                return ((k-3>=FIRST_MIXO)?(k-3-FIRST_MIXO+FIRST_PHRY):WMS_NBTONS);
                // down : previous mixolydian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIXO)?(k-1):WMS_NBTONS);
                // front : dorian tone with the same key signature
            case 4:
                return ((k-2 >= FIRST_MIXO)?(k-2-FIRST_MIXO+FIRST_DOR):WMS_NBTONS);
                // back : major tone with the same key signature
            case 5:
                return ((k-1 >= FIRST_MIXO)?(k-1-FIRST_MIXO):WMS_NBTONS);
            case 6:
                return WMS_NBTONS;
            case 7:
                return WMS_NBTONS;
                //homonymous blues scale
            case 8:
                assert(k+FIRST_BLMAJ < WMS_NBTONS);
                return (k+FIRST_BLMAJ);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_AEOL <= k && k < FIRST_LOC)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < FIRST_LOC)?(k+1):WMS_NBTONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL);
                // right : next major after left in array of fifths
                //        = parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_LOC)?(k+3-FIRST_AEOL):WMS_NBTONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_AEOL)?(k-1):WMS_NBTONS);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_AEOL)?((k-1)+FIRST_DOR-FIRST_AEOL):WMS_NBTONS);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_LOC)?(k+1+FIRST_PHRY-FIRST_AEOL):WMS_NBTONS);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL+FIRST_MIN_HARM);
            case 7:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_AEOL+FIRST_MIN_MEL);
                //homonymous blues scale
            case 8:
                assert(k+FIRST_BLMIN < WMS_NBTONS);
                return (k+FIRST_BLMIN);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_LOC <= k && k < WMS_NBTONS)
    {
        switch (n)
        {
                // up : next locrian after k in array of fifths
            case 0:
                return ((k+1 < WMS_NBTONS)?(k+1):WMS_NBTONS);
                // left : "relative dorian" of k
            case 1:
                assert(k-3-FIRST_LOC >= 0);
                return (k-3-FIRST_LOC+FIRST_DOR);
                // right = parallel dorian of k
            case 2:
                return (k-FIRST_LOC+FIRST_DOR);
                // down : previous locrian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_LOC)?(k-1):WMS_NBTONS);
                // back : phrygian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_LOC)?((k-1)+FIRST_PHRY-FIRST_LOC):WMS_NBTONS);
            case 5:
                return WMS_NBTONS;
            case 6:
                return WMS_NBTONS;
            case 7:
                return WMS_NBTONS;
            case 8:
                return WMS_NBTONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else
    {
        ERROR("Weber neighbour: unexpected ton index {}", k);
        return WMS_NBTONS;
    }
}
     


} // end namespace pse





// struct orderi
// {
// constexpr bool operator()(const int& x, const int& y) const noexcept
// {
//         if (DIST[i][x] ==WS_UNDEFDIST)
//         {
//             return false;
//         }
//         else if (y.second != WS_UNDEFDIST)
//         {
//             return (x.second != WS_UNDEFDIST);
//         }
//         else
//         {
//             return x.second < y.second; // min
//         }
//     }
// };


// ordering function for priority queue (lambda)
// this (for DIST) and local variable i are captured
//        auto comparsupi = [this, i](size_t a, size_t b) // a > b
//        {
//            assert(a < WMS_NBTONS);
//            assert(b < WMS_NBTONS);
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
//            assert(a < WMS_NBTONS);
//            assert(b < WMS_NBTONS);
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
