//
//  PSM21Enum.cpp
//  qparse
//
//  Created by Florent Jacquemard on 12/11/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//

#include "PSM21Enum.hpp"



namespace pse {


// import Python Note class from Music 21 module
// py::object M21note = py::module_::import("music21").attr("note");
// py::object Decimal = py::module_::import("decimal").attr("Decimal");


PSM21Enum::PSM21Enum(py::list& ln, const py::list& lb):
PSEnum(0),
_import(ln),
_notes(),    // empty
_barnum()    // empty
{
    int c = 0;
    for (auto item : ln) // py::handle
    {
        py::object pypitch = item.attr("pitch")();
        py::object pymidi = pypitch.attr("midi")();
        int pm = pymidi.cast<int>();
        TRACE("note {}: {}", c, pm);
        _notes.push_back(pm);
        ++c;
    }
    c = 0;
    for (const py::handle item : lb)
    {
        int b = item.cast<int>();
        TRACE("bar of note {}: {}", c, b);
        _barnum.push_back(b);
        ++c;
    }
    assert(_notes.size() == _barnum.size());
}


//PSM21Enum::PSM21Enum(const std::vector<int>& notes,
//                     const std::vector<int>& barnum):
//PSEnum(0),
//_import(py::cast(std::vector<py::object&>())), /// ugly @todo rm
//_notes(notes),  // copy
//_barnum(barnum) // copy
//{
//    assert(_notes.size() == _barnum.size());
//}



PSM21Enum::PSM21Enum(const PSM21Enum& e):
PSEnum(e),
_import(e._import), // copy possible ?
_notes(e._notes),
_barnum(e._barnum)
{ }


std::unique_ptr<pse::PSEnum> PSM21Enum::clone() const
{
    return std::make_unique<PSM21Enum>(*this); // copy
}


PSM21Enum::PSM21Enum(const PSM21Enum& e, size_t i0):
PSEnum(e, i0),
_import(e._import), // copy possible ?
_notes(e._notes),
_barnum(e._barnum)
{ }


std::unique_ptr<pse::PSEnum> PSM21Enum::clone(size_t i0) const
{
    return std::make_unique<PSM21Enum>(*this, i0);
}


PSM21Enum::PSM21Enum(const PSM21Enum& e, size_t i0, size_t i1):
PSEnum(e, i0, i1),
_import(e._import), // copy possible ?
_notes(e._notes),
_barnum(e._barnum)
{ }


std::unique_ptr<pse::PSEnum> PSM21Enum::clone(size_t i0, size_t i1) const
{
    return std::make_unique<PSM21Enum>(*this, i0, i1);
}


PSM21Enum::~PSM21Enum()
{
    TRACE("delete PSM21Enum");
    //_import.clear();
    _notes.clear();
    _barnum.clear();
}


long PSM21Enum::measure(size_t i) const
{
    assert(i < _barnum.size());
    return _barnum[i];
}


unsigned int PSM21Enum::midipitch(size_t i) const
{
    assert(i < _notes.size());
    return _notes[i];
}


void PSM21Enum::rename(size_t i, const pse::NoteName& name,
                       const pse::Accid& accid, int oct, bool altprint)
{
    assert(i < _notes.size());
    const py::object& pynote = _import[i];
    // TBC
    // possible to call non const methods on pynote (dirty) ?
}



} // namespace scoremodel

