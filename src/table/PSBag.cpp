//
//  PSBag.cpp
//  qparse
//
//  Created by Florent Jacquemard on 10/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "PSBag.hpp"
#include "Enharmonic.hpp"
#include "PSOrder.hpp"
// #include "PSTransit.hpp" // obsolete


namespace pse {


PSB::PSB(const Algo& a, const Cost& seed, PSEnum& e,
         const Ton& gton, const Ton& lton, bool tonal):
_algo(a),
_enum(e),
_bests(),   // empty
_cost(seed.shared_zero())     // zero
//_visited()  // empty
{
    if (! e.empty())
        init(seed, gton, lton, tonal);
    // otherwise n0 == n1, no note, leave _best empty
    else
    {
        // msg
    }
}


PSB::~PSB()
{
    // deallocate all configs in the priority queue
    TRACE("delete PSB");
  
    // clear priority queue
    // while(! _bests.empty())
    // {
    //    std::shared_ptr<const PSC0> c = _bests.top();
    //    if(c) delete c;
    //    _bests.pop();
    // }

    // free other configs
    //for (PSConfig* c : _visited)
    //{
    //    if(c) delete c;
    // }
    //_visited.clear();
}


// algo best path search
void PSB::init(const Cost& seed, const Ton& ton, const Ton& lton, bool tonal)
{
    // at least one note, the bag cannot be empty.
    assert(_enum.first() < _enum.stop());

    assert((_algo == Algo::PSE) || (_algo == Algo::PSD));
    //Transition transition(a, _enum);
    
    // backup of configurations during construction
    // to prevent deletion when pop from queue
    std::vector<std::shared_ptr<const PSC0>> visited;

    // heap of candidate configs for the best path search (incomplete paths)
    // every configuration created is added to this heap
    // final configurations of a best path are moved to _best
    // other configurations are moved to _visited (except if the cost is
    // larger than cost of a best path)
    // @todo limtit _visited to non-final configurations in a best path

    // use the default ordering > (and ==) on PSCost
    PSCQueue q = PSCQueue(PSClex());
    
//    if (fsucc)
//        q = PSCQueue(PSClex()); // empty
//    else
//        q = PSCQueue(PSCad()); // empty
    
    // initial configuration
    q.push(std::make_shared<const PSC0>(ton, _enum.first(), seed, tonal)); // n0
    
    while (! q.empty())
    {
        std::shared_ptr<const PSC0> c = q.top(); // smallest
        assert(c);
        assert(_enum.first() <= c->id());
        assert(c->id() <= _enum.stop());
        q.pop(); // remove c
        
        // the path c is complete
        if (c->id() == _enum.stop())
        {
            assert(c->id() != _enum.first()); // measure not empty
            // first minimal complete path found
            if (_bests.empty())
            {
                _bests.push_back(c);
                _cost = c->cost().shared_clone(); // copy
            }
            else
            {
                // c is a minimal complete path
                if (c->cost() == this->cost())
                {
                    _bests.push_back(c);
                }
                // we have found all the minimal complete paths
                else
                {
                    assert(c->cost() > this->cost());
                    break;
                }
            }
        }
        // prune: the cost of current config already overwhelmed
        // the cost of a best path found
        else if ((! _bests.empty()) && (c->cost() > this->cost()))
        {
            continue;
        }
        // complete the path of c with its successors
        else
        {
            // keep c from deletion, as it might be added to _inbest
            // for rollback of best path.
            // (c will be the prev of the succ computed here)
            visited.push_back(c);
            // add every possible successor configs to q
            succ(c, q, ton, lton);
        }
    }
}


void PSB::succ(std::shared_ptr<const PSC0> c, PSCQueue& q,
               const Ton& gton, const Ton& lton) const
{
    assert(c);
    size_t id = c->id();

    // parameters for initialization of next configs
    std::stack<enum NoteName> names;
    std::stack<enum Accid> accids;
    //std::stack<bool> prints;
    
    // note in chord
    if (_enum.simultaneous(id))
    {
        // first note of chord
        if (! c->inChord())
        {
            get_names(id, gton, names, accids);
            assert(names.size() == accids.size());
            //assert(prints.size() == accids.size());
            while (! names.empty())
            {
                q.push(std::make_shared<PSC1c>(c, _enum,
                                               names.top(),
                                               accids.top(),
                                               gton, lton));
                names.pop();
                accids.pop();
                //prints.pop();
            }
        }
        // next note of chord
        else
        {
            std::shared_ptr<const PSC1c> c1 =
            std::dynamic_pointer_cast<const PSC1c>(c);
            assert(c1);
            assert(c1->id() == id);
            const PSChord chord = c1->chord();
            unsigned int m = chord.midipitch(id) % 12; // chroma in 0..11
            const enum NoteName dejaname = c1->dejavu(m);
            
            // pitch class already processed in chord,
            // we resuse the previous name chosen for the pitch class
            if (dejaname != NoteName::Undef)
            {
                q.push(std::make_shared<PSC1c>(c1,
                                               dejaname,
                                               MidiNum::accid(m, dejaname),
                                               false,
                                               //c1->dejaprint(m), // force print
                                               gton, lton));
            }
            else
            {
                get_names(id, gton, names, accids);
                assert(names.size() == accids.size());
                //assert(prints.size() == accids.size());
                while (! names.empty())
                {
                    q.push(std::make_shared<PSC1c>(c1,
                                                   names.top(),
                                                   accids.top(),
                                                   false, // force print
                                                   gton, lton));
                    names.pop();
                    accids.pop();
                    //prints.pop();
                }
            }
        }
    }
    // single note
    else
    {
        get_names(id, gton, names, accids);
        assert(names.size() == accids.size());
        //assert(prints.size() == accids.size());
        while (! names.empty())
        {
            q.push(std::make_shared<PSC1>(c, _enum,
                                          names.top(),
                                          accids.top(),
                                          false, // force print
                                          gton, lton));
            names.pop();
            accids.pop();
            //prints.pop();
        }
    }
}


void PSB::get_names(size_t id, const Ton& gton,
                    std::stack<enum NoteName>& names,
                    std::stack<enum Accid>& accids) const
                    //std::stack<bool>& prints) const
{
    unsigned int pm = _enum.midipitch(id);
    assert(0 <= pm);
    assert(pm <= 128);
    // chroma in 0..11
    int m = pm % 12;

    // 3 potential successors in exhaustive search algo PSE
    // if ((_algo == Algo::PSE0) || (_algo == Algo::PSE1))
    if (_algo == Algo::PSE)
    {
        for (int j = 0; j < 3; ++j)
        {
            enum NoteName name = Enharmonics::name(m, j);
            enum Accid accid = Enharmonics::accid(m, j);
            // case of 8 and (short list) 1, 3, 6, 10
            if (! defined(name) || !defined(accid))
                continue;
            assert(accid == MidiNum::accid(m, name));
            names.push(name);
            accids.push(accid);
            //prints.push(false); // no force print
        }
    }
    // only 1 potential successor in determonistic variant PSD
    else if (_algo == Algo::PSD)
    {
        // const Scale& scale = gton.chromatic();
        // int p = scale.pitchClass(0); // pitch class of tonic of scale
        // degree of m in the chromatic harmonic scale of p
        // size_t deg = (p <= m)?(m - p):(12-p+m);
        // assert(0 <= deg); // debug
        // assert(deg < 12);
        // enum NoteName name = scale.name(deg);
        // enum Accid accid = scale.accid(deg);
        enum NoteName name = gton.chromaname(m);
        enum Accid accid = MidiNum::accid(m, name);
        assert(defined(name));
        assert(defined(accid));
        assert(accid == MidiNum::accid(m, name));
        names.push(name);
        accids.push(accid);
        //prints.push(false); // no force print
    }
    else
    {
        ERROR ("Transition succ1: unexpected algo name {}", _algo);
    }

}


bool PSB::empty() const
{
    return _bests.empty();
}


size_t PSB::size() const
{
    return _bests.size();
}


const Cost& PSB::cost() const
{
    assert(_cost);
    return *(_cost);
//    assert((! _bests.empty()) || _enum.empty());
//    if (_bests.empty())
//    {
//        assert(_cost);
//        return *(_cost);
//    }
//    else
//    {
//        return top().cost();
//    }
}


const PSC0& PSB::at(size_t i) const
{
    //assert(! _bests.empty());
    assert(i < _bests.size());
    std::shared_ptr<const PSC0> psc = _bests.at(i);
    assert(psc);
    return *psc;
}


const PSC0& PSB::top() const
{
    assert(! _bests.empty());
    //std::shared_ptr<const PSC0> psc = _bests.at(0);
    assert(_bests.at(0));
    return *(_bests.at(0));
}


//void PSB::pop()
//{
//    assert(! _bests.empty());
//    _bests.pop();
//}


PSCHeap::const_iterator PSB::cbegin() const
{
    return _bests.cbegin();
}


PSCHeap::const_iterator PSB::cend() const
{
    return _bests.cend();
}


//void PSB::addBest(std::shared_ptr<const PSC0>& c)
//{
//    assert(c);
//    assert(_bests.empty() || c->cost() <= _cost);
//    _bests.push_back(c);
//}

} // end namespace pse

/// @}

