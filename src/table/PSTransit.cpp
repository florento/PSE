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


void Transition::succ(std::shared_ptr<const PSC0>& c,
                      const Ton& ton, const Ton& lton,
                      PSCQueue& q) const
{
    assert(c);
    if (_enum.simultaneous(c->id()))
        succ2(c, ton, lton, q);    // chord
    else
        succ1(c, ton, lton, q);    // single note
}


void Transition::succ1(std::shared_ptr<const PSC0>& c,
                       const Ton& ton, const Ton& lton,
                       PSCQueue& q) const
{
    assert(c);
    // midi pitch of the note read for transition from this config
    unsigned int pm = _enum.midipitch(c->id());
    assert(0 <= pm);
    assert(pm <= 127);
    // chroma in 0..11
    int m = pm % 12;
    
    // 3 potential successors in algo PSE
    if ((_algo == Algo::PSE0) || (_algo == Algo::PSE1))
    {
        for (int j = 0; j < 3; ++j)
        {
            enum NoteName name = Enharmonics::name(m, j);
            enum Accid accid = Enharmonics::accid(m, j);
            // case of 8 and (short list) 1, 3, 6, 10
            if (! defined(name) || !defined(accid))
                continue;
            /// @todo merge into 1 constructor PSC1, with algo name
            if (_algo == Algo::PSE0) // lton is ignored
                q.push(std::make_shared<PSC1>(c, _enum, name, accid, ton));
            else if (_algo == Algo::PSE1)
                q.push(std::make_shared<PSC1>(c, _enum, name, accid,
                                              ton, lton));
        }
    }
    // only 1 potential successor in algo PS14
    else if (_algo == Algo::PS14)
    {
        const Scale& scale = ton.chromatic();
        int p = scale.pitchClass(0); // pitch class of tonic of scale
        // degree of m in the chromatic harmonic scale of p
        size_t deg = (p <= m)?(m - p):(12-p+m);
        assert(0 <= deg); // debug
        assert(deg < 12);
        enum NoteName name = scale.name(deg);
        enum Accid accid = scale.accid(deg);
        assert(defined(name));
        assert(defined(accid));
        // lton is ignored
        q.push(std::make_shared<PSC1>(c, _enum, name, accid, ton));
    }
    else
    {
        ERROR ("Transition succ1: unexpected algo name {}", _algo);
    }
}


// static
void Transition::succ2(std::shared_ptr<const PSC0>& c,
                       const Ton& ton, const Ton& lton,
                       PSCQueue& q) const
{
    assert(c);
    //assert(c->size() > 1);
    std::stack<std::shared_ptr<const PSC2>> cs;
    cs.push(std::make_shared<const PSC2>(c, _enum, c->id())); // initial config
    
    while (! cs.empty())
    {
        std::shared_ptr<const PSC2> c2 = cs.top();
        cs.pop();
        assert(c2);
        
        // finished to process chord: add config2 to queue of successors
        if (c2->complete())
        {
            q.push(c2); // copy of shared ptr
            continue;
        }
        // otherwise continue processing of the chord
        unsigned int m = c2->current(); // chroma in 0..11
        assert(0 <= m);
        assert(m < 12);
        const PSC2& rc2 = *c2;

        // 3 enharmonics = potential successors in algo PSE
        if ((_algo == Algo::PSE0) || (_algo == Algo::PSE1))
        {
            for (int j = 0; j < 3; ++j)
            {
                enum NoteName name = Enharmonics::name(m, j);
                enum Accid accid = Enharmonics::accid(m, j);
                // case of 8 and (short list) 1, 3, 6, 10
                if (defined(name) && defined(accid) &&
                    rc2.consistent(name, accid))
                {
                    /// @todo merge into 1 constructor PSC2, with algo name
                    if (_algo == Algo::PSE0)
                        cs.push(std::make_shared<PSC2>(rc2, name, accid,
                                                       ton));
                    if (_algo == Algo::PSE1)
                        cs.push(std::make_shared<PSC2>(rc2, name, accid,
                                                       ton, lton));
                }
            }
        }
        // only 1 potential successor in algo PS14
        else if (_algo == Algo::PS14)
        {
            // only 1 potential successor in algo PS14
            // lton is ignored
            const Scale& scale = ton.chromatic();
            int p = scale.pitchClass(0); // pitch class of tonic of scale
            // degree of m in the chromatic harmonic scale of p
            size_t deg = (p <= m)?(m - p):(12-p+m);
            assert(0 <= deg); // debug
            assert(deg < 12);
            enum NoteName name = scale.name(deg);
            enum Accid accid = scale.accid(deg);
            assert(defined(name));
            assert(defined(accid));
            cs.push(std::make_shared<PSC2>(rc2, name, accid, ton));
        }
    }
}


} // end namespace pse
