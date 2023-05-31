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
#include "PSTransit.hpp"


namespace pse {


PSB::PSB(const Algo& a, const Cost& seed, PSEnum& e,
         const Ton& gton, const Ton& lton):
_enum(e),
_bests(),   // empty
_cost(seed.shared_zero())     // zero
//_visited()  // empty
{
    if (! e.empty())
        init(gton, lton, a, seed);
    // otherwise n0 == n1, no note, leave _best empty
    else
    {
        
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


// search of best path
void PSB::init(const Ton& ton, const Ton& lton,
               const Algo& a, const Cost& seed)
{
    // at least one note, the bag cannot be empty.
    assert(_enum.first() < _enum.stop());

    assert((a == Algo::PSE0) || (a == Algo::PSE1) || (a == Algo::PS14));
    Transition transition(a, _enum);
    
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
    q.push(std::make_shared<const PSC0>(ton, _enum.first(), seed)); // n0
    
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
            transition.succ(c, ton, lton, q);
//            if (fsucc)
//                succ(c, ton, lton, q); //static
//            else
//                succ(c, ton, q);
        }
    }
}


// iff first() == stop()
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

