//
//  WeberModal.cpp
//  pse
//
//  Created by Florent Jacquemard on 27/02/2024.
//





#include "WeberModal.hpp"
#include "Weber.hpp"


namespace pse {


WeberModal::WeberModal():
DIST()
{
    for (size_t i = 0; i < NB_TONS; ++i)
        DIST[i].fill(Weber::UNDEF_DIST);
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
size_t WeberModal::extractMin(std::array<bool, NB_TONS>& heap,
                         const std::array<int, NB_TONS>& dist)
{
    size_t m = NB_TONS; // undef
    for (size_t i = 0; i < NB_TONS; ++i)
    {
        assert(m <= NB_TONS);
        if (heap[i] && (m == NB_TONS || Weber::inf(dist[i], dist[m])))
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
//                if (DIST[i][x] == Weber::UNDEF_DIST)
//                {
//                    return false;
//                }
//                else if (y.second != Weber::UNDEF_DIST)
//                {
//                    return (x.second != Weber::UNDEF_DIST);
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
//            if (DIST[i][a] == Weber::UNDEF_DIST)
//            {
//                return (DIST[i][b] != Weber::UNDEF_DIST);
//            }
//            else if (DIST[i][b] == Weber::UNDEF_DIST)
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
//            if (DIST[i][a] == Weber::UNDEF_DIST)
//            {
//                return false;
//            }
//            else if (DIST[i][b] == Weber::UNDEF_DIST)
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
//    assert(DIST[j][i] != Weber::UNDEF_DIST);
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
        //    pqueue.push(j); // std::make_pair(j, Weber::UNDEF_DIST));

        // complete the lower part of triangle (in column i)
        while (! isEmpty(heap))
        {
            //size_t k = pqueue.top();
            size_t k = extractMin(heap, DIST[i]);
            assert(k < NB_TONS);
            int distik = DIST[i][k];
            assert(distik != Weber::UNDEF_DIST);
            
            // explore the neighbour tonalities of k
            for (size_t l = 0; l < 4; ++l)
            {
                size_t nl = neighbour_modal(k, l);
                assert(nl <= NB_TONS);
                assert(nl == NB_TONS ||
                       DIST[i][nl] == Weber::UNDEF_DIST ||
                       DIST[i][nl] <= distik + 1);
                if ((nl < NB_TONS) &&
                    (DIST[i][nl] == Weber::UNDEF_DIST || (distik + 1) < DIST[i][nl]))
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
            assert(DIST[i][j] != Weber::UNDEF_DIST);
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
size_t WeberModal::index_modal(const Ton& ton)
{
    if (ton.getMode() == ModeName::Major)
    {
        return ton.fifths() + 7;
    }
    else if (ton.getMode() == ModeName::Minor)
    {
        return ton.fifths() + 22;
    }
    else if (ton.getMode() == ModeName::MinorMel)
    {
        return ton.fifths() + 37;
    }
    else if (ton.getMode() == ModeName::Dorian)
    {
        return ton.fifths() + 52;
    }
    else if (ton.getMode() == ModeName::Phrygian)
    {
        return ton.fifths() + 67;
    }
    else if (ton.getMode() == ModeName::Lydian)
    {
        return ton.fifths() + 82;
    }
    else if (ton.getMode() == ModeName::Mixolydian)
    {
        return ton.fifths() + 97;
    }
    else if (ton.getMode() == ModeName::Aeolian)
    {
        return ton.fifths() + 112;
    }
    else if (ton.getMode() == ModeName::Locrian)
    {
        return ton.fifths() + 127;
    }
    else
    {
    ERROR("Weber index undefined for {}", ton);
    return NB_TONS;
    }
}


// static
size_t WeberModal::neighbour_modal(size_t i, size_t n)
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
     


} // end namespace pse

