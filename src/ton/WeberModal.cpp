//
//  WeberModal.cpp
//  pse
//
//  Created by Florent Jacquemard on 27/02/2024.
//


#include "WeberModal.hpp"
#include "Weber_static.hpp"
//#include "WeberModal_static.hpp"
#include "WeberBluesModal_static.hpp"


namespace pse {


// static, private
// const size_t WeberModal::WMS_NBTONS = WeberBluesModal_static::NB_TONS;


// static
const int WeberModal::WS_UNDEFDIST = Weber_static::UNDEF_DIST;


WeberModal::WeberModal():
DIST()
{
    // fill the whole DIST table with UNDEF values
    for (size_t i = 0; i < WMS_NBTONS; ++i)
        DIST[i].fill(WS_UNDEFDIST);
    
    
    init();
}


WeberModal::~WeberModal()
{ }


int WeberModal::dist(const Ton& ton1, const Ton& ton2)
{
    size_t i1 = WeberBluesModal_static::index(ton1);
    assert(i1 < WMS_NBTONS);
    size_t i2 = WeberBluesModal_static::index(ton2);
    assert(i2 < WMS_NBTONS);
    return DIST[i1][i2];
}


// static
size_t WeberModal::extractMin(std::array<bool, WMS_NBTONS>& heap,
                        const std::array<int,  WMS_NBTONS>& dist)
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
    // null distance in diagonal
    for (size_t i = 0; i < WMS_NBTONS; i++)
    {
        DIST[i][i] = 0;
    }

    // null distance for enhamonics (of same mode)
    for (size_t i = 0; i < WMS_NBTONS; i++)
    {
        int i_fifths = (i%15)-7;
        for (size_t j = i+1; j < WMS_NBTONS; j++)
        {
            int j_fifths = (j%15)-7;
            // i/15 = j/15   (floor)
            if (WeberBluesModal_static::mode(i) == WeberBluesModal_static::mode(j))
            {
                if ((i_fifths == -7 && j_fifths ==  5) ||
                    (i_fifths == -6 && j_fifths ==  6) ||
                    (i_fifths == -5 && j_fifths ==  7) ||
                    (i_fifths ==  5 && j_fifths == -7) ||
                    (i_fifths ==  6 && j_fifths == -6) ||
                    (i_fifths ==  7 && j_fifths == -5))
                {
                    DIST[i][j] = 0;
                    DIST[j][i] = 0;
                }
            }
        }
    }

    // process row i
    for (size_t i = 0; i < WMS_NBTONS; i++)
    {
        //typedef std::priority_queue<size_t, std::vector<size_t>, decltype(comparinfi)> q;
        //q pqueue(comparinfi);
        std::array<bool, WMS_NBTONS> heap;
        heap.fill(true); // all in heap
        
        // null diagonal
        // DIST[i][i] = 0;
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
    assert(n < 9);
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
                return ((k+1 < FIRST_MIN_HARM)?(k+1):(k-11));
                // left : parallel minor of k : "ton homonyme"
            case 1:
                return ((k-3 >= 0)?(k-3+FIRST_MIN_HARM):k+9+FIRST_MIN_HARM);
                // right : relative minor of k
            case 2:
                assert(k+FIRST_MIN_HARM < FIRST_MIN_MEL);
                return (k+FIRST_MIN_HARM);
                // down : previous major before k in array of fifths
            case 3:
                return ((k-1 >= 0)?(k-1):(k+11));
                // back : lydian mode with the same key signature
            case 4:
                return ((k-1 >= 0)?(k-1+FIRST_LYD):k+11+FIRST_LYD);
                // front : mixolydian mode with the same key signature
            case 5:
                return ((k+1+FIRST_MIXO < FIRST_AEOL)?(k+1+FIRST_MIXO):k-11+FIRST_MIXO);
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
                return ((k+1 < FIRST_MIN_MEL)?(k+1):k-11);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM);
                // right : parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_MIN_MEL)?(k+3-FIRST_MIN_HARM):k-9-FIRST_MIN_HARM);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN_HARM)?(k-1):k+11);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_MIN_HARM)?((k-1)+FIRST_DOR-FIRST_MIN_HARM):(k+11+FIRST_DOR-FIRST_MIN_HARM));
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_MIN_MEL)?(k+1+FIRST_PHRY-FIRST_MIN_HARM):(k-11+FIRST_PHRY-FIRST_MIN_HARM));
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM+FIRST_MIN_MEL);
            case 7:
                assert(k-FIRST_MIN_HARM >= 0);
                return (k-FIRST_MIN_HARM+FIRST_AEOL);
                // homonymous blues scale
            case 8:
                assert(k-FIRST_MIN_HARM+FIRST_BLMIN < WMS_NBTONS);
                return (k-FIRST_MIN_HARM+FIRST_BLMIN);
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
                return ((k+1 < FIRST_DOR)?(k+1):k-11);
                // left : relative major of k
            case 1:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL);
                // right : parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_DOR)?(k+3-FIRST_MIN_MEL):k-9-FIRST_MIN_MEL);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIN_MEL)?(k-1):k+11);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_MIN_MEL)?((k-1)+FIRST_DOR-FIRST_MIN_MEL):((k+11)+FIRST_DOR-FIRST_MIN_MEL));
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_DOR)?(k+1+FIRST_PHRY-FIRST_MIN_MEL):k-11+FIRST_PHRY-FIRST_MIN_MEL);
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL+FIRST_MIN_HARM);
            case 7:
                assert(k-FIRST_MIN_MEL >= 0);
                return (k-FIRST_MIN_MEL+FIRST_AEOL);
                // homonymous blues scale
            case 8:
                assert(k-FIRST_MIN_MEL+FIRST_BLMIN < WMS_NBTONS);
                return (k-FIRST_MIN_MEL+FIRST_BLMIN);
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
                return ((k+1 < FIRST_PHRY)?(k+1):k-11);
                // left : relative lydian of k
            case 1:
                assert(k-FIRST_DOR >= 0);
                return (k-FIRST_DOR+FIRST_LYD);
                // right = parallel lydian of k
            case 2:
                return ((k+3 < FIRST_PHRY)?(k+3-FIRST_DOR+FIRST_LYD):k-9-FIRST_DOR+FIRST_LYD);
                // down : previous dorian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_DOR)?(k-1):k+11);
                // back : mixolydian mode with the same key sig
            case 4:
                return ((k+2 < FIRST_PHRY)?((k+2)-FIRST_DOR+FIRST_MIXO):((k-10)-FIRST_DOR+FIRST_MIXO));
                // front : minor mode with the same key sig
            case 5:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_MIN_HARM):(k-11-FIRST_DOR+FIRST_MIN_HARM));
                // the other minor modes have to be close too...
            case 6:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_MIN_MEL):k-11-FIRST_DOR+FIRST_MIN_MEL);
            case 7:
                return ((k+1 < FIRST_PHRY)?(k+1-FIRST_DOR+FIRST_AEOL):k-11-FIRST_DOR+FIRST_AEOL);
                // homonymous blues scale
            case 8:
                assert(k-FIRST_DOR+FIRST_BLMIN < WMS_NBTONS);
                return (k-FIRST_DOR+FIRST_BLMIN);
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
                return ((k+1 < FIRST_LYD)?(k+1):k-11);
                // left : relative mixolydian of k
            case 1:
                assert(k-FIRST_PHRY >= 0);
                return (k-FIRST_PHRY+FIRST_MIXO);
                // right = parallel mixolydian of k
            case 2:
                return ((k+3 < FIRST_LYD)?(k+3-FIRST_PHRY+FIRST_MIXO):k-9-FIRST_PHRY+FIRST_MIXO);
                // down : previous phrygian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_PHRY)?(k-1):k+11);
                // back : minor mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_MIN_HARM):(k+11-FIRST_PHRY+FIRST_MIN_HARM));
                // front : locrian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_LYD)?(k+1-FIRST_PHRY+FIRST_LOC):(k-11-FIRST_PHRY+FIRST_LOC));
                // the other minor modes have to be close too...
            case 6:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_MIN_MEL):k+11-FIRST_PHRY+FIRST_MIN_MEL);
            case 7:
                return ((k-1 >= FIRST_PHRY)?(k-1-FIRST_PHRY+FIRST_AEOL):(k+11-FIRST_PHRY+FIRST_AEOL));
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
                return ((k+1 < FIRST_MIXO)?(k+1):k-11);
                // right : relative dorian of k
            case 1:
                return ((k-FIRST_LYD >= 0)?(k-FIRST_LYD+FIRST_DOR):WMS_NBTONS);
                // left : parallel dorian of k
            case 2:
                return ((k-3>=FIRST_LYD)?(k-3-FIRST_LYD+FIRST_DOR):k+9-FIRST_LYD+FIRST_DOR);
                // down : previous lydian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_LYD)?(k-1):k+11);
                // front : Major tone with the same key signature
            case 4:
                return ((k+1 < FIRST_MIXO)?(k+1-FIRST_LYD):k-11-FIRST_LYD);
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
                // up : next mixolydian after k in array of fifths
            case 0:
                return ((k+1 < FIRST_AEOL)?(k+1):k-11);
                // right : relative phrygian of k
            case 1:
                return ((k-FIRST_MIXO >= 0)?(k-FIRST_MIXO+FIRST_PHRY):WMS_NBTONS);
                // left = parallel phrygian of k
            case 2:
                return ((k-3>=FIRST_MIXO)?(k-3-FIRST_MIXO+FIRST_PHRY):k+9-FIRST_MIXO+FIRST_PHRY);
                // down : previous mixolydian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_MIXO)?(k-1):k+11);
                // front : dorian tone with the same key signature
            case 4:
                return ((k-2 >= FIRST_MIXO)?(k-2-FIRST_MIXO+FIRST_DOR):k+10-FIRST_MIXO+FIRST_DOR);
                // back : major tone with the same key signature
            case 5:
                return ((k-1 >= FIRST_MIXO)?(k-1-FIRST_MIXO):k+11-FIRST_MIXO);
            case 6:
                return WMS_NBTONS;
            case 7:
                return WMS_NBTONS;
                //homonymous blues scale
            case 8:
                assert(k-FIRST_MIXO+FIRST_BLMAJ < WMS_NBTONS);
                return (k-FIRST_MIXO+FIRST_BLMAJ);
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
                return ((k+1 < FIRST_LOC)?(k+1):k-11);
                // left : relative major of k
            case 1:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL);
                // right : parallel major of k "ton homonyme"
            case 2:
                return ((k+3 < FIRST_LOC)?(k+3-FIRST_AEOL):k-9-FIRST_AEOL);
                // down : previous minor before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_AEOL)?(k-1):k+11);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_AEOL)?((k-1)+FIRST_DOR-FIRST_AEOL):(k+11)+FIRST_DOR-FIRST_AEOL);
                // front : phrygian mode with the same key sig
            case 5:
                return ((k+1 < FIRST_LOC)?(k+1+FIRST_PHRY-FIRST_AEOL):(k-11+FIRST_PHRY-FIRST_AEOL));
                // the other minor modes have to be close too...
            case 6:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL+FIRST_MIN_HARM);
            case 7:
                assert(k-FIRST_AEOL >= 0);
                return (k-FIRST_AEOL+FIRST_MIN_MEL);
                //homonymous blues scale
            case 8:
                assert(k-FIRST_AEOL+FIRST_BLMIN < WMS_NBTONS);
                return (k-FIRST_AEOL+FIRST_BLMIN);
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_LOC <= k && k < FIRST_BLMAJ)
    {
        switch (n)
        {
                // up : next locrian after k in array of fifths
            case 0:
                return ((k+1 < WMS_NBTONS)?(k+1):k-11);
                // left : "relative dorian" of k
            case 1:
                return ((k-3-FIRST_LOC >= 0)?(k-3-FIRST_LOC+FIRST_DOR):(k+9-FIRST_LOC+FIRST_DOR));
                // right = parallel dorian of k
            case 2:
                return (k-FIRST_LOC+FIRST_DOR);
                // down : previous locrian before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_LOC)?(k-1):k+11);
                // back : phrygian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_LOC)?((k-1)+FIRST_PHRY-FIRST_LOC):(k+11)+FIRST_PHRY-FIRST_LOC);
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
    else if (FIRST_BLMAJ <= k && k < FIRST_BLMIN){
        switch (n)
        {
                // up : next major blues after k in array of fifths
            case 0:
                return ((k+1 < FIRST_BLMIN)?(k+1):k-11);
                // left : parallel minor blues of k : "ton homonyme"
            case 1:
                return ((k-3 >= FIRST_BLMAJ)?(k-3-FIRST_BLMAJ+FIRST_BLMIN):k+9-FIRST_BLMAJ+FIRST_BLMIN);
                // right : relative minor blues of k
            case 2:
                assert(k-FIRST_BLMAJ+FIRST_BLMIN < WMS_NBTONS);
                return (k-FIRST_BLMAJ+FIRST_BLMIN);
                // down : previous major blues before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_BLMAJ)?(k-1):k+11);
                // back : lydian mode with the same key signature
            case 4:
                return ((k-1 >= FIRST_BLMAJ)?(k-1-FIRST_BLMAJ+FIRST_LYD):(k+11-FIRST_BLMAJ+FIRST_LYD));
                // front : major mode with the same key signature
            case 5:
                return ((k-FIRST_BLMAJ < FIRST_MIN_HARM)?(k-FIRST_BLMAJ):WMS_NBTONS);
                
            case 6:
                return WMS_NBTONS;//peut-etre pourrait-on ajouter un voisin mixolydien car pas de raison d'etre plus proche du lydien que du mixolydien... mais cela casserait l'architecture de la table alternative modale de Weber proposee
            case 7:
                return WMS_NBTONS;
            case 8:
                return WMS_NBTONS;
            default:
                ERROR("Weber neighbour: unexpected neighbour number {}", n);
                return WMS_NBTONS;
        }
    }
    else if (FIRST_BLMIN <= k && k < WMS_NBTONS)
    {
        switch (n)
        {
                // up : next minor blues after k in array of fifths
            case 0:
                return ((k+1 < WMS_NBTONS)?(k+1):k-11);
                // left : relative major blues of k
            case 1:
                assert(k-FIRST_BLMIN >= 0);
                return (k-FIRST_BLMIN+FIRST_BLMAJ);
                // right : parallel major blues of k "ton homonyme"
            case 2:
                return ((k+3 < WMS_NBTONS)?(k+3-FIRST_BLMIN+FIRST_BLMAJ):k-9-FIRST_BLMIN+FIRST_BLMAJ);//ex sol# min a pour homonyme non pas sol# Maj qui n'existe pas mais bien lab Maj (9 quintes plus bas dans le cycle)
                // down : previous minor blues before k in array of fifths
            case 3:
                return ((k-1 >= FIRST_BLMIN)?(k-1):k+11);
                // back : dorian mode with the same key sig
            case 4:
                return ((k-1 >= FIRST_BLMIN)?((k-1)+FIRST_DOR-FIRST_BLMIN):(k+11)+FIRST_DOR-FIRST_BLMIN);
                // front : aeolian minor mode with the same key sig
            case 5:
                return (k+FIRST_AEOL-FIRST_BLMIN);
                // the other minor modes have to be close too... or do they? not sure on this one as the aeolian mode is closer to minor blues than the harmonic and ascending minor ones, so let's just stick with the aeolian
            case 6:
               // return (k+FIRST_MIN_MEL-FIRST_BLMIN);
                return WMS_NBTONS;
            case 7:
                //return (k+FIRST_MIN_HARM-FIRST_BLMIN);
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
