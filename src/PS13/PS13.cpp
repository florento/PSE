//
//  PS13.cpp
//  pse
//
//  Created by Florent Jacquemard on 29/03/2023.
//

#include "PS13.hpp"


namespace pse {


PS13::PS13(size_t kpre, size_t kpost, bool dflag):
Speller(Algo::PS13, dflag),
_scales(),       // empty vector
_Kpre(kpre),
_Kpost(kpost),
_global()        // Undef TOn
{
    init_scales();
}


PS13::~PS13()
{ }


//Algo PS13::algo() const
//{
//    return Algo::PS13;
//}


void PS13::init_scales()
{
    assert(_scales.empty());
    const std::array<const enum NoteName, 12> names{
        NoteName::C,
        NoteName::D, NoteName::D,
        NoteName::E, NoteName::E, NoteName::F, NoteName::F,
        NoteName::G,
        NoteName::A, NoteName::A, NoteName::B, NoteName::B };

    //ModeFactory mf;
    //std::shared_ptr<Mode> pm = mf.make(ModeName::Chromatic);
    //assert(pm);
    const Mode m(ModeName::Chromatic);

    // for every pitch class
    for (int c = 0; c < 12; ++c)
    {
        // mode, pitch-class, name of tonic
        _scales.emplace_back(m, c, names[c]);
    }
    // associate a Ton to each scale ?
}


bool PS13::spell()
{
    // step 1: name all notes in enumerator
    for (size_t n = _enum.first(); n < _enum.stop(); ++n)
    {
        // pitch class of n
        int nm = _enum.midipitch(n);
        assert(0 <= nm);
        assert(nm <= 128);
        unsigned int nc = nm % 12;

        // counter for each candidate name for note n
        std::array<size_t, 7> nname;
        nname.fill(0);
        int maxi = -1; // name in 0..6 with maximal count
        enum NoteName maxName = NoteName::Undef;
        
        // for all pitch class
        for (int p = 0; p < 11; ++p)
        {
            
            // degree of n in the chromatic harmonic scale of p
            size_t deg = (p <= nc)?(nc - p):(12-p+nc);
            assert(0 <= deg); // debugÅ
            assert(deg < 12);
            // name of n in chromatic harmonic scale of p
            const enum NoteName nn = _scales[p].name(deg);
            assert(nn != NoteName::Undef);
            int nni = toint(nn);
            assert(0 <= nni);
            assert(nni < 7);
            //size_t cpn = count(p, n);
            nname[nni] += count(p, n);
            if (maxi == -1 || nname[nni] > nname[maxi])
            {
                maxi = nni;
                maxName = nn;
            }
        }
        // set name of n to maxi
        assert(0 <= maxi);
        assert(maxi < 7);
        assert(maxName != NoteName::Undef);
        _enum.rename(n, maxName);
    }
    
    return true;
}


size_t PS13::count(int c, size_t n) const
{
    return _enum.count(c, n, _Kpre, _Kpost);
//    assert(0 <= c);
//    assert(c < 12);
//    assert(! _enum.open());
//    assert(! _enum.empty());
//    size_t efirst = _enum.first();
//    size_t estop = _enum.stop();
//    assert(efirst <= n);
//    assert(n <= _enum.stop());
//    size_t left = (n - efirst >= _Kpre)?(n - _Kpre):efirst;
//    size_t right = (estop - n >= _Kpost)?(n + _Kpost):estop;
//    assert(left <= right);
//    unsigned int cpt = 0;
//    for (size_t i = left; i < right; ++i)
//    {
//        assert(efirst <= i);
//        assert(i < estop);
//        unsigned int mp = _enum.midipitch(i);
//        assert(0 <= mp);
//        assert(mp <= 128);
//        if (mp%12 == c)
//            ++cpt;
//    }
//
//    return cpt;
}


const Ton& PS13::global() const
{
    return _global; // Undef
}


//const Ton& PS13::local(size_t i) const
//{
//    return _global; // Undef
//}


} // namespace pse
