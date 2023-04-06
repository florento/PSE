//
//  PSPath.cpp
//  squant2
//
//  Created by Florent Jacquemard on 16/02/2021.
//  Copyright © 2021 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{

#include "PSPath.hpp"
#include "MidiNum.hpp"


namespace pse {


PSP::PSP(const PSC0& psc, PSEnum& e):
_enum(e),
_names(),   // initially empty
_accids(),
_prints(),
_cost(psc.cost()),
_computed(false)
{
    TRACE("PSP: computing best path for {}-{}", e.first(), e.stop());

    // empty sequence of notes
    if (e.first() == e.stop())
        return; // nothing to do
    
    assert(e.first() < e.stop());
    //init(ton, lton);
    record_path(psc);
}


// @todo TBR obsolete
//bool PSP::init(const Ton& ton, const Ton& lton)
//{
//    // recompute best path with ton and lton
//    const PSB psb(ton, lton, _enum);
//    if (psb.empty())
//    {
//        ERROR("PSP {}-{}: no best path found", _enum.first(), _enum.stop());
//        return false;
//    }
//    else if (psb.size() > 1)
//    {
//        WARN("PSP {}-{}: tie break fail ({} bests)",
//             _enum.first(), _enum.stop(), psb.size());
//    }
//    const PSC0& psc = psb.top();
//    record_path(psc);
//    return true;
//}


PSP::~PSP()
{
    TRACE("delete PS Path {}-{}", _enum.first(), _enum.stop());
  
    _names.clear();
    _accids.clear();
    _prints.clear();
}


const enum NoteName& PSP::name(size_t i) const
{
    assert(_enum.inside(i));
    //if (_names.empty()) init();
    assert((i - _enum.first()) < _names.size());
    return _names[i - _enum.first()];
}


const enum Accid& PSP::alteration(size_t i) const
{
    // if (_accids.empty()) init();
    assert(_enum.inside(i));
    assert(i - _enum.first() < _accids.size());
    return _accids[i - _enum.first()];
}


bool PSP::printed(size_t i) const
{
    // if (_prints.empty()) init();
    assert(_enum.inside(i));
    assert(i - _enum.first() < _prints.size());
    return _prints[i - _enum.first()];
}


//unsigned int PSP::cost() const
//{
//    return _cost;
//}


void PSP::record_path(const PSC0& c)
{
    assert(c.id() == _enum.stop());
    assert(_cost == c.cost());
    const PSC0* co = &c;
    assert(co);
    assert(co->initial() || co->fromNote() || co->fromChord());
    while (! co->initial())
    {
        if (co->fromNote())
        {
            const PSC1* com = dynamic_cast<const PSC1*>(co);
            assert(com);
            _names.insert(_names.begin(), com->name());  // push_front (copy)
            _accids.insert(_accids.begin(), com->accidental());
            _prints.insert(_prints.begin(), com->printed());
        }
        else
        {
            assert(co->fromChord());
            const PSC2* com = dynamic_cast<const PSC2*>(co);
            assert(com);
            // assert(com->size() > 1);
            std::vector<enum Accid> accids;
            for (size_t i = 0; i < com->size(); ++i)
                accids.push_back(com->accidental(i));

            _names.insert(_names.begin(), com->cbeginName(), com->cendName());
            _accids.insert(_accids.begin(), accids.cbegin(), accids.cend());
            _prints.insert(_prints.begin(), com->cbeginPrint(), com->cendPrint());
        }
        
        assert(co->previous());
        co = co->previous(); // NULL if co is initial
        assert(co);
        assert(co->initial() || co->fromNote() || co->fromChord());
    }
    
    assert(_names.size()  == _enum.size()); // number of notes
    assert(_accids.size() == _enum.size());
    assert(_prints.size() == _enum.size());
}


void PSP::rename()
{
    // if (_names.empty()) return;
    assert(_enum.first() <= _enum.stop());
    assert((_enum.stop() - _enum.first()) <= _names.size());
    
    for (size_t i = _enum.first(); i < _enum.stop(); ++i)
    {
        const enum NoteName& name = _names[i - _enum.first()];
        const enum Accid& accid = _accids[i - _enum.first()];
        unsigned int mp = _enum.midipitch(i);
        int oct = MidiNum::midi_to_octave(mp, name, accid);
        assert(-2 <= oct);
        assert(oct <= 9);
        bool altprint = _prints[i - _enum.first()];
        //std::cout << "enum: rename" << i << std::endl;
        _enum.rename(i, name, accid, oct, altprint);
        TRACE("PSP.rename: note {} : {}{}{} {}", i, name, oct, accid, altprint);
    }
}


//void PSP::init(const PSB& psb)
//{
//    std::vector<std::stack<const PSC0*>> stacks; // empty
//}


//void PSP::init()
//{
//    // heap of candidate configs for the best path search (incomplete paths)
//    PSCQueue queue1(PSCacc); // empty
//
//    // initial configuration
//    queue1.push(std::make_shared<const PSC0>(_ton, first()));
//
//    // heap of complete paths with minimal number of accidents
//    PSCQueue queue2(PSCdist); // empty
//
//    while (! queue1.empty())
//    {
//        std::shared_ptr<const PSC0> c = queue1.top(); // smallest
//        assert(c);
//        assert(first() <= c->id());
//        assert(c->id() <= stop());
//        _visited.push_back(c); // keep c for deletion
//        queue1.pop(); // remove c
//
//        // the path c is complete
//        if (c->id() == stop())
//        {
//            // first minimal complete path found
//            if (queue2.empty())
//            {
//                queue2.push(c);
//                _cost = c->cost();
//            }
//            else
//            {
//                // c is a minimal complete path
//                if (_cost == c->cost())
//                {
//                    queue2.push(c);
//                }
//                // we have found all the minimal complete paths
//                else
//                {
//                    assert(_cost < c->cost());
//                    break;
//                    //init2(queue2);
//                    //return;
//                }
//            }
//        }
//        // complete the path of c with its successors
//        else
//        {
//            Note* n = note(c->id());
//            assert(n);
//            // add every possible successor configs to queue1
//            c->succ(n->pitch(), _ton, queue1);
//
//           // add all successors of c to queue
// //           std::vector<std::shared_ptr<PSConfig>> cv = c->successors(n->pitch(), _ton);
// //           while(! cv.empty())
// //           {
// //               assert(cv.back());
// //               queue1.push(cv.back());
// //               cv.pop_back();
// //           }
//        }
//    }
//
//}


//void PSP::init2(PSCQueue& queue2)
//{
//    // there is at least one minimal path
//    assert(! queue2.empty());
//    std::shared_ptr<const PSC0> c = queue2.top(); // smallest distance
//    assert(c);
//    assert(c->id() == stop()+1);
//    assert(c->cost() == _cost);
//    record_path(c);
//}


//void PSP::complete()
//{
//    while (_names.empty())
//    {
//        assert(! _queue1.empty());
//        std::shared_ptr<const PSConfig> c = _queue1.top(); // smallest
//        assert(c);
//        assert(first() <= c->id());
//        assert(c->id() <= stop()+1);
//        if (c->id() == stop()+1)
//        {
//            record_path(c);
//            return;
//        }
//        
//        Note* n = note(c->id());
//        assert(n);
//        std::vector<std::shared_ptr<PSConfig>> cv =
//                                               c->successors(n->pitch(), _ton);
//        _visited.push_back(c); // keep c for deletion
//        _queue1.pop(); // remove
//        // add all successors pv c to queue
//        while(! cv.empty())
//        {
//            assert(cv.back());
//            _queue1.push(cv.back());
//            cv.pop_back();
//        }
//    }
//}


} // end namespace pse

/// @}

