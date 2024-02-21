//
//  WeberModal.cpp
//  pse
//
//  Created by Florent Jacquemard on 17/12/2022.
//

#include "WeberModal.hpp"

namespace pse {


const int WeberModal::UNDEF_DIST = -1;


WeberModal::WeberModal():
DIST()
{
    for (size_t i = 0; i < NB_TONS; ++i)
        DIST[i].fill(UNDEF_DIST);
    init();
}


WeberModal::~WeberModal()
{ }


int WeberModal::dist(const Ton& ton1, const Ton& ton2)
{
    size_t i1 = index(ton1);
    assert(i1 < NB_TONS);
    size_t i2 = index(ton2);
    assert(i2 < NB_TONS);
    return DIST[i1][i2];
}


// static
int WeberModal::static_dist(const Ton& ton1, const Ton& ton2)
{
    size_t i1 = index(ton1);
    assert(i1 < NB_TONS);
    size_t i2 = index(ton2);
    assert(i2 < NB_TONS);
    return WEBER_DIST[i1][i2];
}


// static
bool WeberModal::inf(int a, int b)
{
    assert(a == UNDEF_DIST || a >= 0);
    assert(b == UNDEF_DIST || b >= 0);
    if (a == UNDEF_DIST)
    {
        return false;
    }
    else if (b == UNDEF_DIST)
    {
        return true;
    }
    else
    {
        return a < b;
    }
};


// static
size_t WeberModal::extractMin(std::array<bool, NB_TONS>& heap,
                         const std::array<int, NB_TONS>& dist)
{
    size_t m = NB_TONS; // undef
    for (size_t i = 0; i < NB_TONS; ++i)
    {
        assert(m <= NB_TONS);
        if (heap[i] && (m == NB_TONS || inf(dist[i], dist[m])))
            m = i;
    }
    // extract
    if (m < NB_TONS)
        heap[m] = false;
    
    return m;
}


bool WeberModal::isEmpty(const std::array<bool, NB_TONS>& heap)
{
    for (size_t i = 0; i < NB_TONS; ++i)
    {
        if (heap[i])
            return false;
    }
    return true;
}


//        struct orderi
//        {
//            constexpr bool operator()(const int& x, const int& y) const noexcept
//            {
//                if (DIST[i][x] == UNDEF_DIST)
//                {
//                    return false;
//                }
//                else if (y.second != UNDEF_DIST)
//                {
//                    return (x.second != UNDEF_DIST);
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
//            assert(a < NB_TONS);
//            assert(b < NB_TONS);
//            if (DIST[i][a] == UNDEF_DIST)
//            {
//                return (DIST[i][b] != UNDEF_DIST);
//            }
//            else if (DIST[i][b] == UNDEF_DIST)
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
//            assert(a < NB_TONS);
//            assert(b < NB_TONS);
//            if (DIST[i][a] == UNDEF_DIST)
//            {
//                return false;
//            }
//            else if (DIST[i][b] == UNDEF_DIST)
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
//    assert(DIST[j][i] != UNDEF_DIST);
//    DIST[i][j] = DIST[j][i];
//}

void WeberModal::init()
{
    // row i
    for (size_t i = 0; i < NB_TONS; i++)
    {
        //typedef std::priority_queue<size_t, std::vector<size_t>, decltype(comparinfi)> q;
        //q pqueue(comparinfi);
        std::array<bool, NB_TONS> heap;
        heap.fill(true); // all in heap
        
        // null diagonal
        DIST[i][i] = 0;
        //for (size_t j = 0; j < NB_TONS; ++j)
        //    pqueue.push(j); // std::make_pair(j, UNDEF_DIST));

        // complete the lower part of triangle (in column i)
        while (! isEmpty(heap))
        {
            //size_t k = pqueue.top();
            size_t k = extractMin(heap, DIST[i]);
            assert(k < NB_TONS);
            int distik = DIST[i][k];
            assert(distik != UNDEF_DIST);
            
            // explore the neighbour tonalities of k
            for (size_t l = 0; l < 4; ++l)
            {
                size_t nl = neighbour(k, l);
                assert(nl <= NB_TONS);
                assert(nl == NB_TONS ||
                       DIST[i][nl] == UNDEF_DIST ||
                       DIST[i][nl] <= distik + 1);
                if ((nl < NB_TONS) &&
                    (DIST[i][nl] == UNDEF_DIST || (distik + 1) < DIST[i][nl]))
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
    for (size_t i = 0; i < NB_TONS; ++i)
    {
        std::cout << "{ ";
        for (size_t j = 0; j < NB_TONS; ++j)
        {
            assert(DIST[i][j] != UNDEF_DIST);
            if (DIST[i][j] < 10)
                std::cout << ' ';
            std::cout << DIST[i][j];
            if (j < NB_TONS-1)
                std::cout << ", ";
        }
        std::cout << " }";
        if (i < NB_TONS-1)
            std::cout << ", ";
        std::cout << std::endl;
    }
}


// static
size_t WeberModal::index(const Ton& ton)
{
    if (ton.getMode() == ModeName::Major)
    {
        return ton.fifths() + 7;
    }
    else if (ton.getMode() == ModeName::Minor    ||
             ton.getMode() == ModeName::MinorNat ||
             ton.getMode() == ModeName::MinorMel)
    {
        return ton.fifths() + 22;
    }
    else
    {
        ERROR("Weber index undefined for {}", ton);
        return NB_TONS;
    }
}


// static
size_t WeberModal::neighbour(size_t i, size_t n)
{
    assert(n < 8);
    size_t FIRST_MIN_HARM = NB_TONS/9;
    size_t FIRST_MIN_MEL = 2*NB_TONS/9;
    size_t FIRST_DOR = 3*NB_TONS/9;
    size_t FIRST_PHRY = 4*NB_TONS/9;
    size_t FIRST_LYD = 5*NB_TONS/9;
    size_t FIRST_MIXO = 6*NB_TONS/9;
    size_t FIRST_AEOL = 7*NB_TONS/9;
    size_t FIRST_LOC = 8*NB_TONS/9;
    
    long k = i; // signed
    assert(0 <= k);
    assert(k < NB_TONS);
    
    // major tonality
    if (0 <= k && k < FIRST_MIN_HARM)
    {
        switch (n)
        {
                // up : next major after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIN_HARM)?(k+1):NB_TONS);
                // left = parallel minor of k : "ton homonyme"
            case 1:
                return ((k-3 >= 0)?(k-3+FIRST_MIN_HARM):NB_TONS);
                // right : relative minor of k
            case 2:
                assert(k+FIRST_MIN_HARM < FIRST_MIN_MEL);
                return (k+FIRST_MIN_HARM);
                // down : previous major before k in array of fifths
            case 3:
                return ((k-1 >= 0)?(k-1):NB_TONS);
                // back : lydian mode with the same key signature
            case 4:
                return ((k-1 >= 0)?(k-1+FIRST_LYD):NB_TONS);
                // front : mixolydian mode with the same key signature
            case 5:
                return ((k+1+FIRST_MIXO < FIRST_AEOL)?(k+1+FIRST_MIXO):NB_TONS);
                // relative min_nat
            case 6:
                assert(k+FIRST_AEOL < NB_TONS);
                return (k+FIRST_AEOL);
                // relative min_mel
            case 7:
                assert(k+FIRST_MIN_MEL < NB_TONS);
                return (k+FIRST_MIN_MEL);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    // minor tonality
    else if (FIRST_MIN_HARM <= k && k < FIRST_MIN_MEL)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIN_MEL)?(k+1):NB_TONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM);
                // right : next major after left in array of fifths
                //        = parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_MIN_MEL)?(k+3-FIRST_MIN_HARM):NB_TONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN_HARM)?(k-1):NB_TONS);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_MIN_HARM)?((k-1)+FIRST_DOR-FIRST_MIN_HARM):NB_TONS);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_MIN_MEL)?(k+1+FIRST_PHRY-FIRST_MIN_HARM):NB_TONS);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM+FIRST_MIN_MEL);
            case 7:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM+FIRST_AEOL);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_MIN_MEL <= k && k < FIRST_DOR)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < FIRST_DOR)?(k+1):NB_TONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL);
                // right : next major after left in array of fifths
                //        = parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_DOR)?(k+3-FIRST_MIN_MEL):NB_TONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN_MEL)?(k-1):NB_TONS);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_MIN_MEL)?((k-1)+FIRST_DOR-FIRST_MIN_MEL):NB_TONS);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_DOR)?(k+1+FIRST_PHRY-FIRST_MIN_MEL):NB_TONS);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL+FIRST_MIN_HARM);
            case 7:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL+FIRST_AEOL);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_DOR <= k && k < FIRST_PHRY)
    {
        switch (n)
        {
                // up : next dorian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_PHRY)?(k+1):NB_TONS);
                // left : relative lydian of k
            case 1:
                assert(k-FIRST_DOR >= 0);
                return (k-FIRST_DOR+FIRST_LYD);
                // right = parallel lydian of k
            case 2:
                return ((k+3 < FIRST_PHRY)?(k+3-FIRST_DOR+FIRST_LYD):NB_TONS);
                // down : previous dorian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_DOR)?(k-1):NB_TONS);
                // back : mixolydian mode with the same key sig
            case 4:
                return ((k+2 < FIRST_PHRY)?((k+2)-FIRST_DOR+FIRST_MIXO):NB_TONS);
                // front : minor mode with the same key sig
            case 5:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_MIN_HARM):NB_TONS);
                // the other minor modes have to be close too...
            case 6:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_MIN_MEL):NB_TONS);
            case 7:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_AEOL):NB_TONS);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_PHRY <= k && k < FIRST_LYD)
    {
        switch (n)
        {
                // up : next phrygian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_LYD)?(k+1):NB_TONS);
                // left : relative mixolydian of k
            case 1:
                assert(k-FIRST_PHRY >= 0);
                return (k-FIRST_PHRY+FIRST_MIXO);
                // right = parallel mixolydian of k
            case 2:
                return ((k+3 < FIRST_LYD)?(k+3-FIRST_PHRY+FIRST_MIXO):NB_TONS);
                // down : previous phrygian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_PHRY)?(k-1):NB_TONS);
                // back : minor mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_MIN_HARM):NB_TONS);
                // front : locrian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_LYD)?(k+1-FIRST_PHRY+FIRST_LOC):NB_TONS);
                // the other minor modes have to be close too...
            case 6:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_MIN_MEL):NB_TONS);
            case 7:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_AEOL):NB_TONS);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_LYD <= k && k < FIRST_MIXO)
    {
        switch (n)
        {
                // up : next lydian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIXO)?(k+1):NB_TONS);
                // right : relative dorian of k
            case 1:
                return ((k-FIRST_LYD >= 0)?(k-FIRST_LYD+FIRST_DOR):NB_TONS);
                // left : next dorian after right in array of fifths
                //        = parallel dorian of k
            case 2:
                return ((k-3>=FIRST_LYD)?(k-3-FIRST_LYD+FIRST_DOR):NB_TONS);
                // down : previous lydian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_LYD)?(k-1):NB_TONS);
                // front : Major tone with the same key signature
            case 4:
                return ((k+1 < FIRST_MIXO)?(k+1-FIRST_LYD):NB_TONS);
            case 5:
                return NB_TONS;
            case 6:
                return NB_TONS;
            case 7:
                return NB_TONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_MIXO <= k && k < FIRST_AEOL)
    {
        switch (n)
        {
                // up : next lydian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_AEOL)?(k+1):NB_TONS);
                // right : relative phrygian of k
            case 1:
                return ((k-FIRST_MIXO >= 0)?(k-FIRST_MIXO+FIRST_PHRY):NB_TONS);
                // left = parallel phrygian of k
            case 2:
                return ((k-3>=FIRST_MIXO)?(k-3-FIRST_MIXO+FIRST_PHRY):NB_TONS);
                // down : previous mixolydian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIXO)?(k-1):NB_TONS);
                // front : dorian tone with the same key signature
            case 4:
                return ((k-2 >= FIRST_MIXO)?(k-2-FIRST_MIXO+FIRST_DOR):NB_TONS);
                // back : major tone with the same key signature
            case 5:
                return ((k-1 >= FIRST_MIXO)?(k-1-FIRST_MIXO):NB_TONS);
            case 6:
                return NB_TONS;
            case 7:
                return NB_TONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_AEOL <= k && k < FIRST_LOC)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < FIRST_LOC)?(k+1):NB_TONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL);
                // right : next major after left in array of fifths
                //        = parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_LOC)?(k+3-FIRST_AEOL):NB_TONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_AEOL)?(k-1):NB_TONS);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_AEOL)?((k-1)+FIRST_DOR-FIRST_AEOL):NB_TONS);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_LOC)?(k+1+FIRST_PHRY-FIRST_AEOL):NB_TONS);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL+FIRST_MIN_HARM);
            case 7:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_AEOL+FIRST_MIN_MEL);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else if (FIRST_LOC <= k && k < NB_TONS)
    {
        switch (n)
        {
                // up : next locrian after k in array of fifths
            case 0:
                return ((k+1 < NB_TONS)?(k+1):NB_TONS);
                // left : "relative dorian" of k
            case 1:
                assert(k-3-FIRST_LOC >= 0);
                return (k-3-FIRST_LOC+FIRST_DOR);
                // right = parallel dorian of k
            case 2:
                return (k-FIRST_LOC+FIRST_DOR);
                // down : previous locrian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_LOC)?(k-1):NB_TONS);
                // back : phrygian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_LOC)?((k-1)+FIRST_PHRY-FIRST_LOC):NB_TONS);
            case 5:
                return NB_TONS;
            case 6:
                return NB_TONS;
            case 7:
                return NB_TONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else
    {
        ERROR("Weber neighbour: unexpected ton index {}", k);
        return NB_TONS;
    }
}


size_t WeberModal::neighbour_tonal(size_t i, size_t n)
{
    assert(n < 4);
    size_t FIRST_MIN = NB_TONS/2;
    
    long k = i; // signed
    assert(0 <= k);
    assert(k < NB_TONS);
    
    // major tonality
    if (0 <= k && k < FIRST_MIN)
    {
        switch (n)
        {
                // up : next major after k in array of fifths
            case 0:
                return ((k+1 < FIRST_MIN)?(k+1):NB_TONS);
                // left : next minor after right in array of fifths
                //        = parallel minor of k
            case 1:
                return ((k-3 >= 0)?(k-3+FIRST_MIN):NB_TONS);
                // right : relative minor of k
            case 2:
                assert(k+FIRST_MIN < NB_TONS);
                return (k+FIRST_MIN);
                // down : previous major before k in array of fifths
            case 3:
                return ((k-1 >= 0)?(k-1):NB_TONS);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    // minor tonality
    else if (FIRST_MIN <= k && k < NB_TONS)
    {
        switch (n)
        {
                // up : next minor after k in array of fifths
            case 0:
                return ((k+1 < NB_TONS)?(k+1):NB_TONS);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN >= 0);
                return (k-FIRST_MIN);
                // right : next major after left in array of fifths
                //        = parallel major of k
            case 2:
                return ((k+3 < NB_TONS)?(k+3-FIRST_MIN):NB_TONS);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN)?(k-1):NB_TONS);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return NB_TONS;
        }
    }
    else
    {
        ERROR("Weber neighbour: unexpected ton index {}", k);
        return NB_TONS;
    }
}



// static
const std::array<std::array<int, WeberModal::NB_TONS>, 
                                 WeberModal::NB_TONS> WeberModal::WEBER_DIST =
{{
//     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29
//    -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7
//    Cb  Gb  Db  Ab  Eb  Bb   F   C   G   D   A   E   B  F#  C#  ab  eb  bb   f   c   g   d   a   e   b  f#  c#  g#  d#  a#
    {  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8,  9, 10,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8,  9,  9, 10, 11 }, // Cb -7
    {  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8,  9,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8,  9,  9, 10 }, // Gb -6
    {  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8,  9,  9 }, // Db -5
    {  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8,  9 }, // Ab -4
    {  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8 }, // Eb -3
    {  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7 }, // Bb -2
    {  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7 }, // F  -1
    {  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6 }, // C   0
    {  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5 }, // G   1
    {  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5 }, // D   2
    {  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4 }, // A   3
    {  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3 }, // E   4
    {  8,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3 }, // B   5
    {  9,  8,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2 }, // F#  6
    { 10,  9,  8,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  9,  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1 }, // C#  7
    {  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8,  9,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8,  9, 10 }, // ab -7
    {  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  8,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8,  9 }, // eb -6
    {  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  7,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8 }, // bb -5
    {  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  7,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8 }, // f  -4
    {  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  6,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7 }, // c  -3
    {  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  5,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6,  6 }, // g  -2
    {  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5,  6 }, // d  -1
    {  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  4,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4,  5 }, // a   0
    {  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  3,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4,  4 }, // e   1
    {  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  3,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3,  4 }, // b   2
    {  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  2,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2,  3 }, // f#  3
    {  9,  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  1,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2,  2 }, // c#  4
    {  9,  9,  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  2,  8,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1,  2 }, // g#  5
    { 10,  9,  9,  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1,  2,  9,  8,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0,  1 }, // d#  6
    { 11, 10,  9,  9,  8,  7,  7,  6,  5,  5,  4,  3,  3,  2,  1, 10,  9,  8,  8,  7,  6,  6,  5,  4,  4,  3,  2,  2,  1,  0 }  // a#  7
}};

    
    


} // end namespace pse
