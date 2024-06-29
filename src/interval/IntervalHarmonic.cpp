//
//  Interval.cpp
//  pse
//
//  Created by Florent Jacquemard on 12/02/2023.
//

#include "IntervalHarmonic.hpp"

namespace pse {

HarmonicInterval::HarmonicInterval(const Pitch& p1, const Pitch& p2):
SimpleInterval(), // undef
_ascending(p1 <= p2),
_dist(MidiDist(p1, p2)),
_octaves(_dist/12)
{

    // unpitched
    if (p1.undef() || p2.undef())
    {
        // none unpitched or both unpitched
        assert(p1.undef() == p2.undef());
        WARN("Interval between 2 unpitched notes (perfect unisson)");
        _nb = 1; // unison
        _quality = Quality::Perfect;
        _cost = 0;
        return;
    }
    // unnamed
    if (!p1.named() || !p2.named())
    {
        // none unnamed or both unnamed
        //assert((p1.name == NoteName::Undef) == (p2.name == NoteName::Undef));
        assert(p1.named() == p2.named());
        ERROR("Interval between 2 unnamed notes {} {}", p1.midi(), p2.midi());
        // default values nb, quality, cost
        return;
    }
    init_name(p1, p2);
    init_quality();
    assert(1 <= _nb && _nb <= 8); // unison to octave
    assert(_quality != Quality::Undef);
    assert(0 <= _cost && _cost <= 2);
}


unsigned int HarmonicInterval::MidiDist(const Pitch& p1, const Pitch& p2)
{
    if (p1.undef())
    {
        /// none unpitched or both unpitched
        assert(p2.undef());
        return 0;
    }
    else
        return std::abs(((int) p1.midi()) - ((int) p2.midi()));
}


void HarmonicInterval::init_name(const Pitch& p1, const Pitch& p2)
{
    assert(p1.named() && p2.named());
    int n1;
    int n2;
    if (_ascending)
    {
        n1 = toint(p1.name);
        n2 = toint(p2.name);
    }
    else
    {
        n1 = toint(p2.name);
        n2 = toint(p1.name);
    }
    assert(0 <= n1 && n1 <= 6);
    assert(0 <= n2 && n2 <= 6);
    if (n1 < n2)
    {
        _nb = abs(n2 - n1) + 1;
    }
    else if (n1 > n2)
    {
        _nb = abs(n2 - (n1 - 7)) + 1;
    }
    else
    {
        assert(n1 == n2);  // did we miss something?
        if (_octaves == 0) // distance is less than 1 octave
            _nb = 1;       // unison
        else
        {
            _nb = 8;       // octave(s)
            _octaves -= 1;
        }
    }
    assert(1 <= _nb && _nb <= 8); // unison to octave
}


void HarmonicInterval::init_quality()
{
    assert(1 <= _nb && _nb <= 8);
    assert(_dist >= 0);
    size_t d = 15;
    if (_nb < 7)
    {
        d = _dist % 12;
    }
    else if (_dist % 12 < 3) // 7th and 8ve with 12, 13 or 14 semitones
    {
        d = 12 + _dist % 12;
    }
    else
    {
        ERROR("Interval: interval {} incompatible with dist={}", _nb, _dist);
        return;
    }
    assert(d < 15);
    bool notfound = true;
    for (auto i = INTERVALS[d].cbegin(); i < INTERVALS[d].cend(); ++i)
    {
        const SimpleInterval& si = *i;
        if (_nb == si.number())
        {
            _quality = si.quality();
            _cost = si.cost();
            notfound = false;
            break;
        }
    }
    if (notfound)
    {
        ERROR("Interval: case missing for dist={} ({} 1/2tons), nb={}",
              _dist, d, _nb);
    }
}


void HarmonicInterval::print(std::ostream& o) const
{
    o << _nb << tostring(_quality);
}


std::ostream& operator<<(std::ostream& o, const HarmonicInterval& i)
{
    i.print(o);
    return o;
}


//void Interval::init_quality()
//{
//    if (_dist == 0)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Perfect;
//                _cost = 0;
//                break;
//            case 2:
//                _quality = Quality::Diminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                // default quality and cost
//                break;
//        }
//    }
//    else if (_dist == 1)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 2)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 3)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 4)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 5)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 6)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 7)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 8)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 9)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 10)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 11)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 12)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 13)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else if (_dist == 14)
//    {
//        switch (_nb)
//        {
//            case 1:
//                _quality = Quality::Augmented;
//                _cost = 2;
//                break;
//            case 2:
//                _quality = Quality::Minor;
//                _cost = 0;
//                break;
//            case 3:
//                _quality = Quality::SubDiminished;
//                _cost = 2;
//                break;
//            default:
//                ERROR("Interval: cannot set quality of {} for {} halftones",
//                      _nb, _dist);
//                _quality = Quality::Undef;
//                break;
//        }
//    }
//    else
//    {
//        ERROR("Interval: unexpected distance of {} halftones", _dist);
//    }
//}


} // end namespace pse
