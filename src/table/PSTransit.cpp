//
//  PSTransit.cpp
//  pse
//
//  Created by Florent on 11/05/2023.
//

#include "PSTransit.hpp"
#include "Enharmonic.hpp"
//#include "PSOrder.hpp"

namespace pse {

Transition::Transition(const Algo& a, PSEnum& e):
_algo(a),
_enum(e)
{ }


void Transition::succ(std::shared_ptr<const PSC0> c,
                      const Ton& ton, const Ton& lton,
                      PSCQueue& q) const
{
    assert(c);
    if (_enum.simultaneous(c->id()))
        succ2(c, ton, lton, q);    // chord
    else
        succ1(c, ton, lton, q);    // single note
}


void Transition::succ1(std::shared_ptr<const PSC0> c,
                       const Ton& gton, const Ton& lton,
                       PSCQueue& q) const
{
    assert(c);
    // midi pitch of the note read for transition from this config
    unsigned int pm = _enum.midipitch(c->id());
    assert(MidiNum::check_midi(pm)); // assert(0 <= pm); assert(pm <= 127);
    // chroma in 0..11
    int m = pm % 12;
    
    // 3 potential successors in algo PSE
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

            /// @todo merge into 1 constructor PSC1, with algo name
            assert(accid == MidiNum::accid(m, name));
            // no force print
            q.push(std::make_shared<PSC1>(c, _enum, name, accid, false,
                                          gton, lton));
//            if (_algo == Algo::PSE0) // lton is ignored
//                q.push(std::make_shared<PSC1>(c, _enum, name, accid, ton));
//            else if (_algo == Algo::PSE1)
//                q.push(std::make_shared<PSC1>(c, _enum, name, accid,
//                                              ton, lton));
        }
    }
    // only 1 potential successor in algo PS14
    else if (_algo == Algo::PS14)
    {
        const Scale& scale = gton.chromatic();
        int p = scale.pitchClass(0); // pitch class of tonic of scale
        // degree of m in the chromatic harmonic scale of p
        size_t deg = (p <= m)?(m - p):(12-p+m);
        assert(0 <= deg); // debug
        assert(deg < 12);
        enum NoteName name = scale.name(deg);
        enum Accid accid = scale.accid(deg);
        assert(defined(name));
        assert(defined(accid));
        assert(accid == MidiNum::accid(m, name));
        // lton may be ignored
        // no force print
        q.push(std::make_shared<PSC1>(c, _enum, name, accid, false,
                                      gton, lton));
    }
    else
    {
        ERROR ("Transition succ1: unexpected algo name {}", _algo);
    }
}

using PSC1cCompare = std::function<bool(std::shared_ptr<const PSC1c>&,
                                        std::shared_ptr<const PSC1c>&)>;

typedef std::priority_queue<std::shared_ptr<const PSC1c>,
                            std::vector<std::shared_ptr<const PSC1c>>,
                            PSC1cCompare> PSC1cQueue;

// we loop on the chord, with a auxiliary queue
void Transition::succ2(std::shared_ptr<const PSC0> c,
                       const Ton& gton, const Ton& lton, PSCQueue& q) const
{
    assert(c);    //assert(c->size() > 1);
    const PSC0& c0 = *c;
    assert(_enum.simultaneous(c0.id())); // we are at the beginning of a chord
    PSChord chord(_enum, c0.id());
    DEBUGU("CHORD of size {} at {} (current cost:{})",
           chord.size(), chord.first(), c0.cost());
    //std::cin.get();  //system("read"); // Press any key to continue...

    // two pairs of parentheses to declare cs as a variable.
    // std::priority_queue<std::shared_ptr<const PSC1c>,
    //                     std::vector<std::shared_ptr<const PSC1c>>,
    //                     PSCCompare> cs((PSClex()));
    // we store partial chord renaming in a stack (no need of an ordered queue)
    //std::stack<std::shared_ptr<const PSC1c>> cs;
    PSC1cQueue cs = PSC1cQueue(PSClexc());
    
    // initial config (beginning of chord
    // initial config (beginning of chord
    cs.push(std::make_shared<const PSC1c>(c, chord));

    while (! cs.empty())
    {
        std::shared_ptr<const PSC1c> c1 = cs.top();
        cs.pop();
        assert(c1);
        
        // the chord has been fully processed
        if (c1->complete())
        {
            DEBUGU("spelling chord[{},{}] of cost: {}",
                   chord.size(), chord.first(), c1->cost());
            // build c2
            // std::shared_ptr<const PSC2> c2(new PSC2(c, c1.get(), chord));
            q.push(std::make_shared<const PSC2>(c, c1, chord));
            continue;
        }
        
        // otherwise continue processing of the chord
        unsigned int m = chord.midipitch(c1->id()) % 12; // chroma in 0..11

        // not undef if the current pitch class has already been processed in chord
        const enum NoteName dejaname = c1->dejavu(m);
        // in this case, we resuse the previous name chosen for the pitch class
        if (dejaname != NoteName::Undef)
        {
            cs.push(std::make_shared<PSC1c>(c1, chord,
                                            dejaname,
                                            MidiNum::accid(m, dejaname),
                                            c1->dejaprint(m), // force print
                                            gton, lton));
        }
        // 3 enharmonics = 3 potential successors in algo PSE
        else if (_algo == Algo::PSE)
        {
            for (int j = 0; j < 3; ++j)
            {
                enum NoteName name = Enharmonics::name(m, j);
                enum Accid accid = Enharmonics::accid(m, j);
                // case of 8 and (short list) 1, 3, 6, 10
                if (defined(name) && defined(accid))
                {
                    assert(accid == MidiNum::accid(m, name));
                    cs.push(std::make_shared<PSC1c>(c1, chord,
                                                    name, accid, false, // do not force print
                                                    gton, lton));
                }
            }
        }
        // only 1 potential successor in algo PS14
        else if (_algo == Algo::PS14)
        {
            // only 1 potential successor in algo PS14
            // lton is ignored
            const Scale& scale = gton.chromatic();
            int p = scale.pitchClass(0); // pitch class of tonic of scale
            // degree of m in the chromatic harmonic scale of p
            size_t deg = (p <= m)?(m - p):(12-p+m);
            assert(0 <= deg); // debug
            assert(deg < 12);
            enum NoteName name = scale.name(deg);
            enum Accid accid = scale.accid(deg);
            assert(defined(name));
            assert(defined(accid));
            assert(accid == MidiNum::accid(m, name));
            cs.push(std::make_shared<PSC1c>(c1, chord,
                                            name, accid, false, // do not force print
                                            gton, lton));
        }
    }
}


// old version off succ2
//void Transition::succ2_TBR(std::shared_ptr<const PSC0> c,
//                       const Ton& ton, const Ton& lton, PSCQueue& q) const
//{
//    assert(c);
//    //assert(c->size() > 1);
//    std::stack<std::shared_ptr<const PSC2>> cs;
//    cs.push(std::make_shared<const PSC2>(c, _enum, c->id())); // initial config
//
//    while (! cs.empty())
//    {
//        std::shared_ptr<const PSC2> c2 = cs.top();
//        cs.pop();
//        assert(c2);
//
//        // finished to process chord: add config2 to queue of successors
//        if (c2->complete())
//        {
//            q.push(c2); // copy of shared ptr
//            continue;
//        }
//
//        /// @todo ALT:
//        /// construire un PSB contenant le chemin pour toutes les étapes
//        /// de l'accord, partant de c.
//        /// -> PSB sur: enum, c.id(), id premiere note apres accord.
//        /// opt: algo_name specifique pour traiter un accord ?
//
//        // otherwise continue processing of the chord
//        unsigned int m = c2->current(); // chroma in 0..11
//        assert(0 <= m);
//        assert(m < 12);
//        const PSC2& rc2 = *c2;
//
//        // 3 enharmonics = potential successors in algo PSE
//        if ((_algo == Algo::PSE0) || (_algo == Algo::PSE1))
//        {
//            for (int j = 0; j < 3; ++j)
//            {
//                enum NoteName name = Enharmonics::name(m, j);
//                enum Accid accid = Enharmonics::accid(m, j);
//                // case of 8 and (short list) 1, 3, 6, 10
//                if (defined(name) && defined(accid) &&
//                    rc2.consistent(name, accid))
//                {
//                    /// @todo merge into 1 constructor PSC2, with algo name
//                    assert(accid == MidiNum::accid(m, name));
//                    if (_algo == Algo::PSE0)
//                        cs.push(std::make_shared<PSC2>(rc2, _enum, name, accid,
//                                                       ton));
//                    if (_algo == Algo::PSE1)
//                        cs.push(std::make_shared<PSC2>(rc2, _enum, name, accid,
//                                                       ton, lton));
//                }
//            }
//        }
//        // only 1 potential successor in algo PS14
//        else if (_algo == Algo::PS14)
//        {
//            // only 1 potential successor in algo PS14
//            // lton is ignored
//            const Scale& scale = ton.chromatic();
//            int p = scale.pitchClass(0); // pitch class of tonic of scale
//            // degree of m in the chromatic harmonic scale of p
//            size_t deg = (p <= m)?(m - p):(12-p+m);
//            assert(0 <= deg); // debug
//            assert(deg < 12);
//            enum NoteName name = scale.name(deg);
//            enum Accid accid = scale.accid(deg);
//            assert(defined(name));
//            assert(defined(accid));
//            assert(accid == MidiNum::accid(m, name));
//            cs.push(std::make_shared<PSC2>(rc2, _enum, name, accid, ton));
//        }
//    }
//}



} // end namespace pse
