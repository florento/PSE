//
//  TonIndex.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/02/2023.
//

#include <utility>      // std::pair, std::make_pair
#include <algorithm>    // std::sort

#include "TonIndex.hpp"
#include "PSTable.hpp"


namespace pse {

const size_t TonIndex::MAXTONS = 900;

const size_t TonIndex::UNDEF  = MAXTONS+1;

const size_t TonIndex::FAILED = MAXTONS+2;

TonIndex::TonIndex(size_t n):
_tons(),    // vector initially empty
_undef(),   // undef ton
_closed(false),
_repr_tonal(),
_repr_modal(),
// _WeberTonal(true)
_WeberModal(false),
_WeberBluesModal(false),
_rankWeber()
{
    init(n);
}

TonIndex::~TonIndex()
{
    _tons.clear();
}


size_t TonIndex::size() const
{
    return _tons.size();
}


bool TonIndex::empty() const
{
    return _tons.empty();
}


const Ton& TonIndex::ton(size_t i) const
{
    if (i == UNDEF)
    {
        assert(_undef.undef());
        return _undef;
    }
    else
    {
        assert(i < _tons.size());
        return _tons.at(i).first;
    }
}


const Ton& TonIndex::undef() const
{
    assert(_undef.undef());
    return _undef;
}


size_t TonIndex::find(const Ton& ton) const
{
    for (size_t i = 0; i < _tons.size(); ++i)
        if (_tons.at(i).first == ton)
            return i;

    ERROR("TonIndex find: {} not found", ton);
    return UNDEF;
}


size_t TonIndex::find(int ks, const ModeName& mode) const
{
    assert(-7 <= ks);
    assert(ks <= 7);
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        const Ton& toni = _tons.at(i).first;
        if (toni.fifths() == ks && toni.getMode() == mode)
            return i;
    }
    // not found
    return UNDEF;
}


size_t TonIndex::irepresentative(size_t i, bool tonal) const
{
    if (tonal)
    {
        assert(i < _repr_tonal.size());
        assert(_repr_tonal.at(i) < _tons.size());
        return _repr_tonal.at(i);
    }
    else
    {
        assert(i < _repr_modal.size());
        assert(_repr_modal.at(i) < _tons.size());
        return _repr_modal.at(i);
    }
}


const Ton& TonIndex::representative(size_t i, bool tonal) const
{
    size_t j = irepresentative(i, tonal);
    assert(j < _tons.size());
    return _tons.at(j).first;
}


size_t TonIndex::enharmonic(size_t i) const
{
    assert(i < _tons.size());
    const Ton& toni = _tons.at(i).first;
    int ks = toni.fifths();
    assert(-7 <= ks);
    assert(ks <= 7);
    int eks = ks; // key signature of enharmonic
    switch (ks)
    {
        case 7:
            eks = -5;
            break;
        case 6:
            eks = -6;
            break;
        case 5:
            eks = -7;
            break;
        case -5:
            eks = 7;
            break;
        case -6:
            eks = 6;
            break;
        case -7:
            eks = 5;
            break;
        default:
            eks = ks;
    }

    return find(eks, toni.getMode());
}


void TonIndex::reset(size_t n)
{
    TRACE("TonIndex: empty the list of tonalities (row headers)");
    _tons.clear();
    _closed = false;
    _WeberModal = false;
    _WeberBluesModal = false;
    init(n);
}


void TonIndex::add(const Ton& ton, bool global)
{
    assert(_tons.size() == _repr_modal.size());
    assert(_tons.size() == _repr_tonal.size());
    if (_closed)
    {
        ERROR("TonIndex add: array of tons is closed");
        return;
    }
    
    // add the ton to this array
    if (_tons.size() < MAXTONS)
    {
        _tons.push_back(std::make_pair(ton, global)); // copy
    }
    else
    {
        ERROR("TonIndex add: array of tons overfull");
        return;
    }
    
    // update flags Weber
    switch (ton.getMode())
    {
        // table of Weber
        case ModeName::Major:
        case ModeName::Minor:
            break;
            
        // table of WeberModal
        case ModeName::MinorMel:
        case ModeName::Ionian:
        case ModeName::Dorian:
        case ModeName::Phrygian:
        case ModeName::Lydian:
        case ModeName::Mixolydian:
        case ModeName::MinorNat: // = Aeolian
        case ModeName::Aeolian:
        case ModeName::Locrian:
            _WeberModal = true;
            break;

        // table of WeberBluesModal
        case ModeName::MajorBlues:
        case ModeName::MinorBlues:
            _WeberBluesModal = true;
            break;
                        
        default:
            WARN("ton {} not in Weber table", ton);
            break;
    }
    
    // search and store earlier equivalent tons
    assert( _tons.size() > 0);
    
    // it is the first ton added
    if ( _tons.size() == 1)
    {
        _repr_modal.push_back(0);
        _repr_tonal.push_back(0);
    }
    else
    {
        size_t current = _tons.size() - 1; // index of last ton added to this index
        assert(_tons[current].first == ton);
        assert(_tons[current].second == global);
        bool found_tonal = false;
        bool found_modal = false;
        
        for (size_t i = 0; i < current && (!found_tonal || !found_modal); ++i)
        {
            const Ton& toni = _tons.at(i).first;
            assert(toni != ton); // no duplicates in TonIndex
            if (!found_tonal && ton.equivalent(toni, true))  // tonal equiv
            {
                _repr_tonal.push_back(i);
                found_tonal = true;
            }
            if (!found_modal && ton.equivalent(toni, false)) // modal equiv
            {
                _repr_modal.push_back(i);
                found_modal = true;
            }
        }
        
        if (!found_tonal)
            _repr_tonal.push_back(current); // tonal equivalent is self
        
        if (!found_modal)
            _repr_modal.push_back(current); // modal equivalent is self
    }
}


void TonIndex::add(int ks, const ModeName& mode, bool global)
{
    add(Ton(ks, mode), global);
}


void TonIndex::close()
{
    _closed = true;
    initRankWeber();
}


bool TonIndex::closed() const
{
    return _closed;
}


void TonIndex::init(size_t n)
{
    switch (n)
    {
        case 0:
            // _WeberTonal = false;
            // close() must be called afterwards
            assert(!_WeberModal);
            assert(!_WeberBluesModal);
            break;

        case 24:
            init(-5, 6, ModeName::Major, true); // global flag
            init(-5, 6, ModeName::Minor, true);
            close(); // tonal Weber
            assert(!_WeberModal);
            assert(!_WeberBluesModal);
            break;

        case 25:
            init25();
            close(); // tonal Weber
            assert(!_WeberModal);
            assert(!_WeberBluesModal);
            break;

        case 26:
            init(-6, 6, ModeName::Major, true); // global flag
            init(-6, 6, ModeName::Minor, true);
            close(); // tonal Weber
            assert(!_WeberModal);
            assert(!_WeberBluesModal);
            break;

        case 30:
            init(-7, 7, ModeName::Major, true); // global flag
            init(-7, 7, ModeName::Minor, true);
            close(); // tonal Weber
            assert(!_WeberModal);
            assert(!_WeberBluesModal);
            break;

        case 104:
            // _WeberTonal = false;
            // init13(ModeName::Major);
            init(-6, 6, ModeName::Major, true);
            init(-6, 6, ModeName::Minor, true);
            // init(-6, 6, ModeName::Ionian, true); // Major
            init(-6, 6, ModeName::Dorian, false);
            init(-6, 6, ModeName::Phrygian, false);
            init(-6, 6, ModeName::Lydian, false);
            init(-6, 6, ModeName::Mixolydian, false);
            init(-6, 6, ModeName::Aeolian, false);
            init(-6, 6, ModeName::Locrian, false);
            close(); // modal Weber
            assert(_WeberModal);
            assert(!_WeberBluesModal);
            break;

        case 117:
            // _WeberTonal = false;
            // init13(ModeName::Major);
            init(-6, 6, ModeName::Major, true);
            init(-6, 6, ModeName::Minor, true);
            init(-6, 6, ModeName::MinorMel, false);
            // init(-6, 6, ModeName::Ionian, true); // Major
            init(-6, 6, ModeName::Dorian, false);
            init(-6, 6, ModeName::Phrygian, false);
            init(-6, 6, ModeName::Lydian, false);
            init(-6, 6, ModeName::Mixolydian, false);
            init(-6, 6, ModeName::Aeolian, false);
            init(-6, 6, ModeName::Locrian, false);
            close(); // modal Weber
            assert(_WeberModal);
            assert(!_WeberBluesModal);
            break;
            
        case 135:
            // _WeberTonal = false;
            init(-7, 7, ModeName::Major, true); // Ionian
            init(-7, 7, ModeName::Minor, true);
            init(-7, 7, ModeName::MinorMel, false);
            init(-7, 7, ModeName::Dorian, false);
            init(-7, 7, ModeName::Phrygian, false);
            init(-7, 7, ModeName::Lydian, false);
            init(-7, 7, ModeName::Mixolydian, false);
            init(-7, 7, ModeName::Aeolian, false);
            init(-7, 7, ModeName::Locrian, false);
            close(); // modal Weber
            assert(_WeberModal);
            assert(!_WeberBluesModal);
            break;
            
        case 165:
            // _WeberTonal = false;
            init(-7, 7, ModeName::Major, true); // Ionian
            init(-7, 7, ModeName::Minor, true);
            init(-7, 7, ModeName::MinorMel, false);
            init(-7, 7, ModeName::Dorian, false);
            init(-7, 7, ModeName::Phrygian, false);
            init(-7, 7, ModeName::Lydian, false);
            init(-7, 7, ModeName::Mixolydian, false);
            init(-7, 7, ModeName::Aeolian, false);
            init(-7, 7, ModeName::Locrian, false);
            init(-7, 7, ModeName::MajorBlues, false);
            init(-7, 7, ModeName::MinorBlues, false);
            close(); // modal Weber with Blues
            assert(_WeberBluesModal);
            break;
            
            
        default:
            ERROR("TonIndex: unsupported list of default tons: {}", n);
            break;
    }

}


void TonIndex::init(int ksmin, int ksmax, const ModeName& mode, bool f_global)
{
    assert(-7 <= ksmin);
    assert(ksmin <= 7); // chromatic Ton uses -7 to 10
    assert(-7 <= ksmax);
    assert(ksmax <= 7); // chromatic Ton uses -7 to 10
    assert(ksmin <= ksmax);
    assert(mode != ModeName::Undef);
    
    if (ksmin <= 0 and 0 <= ksmax)
        add(0, mode, f_global);
    for (int ks = 1; ks <= 7; ++ks)
    {
        if (ksmin <= ks and ks <= ksmax)
            add(ks, mode, f_global);
        if (ksmin <= -ks and -ks <= ksmax)
            add(-ks, mode, f_global);
    }
}


//void TonIndex::init13(const ModeName& mode, bool f_global)
//{
//    assert(mode != ModeName::Undef);
//    init(-6, 6, mode, f_global);
//}


//void TonIndex::init15(const ModeName& mode, bool f_global)
//{
//    assert(mode != ModeName::Undef);
//     add(-7, mode, f_global);
//     init13(mode, f_global);
//     add(7, mode, f_global);
//}


void TonIndex::init25()
{
    for (int ks = -4; ks <= 7; ++ks)
        add(ks, ModeName::Major, true);
    for (int ks = -6; ks <= 6; ++ks)
        add(ks, ModeName::Minor, true);
}


// not used
void TonIndex::initmodal()
{
    init(-7, 7, ModeName::Major, true);
    init(-7, 7, ModeName::Minor, true);
    init(-7, 7, ModeName::MinorMel, true);
    init(-7, 7, ModeName::Dorian, true);
    init(-7, 7, ModeName::Phrygian, true);
    init(-7, 7, ModeName::Lydian, true);
    init(-7, 7, ModeName::Mixolydian, true);
    init(-7, 7, ModeName::Aeolian, true);
    init(-7, 7, ModeName::Locrian, true);
}


/// @todo TBR
//void TonIndex::setTonal()
//{
//    assert(!_closed);
//    _WeberTonal = true;
//}


/// @todo TBR
//void TonIndex::setModal()
//{
//    assert(!_closed);
//    _WeberTonal = false;
//}


bool TonIndex::isGlobal(size_t i) const
{
    assert(i != TonIndex::UNDEF);
    assert(i < _tons.size());
    return _tons.at(i).second;
}


size_t TonIndex::globals() const
{
    size_t c = 0;
    for (size_t i = 0; i < _tons.size(); ++i)
        if (isGlobal(i)) c++;
    return c;
}


void TonIndex::setGlobal(size_t i)
{
    assert(i < _tons.size());
    _tons.at(i).second = true;
}


void TonIndex::unsetGlobal(size_t i) 
{
    assert(i < _tons.size());
    _tons.at(i).second = false;
}


size_t TonIndex::getGlobal(size_t n) const
{
    size_t c = 0;
    for (size_t i = 0; i < size(); ++i)
    {
        if (isGlobal(i))
        {
            if (c == n)
                return i; // return the index in Ton Index
            else
                c++;
        }
        // ignore non global
    }
    return TonIndex::UNDEF;
}


size_t TonIndex::selectGlobals(const PST& tab, double d, bool refine)
{
    assert(0 <= d);
    assert(d <= 100);

    // select all tons
    if (d == 100)
    {
        if (refine)
            return 0;
        // select all tons as global
        else
        {
            for (size_t i = 0; i < _tons.size(); ++i)
                setGlobal(i);
            return _tons.size();
        }
    }
    
    // counter of selected
    size_t cpt = 0;
    
    // index of row in tab with best cost cummultated cost
    size_t ibest = TonIndex::UNDEF;
    
    // estimate the best tonality in tab wrt costs (nb accidentals)
    // if refine, restrict the serach to the current globals
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        // skip
        if (refine and !isGlobal(i))
            continue;

        assert(i != TonIndex::FAILED);
        assert(i != TonIndex::UNDEF);
                                
        // new best ton
        if (ibest == TonIndex::UNDEF or tab.rowCost(i) < tab.rowCost(ibest))
        {
            ibest = i;
        }
    }
    
    if (ibest == TonIndex::UNDEF)
    {
        assert(empty() or refine); // refine and no globals
        if (refine)
            WARN("TonIndex selectGlobals: refine mode and no globals");
        return 0;
    }
    
    assert(ibest < _tons.size());
    const Cost& bestCost = tab.rowCost(ibest);
    
    // mark as global (candidates) all tonality
    // at distance to ibest smaller than d
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        assert(i != TonIndex::FAILED);
        assert(i != TonIndex::UNDEF);

        // skip
        if (refine and !isGlobal(i))
            continue;

        const Cost& rc = tab.rowCost(i);

        // real tie
        if (rc == bestCost)
        {
            setGlobal(i);
            cpt++;
        }
        // approx tie
        // we keep this case apart from real tie because of floating approx.
        // (dist is double)
        else if ((d > 0) && (rc.dist(bestCost) <= d))
        {
            setGlobal(i);
            cpt++;
        }
        else
        {
            unsetGlobal(i);
        }
    }
    
    // at least ibest
    assert(isGlobal(ibest));
    return cpt;
}


bool TonIndex::selectGlobal()
{
    size_t ibest = bestGlobal();
    if (ibest == TonIndex::UNDEF)
    {
        WARN("TonIndex selectGlobal: best global not found");
        return false;
    }
    
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        if (ibest == i)
        {
            assert(isGlobal(i));
            // setGlobal(i);
        }
        else
        {
            unsetGlobal(i);
        }

    }
    size_t nbg = globals();
    assert(ibest == TonIndex::UNDEF || nbg == 1);
    assert(ibest != TonIndex::UNDEF || nbg == 0);
    return (ibest != TonIndex::UNDEF);
}


size_t TonIndex::bestGlobal() const
{
    // index of the selected best ton.
    size_t ibest = TonIndex::UNDEF; // out of range.
    
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        if (!isGlobal(i)) // ignore non global
            continue;
            
        // first i
        if (ibest == TonIndex::UNDEF)
        {
            assert(isGlobal(i));
            ibest = i;
        }
        else
        {
            // tie break criteria 1:
            // smallest key signature
            assert(isGlobal(i));
            const Ton& toni = ton(i);
            int fifths = std::abs(toni.fifths());
            assert(ibest != TonIndex::UNDEF);
            assert(isGlobal(ibest));
            const Ton& tonbest = ton(ibest);
            int bestfifth = std::abs(tonbest.fifths());
            if (fifths < bestfifth)
            {
                ibest = i; // new best
            }
            else if (fifths == bestfifth)
            {
                assert(i != ibest);
                // tie break criteria 2:
                // Major, Minor > modes
                if (toni.isMajorMinor() and !tonbest.isMajorMinor())
                {
                    ibest = i; // new best
                }
                else if (toni.isMajorMinor() and
                         tonbest.isMajorMinor())
                {
                    // tie break criteria 5:
                    // Major > Minor
                    if ((toni.getMode() == ModeName::Major) and
                        (tonbest.getMode() == ModeName::Minor))
                    {
                        ibest = i; // new best
                    }
                    // keep the former (smaller) ibest
                    // signal
                    // - different color (same abs)
                    // - uncomparable modes
                    else if ((toni.fifths() != tonbest.fifths()) or
                             (!toni.isMajorMinor() and
                              !tonbest.isMajorMinor()))
                    {
                        WARN("PSGrid: tie break fail {}:{} vs {}:{}",
                             i, toni, ibest, tonbest);
                    }
                    else
                    {
                        DEBUG("PSGrid: tie break fail {}:{} vs {}:{}",
                              i, toni, ibest, tonbest);
                    }
                }
            }
        }
    }
    
    if (ibest == TonIndex::UNDEF)
        ERROR("TonIndex bestGlobal: not found");

    return ibest;
}


// static
/// @todo not used
bool TonIndex::global(const ModeName& m)
{
    switch (m)
    {
        case ModeName::Major:
        case ModeName::Ionian:
        case ModeName::Minor:
            return true;

        case ModeName::MinorNat:
        case ModeName::MinorMel:
            return false;

        case ModeName::Dorian:
        case ModeName::Phrygian:
        case ModeName::Lydian:
        case ModeName::Mixolydian:
        case ModeName::Aeolian:
        case ModeName::Locrian:
        case ModeName::Major5:
        case ModeName::Minor5:
        case ModeName::MajorBlues:
        case ModeName::MinorBlues:
        case ModeName::Augmented:
        case ModeName::Diminished:
        case ModeName::Chromatic:
            return false;

        case ModeName::Undef:
            return false;

        default:
        {
            ERROR("unknown mode name");
            return false;
        }
    }
}


unsigned int TonIndex::distWeber(size_t i, size_t j) const
{
    assert(_closed);
    if (_WeberBluesModal)
        return ton(i).distWeberBluesModal(ton(j));
    else if (_WeberModal)
        return ton(i).distWeberModal(ton(j));
    else
        return ton(i).distWeber(ton(j));
}


void TonIndex::initRankWeber()
{
    assert(_rankWeber.empty());
    assert(_closed);
    
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        // _rankWeber.emplace_back(_tons.size(), 0);
        // const Ton& toni = ton(i);

        // pair (j, dist(i, j))
        //std::vector<std::pair <size_t, unsigned int>> pcol; // empty
        std::vector<unsigned int> pcol; // empty
        
        for (size_t j = 0; j < _tons.size(); ++j)
        {
            pcol.push_back(distWeber(i, j));
            // if (tonal_flag)
            //     pcol.push_back(toni.distWeber(ton(j)));
            // else
            //     pcol.push_back(toni.distWeberModal(ton(j)));
            // pcol.push_back(std::make_pair(j, toni.distWeber(ton(j))));
        }
            
        // pcol is sorted by weber distance to i.
        _rankWeber.emplace_back(); // empty column
        assert(_rankWeber.back().empty());
        util::ranks<unsigned int>(pcol,
                 [](unsigned int a, unsigned int b) { return (a == b); },
                 [](unsigned int a, unsigned int b) { return (a <  b); },
                                  _rankWeber.back());
        assert(_rankWeber.back().size() == pcol.size());
    }
}


size_t TonIndex::rankWeber(size_t i, size_t j) const
{

    // if (_tons.empty())
    // {
    //     ERROR("rankWeber[{},{}]: this ton index is empty", i, j);
    //     return 0;
    //  }
    assert(closed());
    assert(! _tons.empty());
    assert(! _rankWeber.empty());
    assert(i < _tons.size());
    assert(j < _tons.size());
    assert(_rankWeber[i][j] < _tons.size());
    return _rankWeber[i][j];
}


// static
//bool TonIndex::pcompare(const std::pair <size_t, unsigned int>& a,
//                        const std::pair <size_t, unsigned int>& b)
//{
//    return (a.second < b.second);
//}


// static
//std::vector<size_t>
//TonIndex::getRanks(std::vector<std::pair <size_t, unsigned int>>& v)
//{
//    std::vector<size_t> ranks(v.size(), 0); // null vector
//    if (v.empty()) return ranks;
//
//    // sort v according to the distance value
//    std::sort(v.begin(), v.end(), pcompare);
//
//    // v[0] has rank 0. we leave it to 0.
//    assert(0 < v.size());
//    size_t rank = 0;
//    size_t ties = 1;
//    unsigned int dprec = v[0].second; // dist value of previous
//
//    for (size_t i = 1; i < v.size(); ++i)
//    {
//        size_t current = v[i].first;
//        unsigned int d = v[i].second;
//        if (d == dprec)
//        {
//            ranks[current] = rank;
//            ties++;
//        }
//        else
//        {
//            assert(d > dprec);
//            rank += ties;
//            ties = 1;
//            dprec = d;
//            ranks[current] = rank;
//        }
//    }
//
//    return ranks;
//}


//std::vector<size_t> TonIndex::getRanks(const std::vector<unsigned int>& v)
//{
//    return (util::ranks<unsigned int>(v,
//                     [](unsigned int a, unsigned int b) { return (a < b); }));
//
//}


} // end namespace pse
