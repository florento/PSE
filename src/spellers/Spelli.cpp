//
//  Spelli.cpp
//  pse
//
//  Created by Florent Jacquemard on 23/01/2025.
//

#include "Spelli.hpp"


namespace pse {


Spelli::Spelli(size_t nbton, bool dflag):
_index(new TonIndex(nbton)),
//_seed(nullptr),
//_seedAdiscount(true), // discount obsolete for CostA
_seedAnodiscount(false),
_seedADplus(),
_seedADlex(),
_debug(dflag),
_uton(new Ton()) // undef
{
    setVerbosityLevel(4);
    debug(dflag);
    spdlog::set_pattern("[%^%l%$] %v");
}


Spelli::Spelli(std::shared_ptr<TonIndex> id, bool dflag):
_index(id), // copy
_seedAnodiscount(false),
_seedADplus(),
_seedADlex(),
_debug(dflag),
_uton(new Ton()) // undef
{
    assert(id);
    assert(id->closed());
    setVerbosityLevel(4);
    debug(dflag);
    spdlog_setPattern();
}


Spelli::~Spelli()
{
    assert(_uton);
    delete _uton;
}


void Spelli::debug(bool flag)
{
    TRACE("Speller: debug mode {}", flag);
    _debug = flag;
    if (flag)
        setVerbosityLevel(5);
    else
        setVerbosityLevel(4);
}

//
// array of tonalities
//

TonIndex& Spelli::index()
{
    assert(_index);
    return *_index;
}


size_t Spelli::nbTons() const
{
    assert(_index);
    return _index->size();  // nbTons();
}


const Ton& Spelli::ton(size_t i) const
{
    assert(_index);
    return _index->ton(i);
}


void Spelli::resetTons(size_t n)
{
    assert(_index);
    _index->reset(n);
}


void Spelli::addTon(const Ton& ton, bool global)
{
    TRACE("Speller: add tonality {}", ton);
    assert(_index);
    _index->add(ton);
}


void Spelli::addTon(int ks, ModeName mode, bool global)
{
    if (ks < -7 || 7 < ks)
    {
        ERROR("Speller addTon: wrong key signature value {}", ks);
    }
    TRACE("Speller: add tonality {} {}", ks, mode);
    assert(_index);
    _index->add(ks, mode, global);
}

//void Spelli::WeberTonal()
//{
//    assert(_index);
//    _index->setTonal();
//}


//void Spelli::WeberModal()
//{
//    assert(_index);
//    _index->setModal();
//}


void Spelli::closeTons()
{
    assert(_index);
    _index->close();
}


bool Spelli::closedTons() const
{
    assert(_index);
    return _index->closed();
}

//
// prepare spelling
//

Cost& Spelli::sampleCost(CostType ct)
{
    switch (ct)
    {
        case CostType::ACCIDlead:
            WARN("Speller: discount obsolete for CostA (ignored)");
            return _seedAnodiscount;
            
        case CostType::ACCID:
            return _seedAnodiscount;
            
        case CostType::ADplus:
            return _seedADplus;
            
        case CostType::ADlex:
            return _seedADlex;
            
        default:
            ERROR("Speller sampleCost unexpected code {}", ct);
            return _seedAnodiscount;
    }
}


} // namespace pse
