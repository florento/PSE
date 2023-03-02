//
//  PSBag.cpp
//  qparse
//
//  Created by Florent Jacquemard on 10/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "PSBag.hpp"
#include "Enharmonic.hpp"
#include "PSorder.hpp"


namespace pse {

PSB::PSB(const Ton& ton, PSEnum& e):
_enum(e),
_bests(),  // empty
_cost()   // zero
//_visited() // empty
{
    if (! e.empty())
        init(ton, ton, false); // second arg. ton is ignored
    // otherwise n0 == n1, no note, leave the bag _bests empty
}


PSB::PSB(const Ton& gton, const Ton& lton, PSEnum& e):
_enum(e),
_bests(),   // empty
_cost()    // zero
//_visited()  // empty
{
    if (! e.empty())
        init(gton, lton, true);
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
    //_visited.clear();
}


// search of best path
void PSB::init(const Ton& ton, const Ton& lton, bool fsucc)
{
    // at least one note, the bag cannot be empty.
    assert(_enum.first() < _enum.stop());

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
    PSCQueue q = PSCQueue(PSClex);
    
//    if (fsucc)
//        q = PSCQueue(PSClex); // empty
//    else
//        q = PSCQueue(PSCad); // empty
    
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
            // keep c from deletion, as it might be added to _inbest
            // for rollback of best path.
            // (c will be the prev of the succ computed here)
            visited.push_back(c);
            // add every possible successor configs to q
            if (fsucc)
                succ(c, ton, lton, q); //static
            else
                succ(c, ton, q);
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


void PSB::addBest(std::shared_ptr<const PSC0>& c)
{
    assert(c);
    assert(_bests.empty() || c->cost() <= _cost);
    _bests.push_back(c);
}


// static
void PSB::succ(std::shared_ptr<const PSC0>& c, // PSEnum& e,
               const Ton& ton, PSCQueue& q) const
{
    assert(c);
    if (_enum.simultaneous(c->id()))
        succ2(c, ton, q);    // chord
    else
        succ1(c, ton, q);    // single note
}


// static
void PSB::succ1(std::shared_ptr<const PSC0>& c, // PSEnum& e,
                const Ton& ton, PSCQueue& q) const
{
    assert(c);
    // midi pitch of the note read for transition from this config
    unsigned int pm = _enum.midipitch(c->id());
    assert(0 <= pm);
    assert(pm <= 127);
    int m = pm % 12; // chroma in 0..11

    for (int j = 0; j < 3; ++j)
    {
        NoteName name = Enharmonics::name(m, j);
        Accid accid = Enharmonics::accid(m, j);
        // case of 8 and (short list) 1, 3, 6, 10
        if (defined(name) && defined(accid))
            q.push(std::make_shared<PSC1>(c, _enum, name, accid, ton));
    }
}


// static
void PSB::succ2(std::shared_ptr<const PSC0>& c, // PSEnum& e,
                const Ton& ton, PSCQueue& q) const
{
    assert(c);
    //assert(c->size() > 1);
    std::stack<std::shared_ptr<const PSC2>> cs;
    cs.push(std::make_shared<const PSC2>(c, _enum, c->id()));     // initial config
    
    while (! cs.empty())
    {
        std::shared_ptr<const PSC2> c2 = cs.top();
        cs.pop();
        assert(c2);
        
        // finished to process chord: add config2 to queue of successors
        if (c2->complete())
            q.push(c2); // copy of shared ptr
        // continue processing of the chord
        else
        {
            unsigned int m = c2->current(); // chroma in 0..11
            assert(0 <= m);
            assert(m < 12);

            assert(c2);
            const PSC2& rc2 = *c2;
            
            // 3 enharmonics
            for (int j = 0; j < 3; ++j)
            {
                NoteName name = Enharmonics::name(m, j);
                Accid accid = Enharmonics::accid(m, j);
                // case of 8 and (short list) 1, 3, 6, 10
                if (defined(name) && defined(accid) &&
                    rc2.consistent(name, accid))
                {
                    cs.push(std::make_shared<PSC2>(rc2, name, accid, ton));
                }
            }
        }
    }
}


// static
void PSB::succ(std::shared_ptr<const PSC0>& c, // PSEnum& e,
               const Ton& ton, const Ton& lton, PSCQueue& q) const
{
    assert(c);
    if (_enum.simultaneous(c->id()))
        succ2(c, ton, lton, q);    // chord
    else
        succ1(c, ton, lton, q);    // single note
}


// static
void PSB::succ1(std::shared_ptr<const PSC0>& c, // PSEnum& e,
                const Ton& ton, const Ton& lton, PSCQueue& q) const
{
    assert(c);
    // midi pitch of the note read for transition from this config
    unsigned int pm = _enum.midipitch(c->id());
    assert(0 <= pm);
    assert(pm <= 127);
    int m = pm % 12; // chroma in 0..11

    for (int j = 0; j < 3; ++j)
    {
        NoteName name = Enharmonics::name(m, j);
        Accid accid = Enharmonics::accid(m, j);
        // case of 8 and (short list) 1, 3, 6, 10
        if (defined(name) && defined(accid))
            q.push(std::make_shared<PSC1>(c, _enum, name, accid, ton, lton));
    }
}


// static
void PSB::succ2(std::shared_ptr<const PSC0>& c, // PSEnum& e,
                const Ton& ton, const Ton& lton, PSCQueue& q) const
{
    assert(c);
    //assert(c->size() > 1);
    std::stack<std::shared_ptr<const PSC2>> cs;
    cs.push(std::make_shared<const PSC2>(c, _enum, c->id()));     // initial config
    
    while (! cs.empty())
    {
        std::shared_ptr<const PSC2> c2 = cs.top();
        cs.pop();
        assert(c2);
        if (c2->complete())
            q.push(c2);
        else
        {
            unsigned int m = c2->current(); // chroma in 0..11
            assert(0 <= m);
            assert(m < 12);

            assert(c2);
            const PSC2& rc2 = *c2;
            
            // 3 enharmonics
            for (int j = 0; j < 3; ++j)
            {
                NoteName name = Enharmonics::name(m, j);
                Accid accid = Enharmonics::accid(m, j);
                // case of 8 and (short list) 1, 3, 6, 10
                if (defined(name) && defined(accid) &&
                    rc2.consistent(name, accid))
                    cs.push(std::make_shared<PSC2>(rc2, name, accid, ton, lton));
            }
        }
    }
}


} // end namespace pse

/// @}

