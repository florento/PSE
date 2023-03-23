//
//  TonIndex.cpp
//  pse
//
//  Created by Florent Jacquemard on 24/02/2023.
//

#include "TonIndex.hpp"


namespace pse {

const size_t TonIndex::MAXTONS = 900;

const size_t TonIndex::UNDEF  = MAXTONS+1;

const size_t TonIndex::FAILED = MAXTONS+2;

TonIndex::TonIndex(size_t n):
_tons() // empty vector
{
    switch (n)
    {
        case 0:
            break;
        case 25:
            init25();

        case 26:
            init13(ModeName::Maj);
            init13(ModeName::Min);
            break;

        case 30:
            init15(ModeName::Maj);
            init15(ModeName::Min);
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
    return _tons.at(i);
}


size_t TonIndex::find(const Ton& ton) const
{
    for (size_t i = 0; i < _tons.size(); ++i)
        if (_tons.at(i) == ton)
            return i;

    ERROR("TonIndex find: {} not found", ton);
    return -1;
}


void TonIndex::reset()
{
    TRACE("TonIndex: empty the list of tonalities (row headers)");
    _tons.clear();
}


void TonIndex::add(const Ton& ton)
{
    if (_tons.size() < MAXTONS)
        _tons.push_back(ton); // copy
    else
        ERROR("TonIndex: array of tons overfull");
}


void TonIndex::add(int ks, const ModeName& mode)
{
    if (_tons.size() < MAXTONS)
        _tons.emplace_back(ks, mode);
    else
        ERROR("TonIndex: array of tons overfull");
}


void TonIndex::init13(const ModeName& mode)
{
    assert(mode != ModeName::Undef);
    for (int ks = -6; ks <= 6; ++ks)
        _tons.emplace_back(ks, mode);
}


void TonIndex::init15(const ModeName& mode)
{
    assert(mode != ModeName::Undef);
    _tons.emplace_back(-7, mode);
    init13(mode);
    _tons.emplace_back(7, mode);
}


void TonIndex::init25()
{
    for (int ks = -4; ks <= 7; ++ks)
        _tons.emplace_back(ks, ModeName::Maj);
    for (int ks = -6; ks <= 6; ++ks)
        _tons.emplace_back(ks, ModeName::Min);
}


//const std::vector<const Ton> TonIndex::TONS30 =
//{
//    Ton(-7, ModeName::Maj),
//    Ton(-6, ModeName::Maj),
//    Ton(-5, ModeName::Maj),
//    Ton(-4, ModeName::Maj),
//    Ton(-3, ModeName::Maj),
//    Ton(-2, ModeName::Maj),
//    Ton(-1, ModeName::Maj),
//    Ton(0,  ModeName::Maj),
//    Ton(1,  ModeName::Maj),
//    Ton(2,  ModeName::Maj),
//    Ton(3,  ModeName::Maj),
//    Ton(4,  ModeName::Maj),
//    Ton(5,  ModeName::Maj),
//    Ton(6,  ModeName::Maj),
//    Ton(7,  ModeName::Maj),
//    Ton(-7, ModeName::Min),
//    Ton(-6, ModeName::Min),
//    Ton(-5, ModeName::Min),
//    Ton(-4, ModeName::Min),
//    Ton(-3, ModeName::Min),
//    Ton(-2, ModeName::Min),
//    Ton(-1, ModeName::Min),
//    Ton(0,  ModeName::Min),
//    Ton(1,  ModeName::Min),
//    Ton(2,  ModeName::Min),
//    Ton(3,  ModeName::Min),
//    Ton(4,  ModeName::Min),
//    Ton(5,  ModeName::Min),
//    Ton(6,  ModeName::Min),
//    Ton(7,  ModeName::Min)
//};
//
//
//const std::vector<const Ton> TonIndex::TONS26 =
//{
//    Ton(-6, ModeName::Maj),
//    Ton(-5, ModeName::Maj),
//    Ton(-4, ModeName::Maj),
//    Ton(-3, ModeName::Maj),
//    Ton(-2, ModeName::Maj),
//    Ton(-1, ModeName::Maj),
//    Ton(0,  ModeName::Maj),
//    Ton(1,  ModeName::Maj),
//    Ton(2,  ModeName::Maj),
//    Ton(3,  ModeName::Maj),
//    Ton(4,  ModeName::Maj),
//    Ton(5,  ModeName::Maj),
//    Ton(6,  ModeName::Maj),
//    Ton(-6, ModeName::Min),
//    Ton(-5, ModeName::Min),
//    Ton(-4, ModeName::Min),
//    Ton(-3, ModeName::Min),
//    Ton(-2, ModeName::Min),
//    Ton(-1, ModeName::Min),
//    Ton(0,  ModeName::Min),
//    Ton(1,  ModeName::Min),
//    Ton(2,  ModeName::Min),
//    Ton(3,  ModeName::Min),
//    Ton(4,  ModeName::Min),
//    Ton(5,  ModeName::Min),
//    Ton(6,  ModeName::Min),
//};

} // end namespace pse
