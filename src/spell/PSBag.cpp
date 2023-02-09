//
//  PSBag.cpp
//  qparse
//
//  Created by Florent Jacquemard on 10/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include "PSBag.hpp"


namespace pse {

/// ordering for PS Config0 based on lexico combination of
/// - cost (nb accidents, dist. to local tonality, number of disjoint moves, color)
/// - index in enumerator
PSCCompare PSClex =
[](std::shared_ptr<const PSC0> lhs, std::shared_ptr<const PSC0> rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost() == rhs->cost())
        return (lhs->id() < rhs->id());  // largest index
    else
        return (lhs->cost() > rhs->cost());  // smallest cost
};
//    if (lhs->accidentals() == rhs->accidentals())
//    {
//        // dist, lexicographically
//        if (lhs->dist() == rhs->dist())
//        {
//            // disjoint moves, lexicographically
//            if (lhs->disjoint() == rhs->disjoint())
//                return false; // tie break fail
//            else
//                return (lhs->disjoint() > rhs->disjoint());  // smallest nb mv disjoint moves
//        }
//        else
//            return (lhs->dist() > rhs->dist());  // smallest dist
//    }
//    else
//        return (lhs->accidentals() > rhs->accidentals());  // smallest cost


/// ordering for PS Config0 based on nb of accidents only.
PSCCompare PSCacc =
[](std::shared_ptr<const PSC0> lhs, std::shared_ptr<const PSC0> rhs)
{
    assert (lhs);
    assert (rhs);
    if (lhs->cost().getAccid() == rhs->cost().getAccid())
        return (lhs->id() < rhs->id());  // largest index
    else
        return (lhs->cost().getAccid() > rhs->cost().getAccid());
};


/// ordering for base PSConfig0 based on dist.
/// @todo TBR
/// @todo mv to PSC0
PSCCompare PSCdist =
[](std::shared_ptr<const PSC0> lhs, std::shared_ptr<const PSC0> rhs)
{
    assert (lhs);
    assert (rhs);
    return (lhs->cost().getDist() > rhs->cost().getDist());
};


PSB::PSB(const Ton& ton, PSEnum& e):
_enum(e),
_bests(),  // empty
_cost(),   // zero
_visited() // empty
{
    if (! e.empty())
        init(ton, ton, false); // second arg. ton is ignored
    // otherwise n0 == n1, no note, leave the bag _bests empty
}


PSB::PSB(const Ton& ton, const Ton& lton, PSEnum& e):
_enum(e),
_bests(),   // empty
_cost(),    // zero
_visited()  // empty
{
    if (! e.empty())
        init(ton, lton, true);
    // otherwise n0 == n1, no note, leave _best empty
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
    _visited.clear();
}


// search of best path
void PSB::init(const Ton& ton, const Ton& lton, bool fsucc)
{
    // at least one note, the bag cannot be empty.
    assert(_enum.first() < _enum.stop());
    
    // heap of candidate configs for the best path search (incomplete paths)
    PSCQueue q;
    if (fsucc)
        q = PSCQueue(PSClex); // empty
    else
        q = PSCQueue(PSCacc); // empty
    
    // initial configuration
    q.push(std::make_shared<const PSC0>(ton, _enum.first())); // n0
    
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
                _cost = c->cost(); // copy
            }
            else
            {
                // c is a minimal complete path
                if (c->cost() == _cost)
                {
                    _bests.push_back(c);
                }
                // we have found all the minimal complete paths
                else
                {
                    assert(c->cost() > _cost);
                    break;
                }
            }
        }
        // prune: the cost of current config already overwhelmed
        // the cost of a best path found
        else if ((! _bests.empty()) && (c->cost() > _cost))
        {
            continue;
        }
        // complete the path of c with its successors
        else
        {
            // c will bbr the prev of the succ computed here,
            // keep it for rollback or best path.
            _visited.push_back(c);
            // add every possible successor configs to q
            if (fsucc)
                c->succ(_enum, ton, lton, q);
            else
                c->succ(_enum, ton, q);
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


const PSCost& PSB::cost() const
{
    assert((! _bests.empty()) || _enum.empty());
    return _cost;
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
    std::shared_ptr<const PSC0> psc = _bests.at(0);
    assert(psc);
    return *psc;
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


} // end namespace pse

/// @}

