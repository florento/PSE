//
//  PSTable.cpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
/// @addtogroup pitch
/// @{


#include "PSTable.hpp"

namespace pse {


PST::PST(PSEnum& e, size_t n):
index(n),    // empty
_enum(e),
_psvs(),     // empty
_global(TonIndex::UNDEF), // undef (value out of range)
_estimated_global(false),
_estimated_locals(false),
_rowcost(), // empty
_frowcost() // empty
{
    TRACE("new PS Table");
    
// init table with default vector of tons
//    for (auto ton : TONS) _tons.push_back(ton); // vector copy default tons
//    _rowcost.assign(nbtons(), 0);
//    _frowcost.assign(nbtons(), false);
}


//PST::PST(const PSEnum& e, size_t n0, size_t n1):
//_tons(TONS), // vector copy
//_enum(e, n0, n1),
//_psvs(), // empty
//_global(nbtons()), // undef (value out of range)
//_estimated_global(false),
//_estimated_locals(false),
//_rowcost(),
//_frowcost()
//{
//    _rowcost.fill(0);
//    _frowcost.fill(false);
//    bool status = init();
//    if (status == false)
//    {
//        ERROR("PST: fail to compute spelling table {}-{}", n0, n1);
//    }
//}


PST::~PST()
{
    TRACE("delete PS Table {}-{}", _enum.first(), _enum.stop());
    //((stop() == NoteEnum::ID_UNDEF)?"NaN":std::to_string(stop())));
    _psvs.clear();
    _rowcost.clear();
    _frowcost.clear();
}


bool PST::init()
{
    TRACE("PST: computing spelling table {}-{}");
    assert(_psvs.empty()); // do not recompute
    
    // first note of current bar
    size_t i0 = _enum.first();
    // current bar number
    // NOT _enum.measure(i0) if first bar is empty
    size_t b  = 0;
    // first note after current bar
    size_t i1;

    // empty seq of notes
    if (_enum.outside(i0))
    {
        WARN("PST init: empty sequence of notes");
        return false;
    }
    
    if (index.empty())
    {
        ERROR("PST init: no tonality");
        return false;
    }
    
    // reset table
    _psvs.clear();
    _estimated_global = false;
    _global = TonIndex::UNDEF; // undef (value out of range)
    _estimated_locals = false;
    
    while (_enum.inside(i0))
    {
        assert(_enum.measure(i0) >= b);
        // the current bar b is empty (i0 is after b)
        if (_enum.measure(i0) > b)
        {
            DEBUGU("PST init: bar {} EMPTY", b);
            // vector of empty bags
            _psvs.push_back(std::make_unique<PSV>(index, _enum, i0, i0));
            ++b;
            continue;
        }
            
        // the current bar b is not empty (i0 in b). add it.
        i1 = bound_measure(b, i0); // first note after current measure b
        TRACE("PST init: bar {} {}-{}", b, i0, i1);
        assert(_enum.inside(i1 - 1));
        assert(_enum.measure(i1 - 1) == b);
        TRACE("PST: compute column of the best spelling table for measure {}\
              (notes {}-{})", b, i0, i1-1);
        // add a PS vector (column) for the measure b
        _psvs.push_back(std::make_unique<PSV>(index, _enum, i0, i1));
        assert(_psvs.size() == b+1);
        // then start next measure
        i0 = i1; // index of first note of next bar
        ++b;
    }
    return true;
}


size_t PST::bound_measure(size_t bar, size_t i0)
{
    size_t i;
    for (i = i0; (_enum.inside(i)) && (_enum.measure(i) == bar); ++i)
    {
        assert(_enum.measure(i) == bar);
    }

    return i;
}
   

size_t PST::size() const
{
    return _psvs.size();
}


PSV& PST::column(size_t i)
{
    assert(i < _psvs.size());
    assert(_psvs.at(i) != nullptr);
    return *(_psvs.at(i));
}


// inline
//size_t PST::nbTons() const
//{
//    return _tons.size();
//}


//const Ton& PST::ton(size_t i) const
//{
//    assert(i < _tons.size());
//    return _tons.at(i);
//}


//void PST::resetTons()
//{
//    TRACE("PST: empty the list of row headers (tonalities)");
//    _tons.clear();
//}


//void PST::addTon(const Ton ton)
//{
//    _tons.push_back(ton); // copy
//}


//void PST::addTon(int ks, Ton::Mode mode)
//{
//    _tons.emplace_back(ks, mode);
//}


size_t PST::iglobal() const
{
    if (_global == TonIndex::UNDEF)  // (_estimated_global == false)
    {
        ERROR("PST: global ton must be estimated before accessed");
    }
    else if (_global == TonIndex::FAILED)  // (_estimated_global == false)
    {
        ERROR("PST: estimated pf global ton failed");
    }
    
    return _global;
}


const Ton& PST::global() const
{
    assert(iglobal() < index.size());
    return index.ton(iglobal());
}


bool PST::estimateGlobal()
{
    if (_global != TonIndex::UNDEF)  // (_estimated_global)
    {
        WARN("PST: re-estimation of global tonality. ignored.");
        return (_global != TonIndex::FAILED);
    }
    
    if (_enum.empty()) // first() == stop()
    {
        ERROR("PST: estimation of global tonality impossible: no entry");
        _global = TonIndex::FAILED; // error value
        // _estimated_global = true;
        return false;
    }
    
    if (index.empty())
    {
        ERROR("PST estimateGlobal: no tonality");
        _global = TonIndex::FAILED; // error value
        return false;
    }
       
    computeRowcost();
    dump_table(); // DEBUGU
    
    size_t ibest = TonIndex::UNDEF; // out-of-range. initialized because of warning
    
    // estimate the best tonality wrt costs (nb accidentals)
    for (size_t i = 0; i < index.size(); ++i)
    {
        // should not happen
        if (frowcost(i) == false)
        {
            WARN("PST: failure with rowcost {}", i);
            continue;
        }
        // new best ton
        else if ((i == 0) || (rowcost(i) < rowcost(ibest)))
        {
            ibest = i;
        }
        // tie break
        else if (rowcost(i) == rowcost(ibest))
        {
            const Ton& ton = index.ton(i);
            const Ton& tonmin = index.ton(ibest);
            // tie break 1 : smaller key signature
            if (std::abs(ton.fifths()) < std::abs(tonmin.fifths()))
            {
                ibest = i;
            }
            // tie break 2 : major better than minor
            else if ((ton.mode() == Ton::Mode::Maj) &&
                     (tonmin.mode() != Ton::Mode::Maj))
            {
                ibest = i;
            }
            // tie break 3 : sharp better than flat
            else if (ton.fifths() > tonmin.fifths())
            {
                ibest = i;
            }
            // default: keep the smaller ibest
            else
            {
                WARN("PST: estim global: tie brack fail {} {}", tonmin, ton);
            }
        }
    }
    
    assert(ibest != TonIndex::FAILED);
    if (ibest == TonIndex::UNDEF)
    {
        ERROR("PST: failed to estimate global tonality");
        _global = TonIndex::FAILED;
        return false;
    }
    else
    {
        _global = ibest;
        //_estimated_global = true;
        return true;
    }
}


/// revoir
void PST::computeRowcost()
{
    // initialise as static arrays
    _rowcost.assign(index.size(), 0);
    _frowcost.assign(index.size(), false);
    
    for (size_t i = 0; i < index.size(); ++i)
    {
        // every column of the table corresponds to a measure
        // for (std::unique_ptr<const PSV> psv : _psvs)
        for (size_t j = 0; j < _psvs.size(); ++j)
        {
            assert(_psvs[j]);
            const PSV& psv = *(_psvs[j]);
            const PSB& psb = psv.best0(i);
            if (! psb.empty())
            {
                if (frowcost(i) == false)
                {
                    _frowcost[i] = true;
                    _rowcost[i] = psb.cost().getAccid();
                }
                else
                {
                    _rowcost[i] += psb.cost().getAccid();
                }
            }
        }
    }
    // if one psb is empty, the whole column is empty
    // hence if a whole row could no be estimated, all columns are empty
    // hence the table is empty.
    assert(check_rowcost());
}


unsigned int PST::rowcost(size_t i) const
{
    assert(i < _rowcost.size());
    return _rowcost.at(i);
}


bool PST::frowcost(size_t i) const
{
    assert(i < _frowcost.size());
    assert(_frowcost.size() == _rowcost.size());
    return _frowcost.at(i);
}


size_t PST::ilocal(size_t i) const
{
    if (_estimated_locals == false)
        ERROR("PST: local tons must be estimated before accessed");

    assert(i < _psvs.size());
    assert(_psvs[i]);
    return _psvs[i]->local();
}


const Ton& PST::local(size_t i) const
{
    return index.ton(ilocal(i));
}


bool PST::estimateLocals()
{
    if (_estimated_locals)
    {
        WARN("PST: re-estimation of local tonalities. ignored.");
        return true;
    }
    
    if (_global == TonIndex::UNDEF)
    {
        ERROR("PST: estimate global ton before estimating local tons.");
        _estimated_locals = false;
        return false;
    }
    else if (_global == TonIndex::FAILED)
    {
        ERROR("PST: failed to estimate global ton. cannot estimate local tons.");
        _estimated_locals = false;
        return false;
    }

    if (_enum.empty()) // (first() == stop()))
    {
        ERROR("PST: estimation of local tonalities impossible: no entry");
        _estimated_locals = true;
        return false;
    }
    assert(_global < index.size());

    // index of previous local tonality
    size_t pre = _global;
    for (size_t i = 0; i < _psvs.size(); ++i)
    {
        PSV& psv = column(i);
        assert(pre < index.size());
        bool status = psv.estimateLocal(pre);
        if (status) // local succesfully estimated
        {
            pre = psv.local(i);
        }
        // otherwise, do not update the pre.
    }
    _estimated_locals = true;
    return true;
}


bool PST::rename()
{
    bool status = (_global != TonIndex::UNDEF);
    if (status == false)
        status = estimateGlobal();
    
    if ((status == false) || (_global == TonIndex::FAILED))
    {
        ERROR("PST: failed to estimate global ton. cannot rename pitches {}-{}",
              _enum.first(), _enum.stop());
        return false;
    }
    assert(_global < index.size());
    const Ton& gton = index.ton(_global);
    DEBUGU("PST: estimated global ton: {}", gton);

    if (! _estimated_locals)
        status = estimateLocals();
    
    if (status == false)
    {
        ERROR("PST: failed to estimate locals. cannot rename pitches {}-{}",
              _enum.first(), _enum.stop());
        return false;
    }
    
    for (size_t i = 0; i < _psvs.size(); ++i)
    {
        assert(_psvs[i]);
        PSV& psv = *(_psvs[i]);
        // std::cout << "PST: rename vector " << i << std::endl;
        DEBUGU("PST: renaming bar {}({}-{}) (local ton = {})",
              i, psv.first(), psv.stop(), index.ton(psv.local()));
        status = psv.rename(_global);
    }
    return status;
}


// debug
bool PST::check_rowcost() const
{
    assert(_frowcost.size() == _rowcost.size());
    for (bool b : _frowcost)
    {
        if (b == true)
            return true;
    }
    return false;
}


void PST::dump_rowcost() const
{
    assert(check_rowcost());
    DEBUGU("PST: Row Costs:");
    for (size_t i = 0; i < index.size(); ++i)
    {
        DEBUGU("PST row: {} cost {}", index.ton(i), _rowcost[i]);
    }
}


void PST::dump_table() const
{
    DEBUGU("PS Table:");
    for (size_t i = 0; i < index.size(); ++i)
    {
        std::string srow; // row content (bars)
        // every column of the table corresponds to a measure
        // for (std::unique_ptr<const PSV> psv : _psvs)
        for (size_t j = 0; j < _psvs.size(); ++j)
        {
            assert(_psvs[j]);
            const PSV& psv = *(_psvs[j]);
            const PSB& psb = psv.best0(i);
            srow += " ";
            if (psb.empty())
            {
                srow += std::to_string(j);
                srow += ":_";
            }
            else
            {
                srow += std::to_string(j);
                srow += ":";
                srow += std::to_string(psb.cost().getAccid());
            }
        }
        std::string hrow; // header
        
        DEBUGU("PST row: {} {}: {}", index.ton(i), _rowcost[i], srow);
    }
}



} // end namespace pse

/// @}
