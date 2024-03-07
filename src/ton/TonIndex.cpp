//
//  TonIndex.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/02/2023.
//

#include <utility>      // std::pair, std::make_pair
#include <algorithm>    // std::sort

#include "TonIndex.hpp"


namespace pse {

const size_t TonIndex::MAXTONS = 900;

const size_t TonIndex::UNDEF  = MAXTONS+1;

const size_t TonIndex::FAILED = MAXTONS+2;

TonIndex::TonIndex(size_t n):
_tons(), // vector initially empty
_rankWeber(),
_closed(false),
_WeberTonal(true)
{
    switch (n)
    {
        case 0:
            _WeberTonal = false;
            // close() must be called afterwards
            break;

        case 25:
            init25();
            close(true); // tonal Weber
            break;

        case 26:
            init13(ModeName::Major, true); // global flag
            init13(ModeName::Minor, true);
            close(true); // tonal Weber
            break;

        case 30:
            init15(ModeName::Major, true);
            init15(ModeName::Minor, true);
            close(true); // tonal Weber
            break;

        case 104:
            _WeberTonal = false;
            // init13(ModeName::Major);
            init13(ModeName::Minor, true);
            init13(ModeName::Ionian, true);
            init13(ModeName::Dorian, false);
            init13(ModeName::Phrygian, false);
            init13(ModeName::Lydian, false);
            init13(ModeName::Mixolydian, false);
            init13(ModeName::Aeolian, false);
            init13(ModeName::Locrian, false);
            close(false); // modal Weber
            break;
            
        case 135:
            _WeberTonal = false;
            init15(ModeName::Major, true); // Ionian
            init15(ModeName::Minor, false);
            init15(ModeName::MinorMel, false);
            init15(ModeName::Dorian, false);
            init15(ModeName::Phrygian, false);
            init15(ModeName::Lydian, false);
            init15(ModeName::Mixolydian, false);
            init15(ModeName::Aeolian, false);
            init15(ModeName::Locrian, false);
            close(false); // modal Weber
            break;
            
        default:
            ERROR("TonIndex: unsupported list of default tons: {}", n);
            break;
    }
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
    assert(i < _tons.size());
    return _tons.at(i).first;
}


bool TonIndex::global(size_t i) const
{
    assert(i < _tons.size());
    return _tons.at(i).second;

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


void TonIndex::reset()
{
    TRACE("TonIndex: empty the list of tonalities (row headers)");
    _tons.clear();
    _closed = false;
    _WeberTonal = true;
}

void TonIndex::setTonal()
{
    assert(!_closed);
    _WeberTonal = true;
}


void TonIndex::setModal()
{
    assert(!_closed);
    _WeberTonal = false;
}


void TonIndex::add(const Ton& ton, bool global)
{
    if (_closed)
    {
        ERROR("TonIndex add: array of tons is closed");
        return;
    }
    
    if (_tons.size() < MAXTONS)
        _tons.push_back(std::make_pair(ton, global)); // copy
    else
        ERROR("TonIndex add: array of tons overfull");
}


void TonIndex::add(int ks, const ModeName& mode, bool global)
{
    add(Ton(ks, mode), global);
}


void TonIndex::close(bool tonal_flag)
{
    _closed = true;
    // tonal flag : ugly
    initRankWeber((_tons.size() > 30)?false:tonal_flag);
}


bool TonIndex::closed() const
{
    return _closed;
}


void TonIndex::init13(const ModeName& mode, bool f_global)
{
    assert(mode != ModeName::Undef);
    // bool f_global = global(mode);
    for (int ks = -6; ks <= 6; ++ks)
        add(ks, mode, f_global);
}


void TonIndex::init15(const ModeName& mode, bool f_global)
{
    assert(mode != ModeName::Undef);
    // bool f_global = global(mode);
    add(-7, mode, f_global);
    init13(mode, f_global);
    add(7, mode, f_global);
}


void TonIndex::init25()
{
    for (int ks = -4; ks <= 7; ++ks)
        add(ks, ModeName::Major, true);
    for (int ks = -6; ks <= 6; ++ks)
        add(ks, ModeName::Minor, true);
}

void TonIndex::initmodal()
{
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Major, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Minor, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::MinorMel, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Dorian, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Phrygian, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Lydian, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Mixolydian, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Aeolian, true);
    for (int ks = -7; ks <= 7; ++ks)
        add(ks, ModeName::Locrian, true);
}


// static
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


void TonIndex::initRankWeber(bool tonal_flag)
{
    assert(_rankWeber.empty());
    assert(_closed);
    
    for (size_t i = 0; i < _tons.size(); ++i)
    {
        // _rankWeber.emplace_back(_tons.size(), 0);
        const Ton& toni = ton(i);

        // pair (j, dist(i, j))
        //std::vector<std::pair <size_t, unsigned int>> pcol; // empty
        std::vector<unsigned int> pcol; // empty
        
        for (size_t j = 0; j < _tons.size(); ++j)
        {
            if (tonal_flag)
                pcol.push_back(toni.distWeber(ton(j)));
            else
                pcol.push_back(toni.distWeberModal(ton(j)));
            // pcol.push_back(std::make_pair(j, toni.distWeber(ton(j))));
        }
            
        // pcol is sorted by weber distance to i.
        _rankWeber.emplace_back(); // empty column
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
