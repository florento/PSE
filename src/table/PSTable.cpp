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
#include "PSGrid.hpp"


namespace pse {


PST::PST(const Algo& a, const Cost& seed, const TonIndex& i,
         PSEnum& e, bool tonal, bool dflag):
_algo(a),
_enum(e),
_index(i),
_psvs(),     // initially empty
_rowcost(),
_debug(dflag)
{
    TRACE("new PS Table {}-{} for {}", e.first(), e.stop(), a);
       
    if (a == Algo::PSE || a == Algo::PSD)
    {
        bool status = init_psvs(seed, tonal);
        if (status == false)
        {
            ERROR("PST: fail to compute spelling table {}-{} for {}",
                  e.first(), e.stop(), a);
        }
    }
    
    compute_rowcosts(seed);

// @todo TBR
//    if (a == Algo::PSE || a == Algo::PSE0)
//    {
//        bool status = init_locals();
//        if (status == false)
//        {
//            ERROR("PST: fail to estimate local tons for table {}-{}, {}",
//                  e.first(), e.stop(), a);
//        }
//    }

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


/// @todo remove globals, replaced by global flag in ton index
PST::PST(const Algo& a, const PST& tab, const Cost& seed,
         const PSO& globals, const PSG& locals, bool tonal, bool dflag):
_algo(a),
_enum(tab._enum),
_index(tab._index),
_psvs(),     // initially empty
_rowcost(),
_debug(dflag)
{
    TRACE("new PS Table {}-{} from grid, for {}",
          _enum.first(), _enum.stop(), _algo);
    assert(locals.nbTons() == _index.size());
    assert(locals.measures() == tab.measures());
    assert(_algo == Algo::PSE || _algo == Algo::PSD);
    bool status = init_psvs(tab, seed, globals, locals, tonal);
    if (status == false)
    {
        ERROR("PST: fail to compute spelling table {}-{} for {}",
              _enum.first(), _enum.stop(), _algo);
    }
    else
    {
        compute_rowcosts(seed, globals);
    }
}


/// @todo remove globals, replaced by global flag in ton index
PST::PST(const PST& tab, const Cost& seed,
         const PSO& globals, const PSG& locals, bool tonal, bool dflag):
PST(tab._algo, tab, seed, globals, locals, tonal, dflag)
{ }


PST::~PST()
{
    TRACE("delete PS Table {}-{}", _enum.first(), _enum.stop());
    //((stop() == NoteEnum::ID_UNDEF)?"NaN":std::to_string(stop())));
    _psvs.clear();
}


bool PST::status() const
{
    return (! _psvs.empty());
}

 
//void PST::init_rowcosts(const Cost& seed)
//{
//    TRACE("PST: initialize row costs to zero");
//    assert(_rowcost.empty());
//    
//    // one different shared pointer for each row cost
//    for (size_t i = 0; i < _index.size(); ++i)
//        _rowcost.push_back(seed.shared_zero());
//}


void PST::compute_rowcosts(const Cost& seed)
{
//    const PSO allglobals(_index, _debug, true); // full
//    compute_rowcosts(seed, allglobals);
//    init_rowcosts(seed);
    for (size_t i = 0; i < _index.size(); ++i)
    {
        _rowcost.push_back(seed.shared_zero());
        assert(_rowcost.back());
        for (size_t j = 0; j < _psvs.size(); ++j)
        {
            assert(_psvs[j]);
            assert(_psvs[j]->size() == _index.size());
            const PSB& psb = _psvs[j]->bag(i);
            if (! psb.empty())
            {
                _rowcost.back()->operator+=(psb.cost());
            }
        }
    }
}


/// @todo remove globals, replaced by global flag in ton index
void PST::compute_rowcosts(const Cost& seed, const PSO& globals)
{
    // init_rowcosts: one different shared pointer for each row cost
    // TRACE("PST: initialize row costs to zero");
    assert(_rowcost.empty());
    for (size_t i = 0; i < _index.size(); ++i)
        _rowcost.push_back(seed.shared_zero());

    for (size_t j = 0; j < _psvs.size(); ++j)
    {
        assert(_psvs[j]);
        PSV& psv = *(_psvs[j]);
        assert(psv.size() == _index.size());

        // compute the rowcosts only for candidate global tonalities
        // for (auto it = globals.cbegin(); it != globals.cend(); ++it)
        for (size_t ig = 0; ig < _index.size(); ++ig)
        {
            // size_t ig = *it; // index of global tnolity in tonindex.
            if (!_index.isGlobal(ig))
            {
                continue;
            }
            assert(ig < _index.size());
            assert(ig < psv.size());
            const PSB& psb = psv.bag(ig);
            if (! psb.empty())
            {
                assert(_rowcost.at(ig));
                (_rowcost.at(ig))->operator+=(psb.cost());
            }
        }
    }
}


bool PST::init_psvs(const Cost& seed, bool tonal)
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
    
    if (_index.empty())
    {
        ERROR("PST init: no tonality");
        return false;
    }
    
    // reset table
    _psvs.clear();
    
    while (_enum.inside(i0))
    {
        assert(_enum.measure(i0) >= b);
        // the current bar b is empty (i0 is after b)
        if (_enum.measure(i0) > b)
        {
            TRACE("PST init: bar {} EMPTY", b);
            // vector of empty bags
            _psvs.push_back(std::unique_ptr<PSV>(new
                           PSV(_algo, seed, _index, _enum, i0, i0, b, tonal)));
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
        _psvs.push_back(std::unique_ptr<PSV>(new
                           PSV(_algo, seed, _index, _enum, i0, i1, b, tonal)));
        assert(_psvs.size() == b+1);
        // then start next measure
        i0 = i1; // index of first note of next bar
        ++b;
    }
    return true;
}


/// @todo remove globals, replaced by global flag in ton index
bool PST::init_psvs(const PST& tab, const Cost& seed,
                    const PSO& globals, const PSG& grid, bool tonal)
{
    TRACE("PST: re-computing spelling table {}-{}");
    assert(_psvs.empty()); // do not recompute
    assert(grid.size() == tab.size()); // nb of columns

    for (size_t j = 0; j < tab.size(); ++j)
    {
        const PSV& col = tab.column(j);
        const std::vector<size_t>& locals = grid.column(j);
        assert(col.bar() == j);
        _psvs.emplace_back(std::unique_ptr<PSV>(new
                                    PSV(col, seed, globals, locals, tonal)));
         // PSV(_algo, seed, _index, col.enumerator(), j)));
    }
    
    return true;
}


//bool PST::init_locals()
//{
//    TRACE("PST: computing local tonality estimations in table {}-{}, {}");
//    bool status = true;
//    for (size_t i = 0; i < _psvs.size(); ++i)
//    {
//        PSV& psv = column(i);
//        if (i == 0) // first bar
//            status = status && psv.estimateLocals();
//        else        // next bars
//            status = status && psv.estimateLocals(column(i-1));
//    }
//
//    return status;
//}


//bool PST::init_locals(size_t ig)
//{
//    bool status = true;
//    assert(ig < _index.size());
//    // index of previous local tonality
//    size_t pre = ig;
//
//    for (size_t i = 0; i < _psvs.size(); ++i)
//    {
//        PSV& psv = column(i);
//        assert(pre < _index.size());
//        bool lstatus;
//
//        if (i == 0) // first bar
//            lstatus = psv.estimateLocal(ig);
//        else        // next bars
//            lstatus = psv.estimateLocal(ig, column(i-1));
//    }
//
//    return status;
//}


//bool PST::init_locals_globals()
//{
//    if (estimatedLocals()) // (_estimated_locals)
//    {
//        WARN("PST: re-estimation of local tonalities. ignored.");
//        return false;
//    }
//
//    if (_globals.empty())
//    {
//        if (_global == TonIndex::UNDEF)
//        {
//            ERROR("PST: estimate global candidates ton before estimating local tons.");
//        }
//        else if (_global == TonIndex::FAILED)
//        {
//            ERROR("PST: failed to estimate global ton. cannot estimate local tons.");
//        }
//        else
//        {
//            ERROR("PST: unexpected state in local tons estimation.");
//        }
//        _estimated_locals = true;
//        return false;
//    }
//
//    if (_enum.empty()) // (first() == stop()))
//    {
//        ERROR("PST: estimation of local tonalities impossible: no entry");
//        _estimated_locals = true;
//        return false;
//    }
//
//    for (size_t ig : _globals)
//        init_locals(ig);
//
//    _estimated_locals = true;
//    return true;
//}

size_t PST::bound_measure(size_t bar, size_t i0)
{
    size_t i;
    for (i = i0; (_enum.inside(i)) && (_enum.measure(i) == bar); ++i)
    {
        assert(_enum.measure(i) == bar);
    }

    return i;
}


const Ton& PST::rowHeader(size_t i) const
{
    assert(i < _index.size());
    return _index.ton(i);
}


const Cost& PST::rowCost(size_t i) const
{
    assert(i < _rowcost.size());
    assert(_rowcost.at(i));
    return *(_rowcost.at(i));
}


size_t PST::size() const
{
    return _psvs.size();
}

PSV& PST::column(size_t j) const
{
    assert(j < _psvs.size());
    assert(_psvs.at(j) != nullptr);
    return *(_psvs.at(j));
}


const PSEnum& PST::columnHeader(size_t j) const
{
    assert(j < _psvs.size());
    assert(_psvs.at(j) != nullptr);
    return _psvs.at(j)->enumerator();
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


//unsigned int PST::rowcost(size_t i) const
//{
//    assert(i < _rowcost.size());
//    return _rowcost.at(i);
//}


//bool PST::frowcost(size_t i) const
//{
//    assert(i < _frowcost.size());
//    assert(_frowcost.size() == _rowcost.size());
//    return _frowcost.at(i);
//}


//bool PST::eGlobals_eq_lex(const PSCost& lhs, const PSCost& rhs) const
//{
//    //return lhs.eq_approx(rhs, _enum.length());
//    return PSCost::approxeq(lhs.getAccid(),  rhs.getAccid(), _enum.size());
//}


//bool PST::eGlobals_eq_cumul(const PSCost& lhs, const PSCost& rhs) const
//{
//    //return lhs.eq_cumul(rhs, _enum.length());
//    return PSCost::approxeq(lhs.getAccid() + lhs.getDia(),
//                            rhs.getAccid() + rhs.getDia(),
//                            2 * _enum.size());
//}


//bool PST::estimateGlobals()
//{
//    if (estimatedGlobals()) // ((! _globals.empty()) || (_global != TonIndex::UNDEF))
//    {
//        ERROR("PST: global tonality candidates already estimated.");
//        //WARN("PST: re-estimation of global tonality candidates. ignored.");
//        return false;
//    }
//
//    if (_enum.empty()) // first() == stop()
//    {
//        ERROR("PST: estimation of global tonality impossible: no entry");
//        _global = TonIndex::FAILED; // error value
//        return false;
//    }
//
//    if (_index.empty())
//    {
//        ERROR("PST estimateGlobal: no tonality");
//        _global = TonIndex::FAILED; // error value        // _estimated_globals = true;
//        return false;
//    }
//
//    /// store cumultated cost of every row
//    /// debug: one row cost at least must be estimated:
//    /// if one psb is empty, the whole column is empty
//    /// hence if a whole row could no be estimated, all columns are empty
//    /// hence the table is empty.
//    std::vector<PSCost> RowCost;
//    for (size_t i = 0; i < _index.size(); ++i)
//    {
//        RowCost.push_back(rowCost(0, i));
//    }
//
//    // DEBUG output
//    // if (_debug) dump_table(RowCost);
//
//    // candidate list for best global
//    // _globals is empty
//    // invariant: all index in _globals have the same RowCost
//    _globals.push_back(0);
//
//    // estimate the best tonality wrt costs (nb accidentals)
//    for (size_t i = 1; i < _index.size(); ++i)
//    {
//        assert(i < RowCost.size());
//        assert(! _globals.empty());
//        // all elements of cands have same cost
//        const PSCost bestCost = RowCost[_globals[0]];
//        const PSCost& rc = RowCost[i];
//        // new best ton
//        if (rc < bestCost)
//        {
//            // rc far under bestCost
//            if (! eGlobals_eq_lex(rc, bestCost))
//                _globals.clear();
//            // push front
//            _globals.insert(_globals.begin(), i);
//            //_globals.push_back(i);
//        }
//        // tie
//        else if (eGlobals_eq_lex(rc, bestCost))
//        {
//            _globals.push_back(i);
//        }
//        // otherwise do nothing
//        else
//        {
//            //assert(eGlobals_less(bestCost, rc));
//            assert(rc > bestCost);
//        }
//    }
//    assert(! _globals.empty());
//    assert(estimatedGlobals()); // the checker is well defined
//    INFO("PST: candidates global: {}", _globals.size());
//    return true;
//
//    // deprecated: tie break solving
//    //    const Ton& ton = index.ton(i);
//    //    const Ton& tonmin = index.ton(ibest);
//    //    // tie break 1 : smaller key signature
//    //    if (std::abs(ton.fifths()) < std::abs(tonmin.fifths()))
//    //    {
//    //        ibest = i;
//    //    }
//    //    // tie break 2 : major better than minor
//    //    else if ((ton.mode() == Ton::Mode::Maj) &&
//    //             (tonmin.mode() != Ton::Mode::Maj))
//    //    {
//    //        ibest = i;
//    //    }
//    //    // tie break 3 : sharp better than flat
//    //    else if (ton.fifths() > tonmin.fifths())
//    //    {
//    //        ibest = i;
//    //    }
//    //    // default: keep the smaller ibest
//    //    else
//    //    {
//    //        WARN("PST: estim global: tie brack fail {} {}", tonmin, ton);
//    //    }
//}


//bool PST::estimatedGlobals() const
//{
//    return ((! _globals.empty()) || (_global != TonIndex::UNDEF));
//}


//size_t PST::globalCands() const
//{
//    if (! estimatedGlobals())
//        WARN("PST: globalCands: globals have not been estimated.");
//    return _globals.size();
//}


//size_t PST::iglobalCand(size_t i) const
//{
//    if (! estimatedGlobals())
//        WARN("PST: globalCands: globals have not been estimated.");
//    assert(i < _globals.size());
//    return _globals.at(i);
//}


//const Ton& PST::globalCand(size_t i) const
//{
//    size_t ig = iglobalCand(i);
//    assert(ig < _index.size());
//    return _index.ton(ig);
//}


//bool PST::estimatedLocals() const
//{
//    return _estimated_locals;
//}


//size_t PST::ilocal(size_t i, size_t j) const
//{
//    if (_estimated_locals == false)
//        ERROR("PST: local tons must be estimated before accessed");
//    assert(j < _psvs.size());
//    assert(_psvs[j]);
//    assert(i < _index.size());
//    assert(i != TonIndex::UNDEF);
//    assert(i != TonIndex::FAILED);
//    return _psvs[j]->ilocal(i);
//}


//const Ton& PST::local(size_t i, size_t j) const
//{
//    return _index.ton(ilocal(i, j));
//}


//void PST::setGlobal(size_t ig)
//{
//    assert(ig < _index.size());
//    assert(ig != TonIndex::UNDEF);
//    assert(ig != TonIndex::FAILED);
//    if ((! _globals.empty()) || (_global != TonIndex::UNDEF))
//    {
//        WARN("PST setGlobal: global tonality candidates already estimated or set, wiped");
//        _globals.clear();
//    }
//    assert(_globals.empty());
//    _globals.push_back(ig);
//    _global = ig;
//    assert(estimatedGlobals()); // the checker is well defined
//    assert(estimatedGlobal()); // the checker is well defined
//}


//bool PST::estimateGlobal()
//{
//    if (_global == TonIndex::FAILED)  // (_estimated_global)
//    {
//        WARN("PST: the estimation of global tonality failed.");
//        return false;
//    }
//    else if (_global != TonIndex::UNDEF)  // (_estimated_global)
//    {
//        WARN("PST: re-estimation of global tonality. ignored.");
//        return true;
//    }
//    if (_globals.empty())
//    {
//        assert(_global == TonIndex::UNDEF);
//        ERROR("PST estimateGlobal: global tonality candidates not estimated.");
//        return false;
//    }
//
//    PSCost cbest; // zero
//    size_t ibest = TonIndex::UNDEF;
//
//
//    // DEBUG output
//    // std::vector<PSCost> RowCost;
//    // for (size_t i = 0; i < index.size(); ++i)
//    //    RowCost.push_back(rowCost(1, i));
//    // if (_debug) dump_table(RowCost);
//
//    // select the best global candidate according to
//    // distance component of the (cumulated) cost.
//    for (size_t i = 0; i < _globals.size(); ++i)
//    {
//        size_t ig = _globals[i];
//        PSCost rc = rowCost(1, ig);
//
//        // new best global
//        // ALT: else if (rc.less_approx(cbest, _enum.length()))
//        if ((i == 0) || (rc.getDist() < cbest.getDist()))
//        {
//            ibest = ig;
//            cbest = rc;
//        }
//        // tie
//        // ALT: else if (rc.eq_approx(cbest, _enum.length()))
//        else if (rc.getDist() == cbest.getDist())
//        {
//            const Ton& ton = _index.ton(ig);
//            const Ton& tonbest = _index.ton(ibest);
//            // tie break 1 : smaller key signature
//            if (std::abs(ton.fifths()) < std::abs(tonbest.fifths()))
//            {
//                ibest = ig;
//            }
//            else if (std::abs(ton.fifths()) == std::abs(tonbest.fifths()))
//            {
//                WARN("PST estimate global: tie break fail {} - {}", ibest, ig);
//                // keep ibest (arbitrarily)
//            }
//            // otherwise ibest unchanged
//        }
//        // otherwise ibest unchanged
//        else
//        {
//            assert(rc.getDist() > cbest.getDist());
//        }
//    }
//
//    if (ibest == TonIndex::UNDEF)
//    {
//        ERROR("PST estimation of global failed");
//        _global = TonIndex::FAILED;
//        return false;
//    }
//    else
//    {
//        assert(ibest != TonIndex::FAILED);
//        assert(ibest < _index.size());
//        _global = ibest;
//        assert(estimatedGlobal()); // the checker is well defined
//        return true;
//    }
//}


//bool PST::estimatedGlobal() const
//{
//    return (_global != TonIndex::UNDEF);
//}


//size_t PST::iglobal() const
//{
//    if (! estimatedGlobal())  // (_global == TonIndex::UNDEF)
//    {
//        ERROR("PST: global ton must be estimated before accessed");
//    }
//    else if (_global == TonIndex::FAILED)  // (_estimated_global == false)
//    {
//        ERROR("PST: estimated pf global ton failed");
//    }
//
//    return _global;
//}


//const Ton& PST::global() const
//{
//    size_t ig = iglobal();
//    assert(ig < _index.size());
//    return _index.ton(ig);
//}


bool PST::rename(size_t ig)
{
    bool status = true;
    
//    if (! estimatedGlobals())
//    {
//        status = estimateGlobals();
//    }
//
//    if (status && !estimatedLocals())
//    {
//        status = init_locals();
//    }
//
//    if (status && !estimatedGlobal())
//    {
//        status = estimateGlobal();
//    }
//
//    if ((status == false) || (_global == TonIndex::FAILED))
//    {
//        ERROR("PST: failed to estimate global ton. cannot rename pitches {}-{}",
//              _enum.first(), _enum.stop());
//        return false;
//    }

    if ((ig == TonIndex::UNDEF) || (ig == TonIndex::FAILED))
    {
        ERROR("PST: cannot remame {}-{} with global tonality {}",
                _enum.first(), _enum.stop(), ig);
        return false;
    }
    
    assert(ig < _index.size());
    TRACE("PST: rename with estimated global ton {}", _index.ton(ig));
    
    for (size_t i = 0; i < _psvs.size(); ++i)
    {
        assert(_psvs[i]);
        PSV& psv = *(_psvs[i]);
        TRACE("PST: renaming bar {} ({}-{})", i, psv.first(), psv.stop());
        status = status && psv.rename(ig);
    }
    return status;
}


// debug
//bool PST::check_rowcost(const std::vector<PSCost>& rc) const
//{
//    for (bool b : rc)
//    {
//        if (b == true)
//            return true;
//    }
//    return false;
//}


void PST::print(std::ostream& o) const
{
    assert(_rowcost.size() == _index.size());
    std::string SEP(", ");
    std::string SPACE(" ");
    std::string LINE("\n");

    // header : bar numbers
    o << SPACE;
    for (size_t j = 0; j < _psvs.size(); ++j)
    {
        o << SEP;
        o << j;
    }
    o << SEP;
    o << "rowcost";
    o << LINE;

    // rows
    for (size_t i = 0; i < _index.size(); ++i)
    {
        o << _index.ton(i);
        // columns
        // every column of the table corresponds to a measure
        // for (std::unique_ptr<const PSV> psv : _psvs)
        for (size_t j = 0; j < _psvs.size(); ++j)
        {
            o << SEP;
            assert(_psvs[j]);
            PSV& psv = *(_psvs[j]);
            if (psv.undef(i) or psv.bag(i).empty())
            {
                o << SPACE;
            }
            else
            {
                o << psv.bag(i).cost(); // psb.cost().print(o);
            }
        }
        o << SEP;
        assert(_rowcost.at(i));
        o << rowCost(i);
        o << LINE;
    }
}


void PST::dump_rowcost() const
{
    DEBUGU("PST: Row Costs:");
    assert(_rowcost.size() == _index.size());
    for (size_t i = 0; i < _index.size(); ++i)
    {
        assert(_rowcost.at(i));
        DEBUGU("PST row {} cost {}", _index.ton(i), *(_rowcost.at(i)));
    }
}


void PST::dump_table(std::ostream& o) const
{
    DEBUGU("PS Table:");
    assert(_rowcost.size() == _index.size());

    // rows
    for (size_t i = 0; i < _index.size(); ++i)
    {
        std::string srow; // row content (bars)

        // columns
        // every column of the table corresponds to a measure
        // for (std::unique_ptr<const PSV> psv : _psvs)
        for (size_t j = 0; j < _psvs.size(); ++j)
        {
            assert(_psvs[j]);
            PSV& psv = *(_psvs[j]);
            const PSB& psb = psv.bag(i);
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
                std::stringstream st;
                psb.cost().print(st);
                srow += st.str(); // std::to_string(psb.cost().getAccid());
            }
        }
        assert(_rowcost.at(i));
        std::string hrow; // header
        DEBUGU("{} {} {}: {}", i, _index.ton(i), rowCost(i), srow);
    }
}


} // end namespace pse

/// @}
