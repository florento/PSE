//
//  PSNoteEnum.cpp
//  qparse
//
//  Created by Florent Jacquemard on 01/10/2022.
//  Copyright © 2022 Florent Jacquemard. All rights reserved.
//
// OBSOLETE.
// replaced by PSRawEnum
// replaced by PSM21Enum

/// @addtogroup pitch
/// @{


#include "PSNoteEnum.hpp"


namespace scoremodel {

PSNoteEnum::PSNoteEnum(NoteEnum& e, size_t i0, size_t i1):
PSEnum(i0, i1),
_enum(e)
{
    assert(i0 <= i1);
}


PSNoteEnum::PSNoteEnum(NoteEnum& e, size_t i0):
PSEnum(i0),
_enum(e)
{ }


PSNoteEnum::PSNoteEnum(const PSNoteEnum& e):
PSEnum(e),
_enum(e._enum)
{ }


std::unique_ptr<pse::PSEnum> PSNoteEnum::clone() const
{
    return std::make_unique<PSNoteEnum>(*this); // copy
}


PSNoteEnum::PSNoteEnum(const PSNoteEnum& e, size_t i0):
PSEnum(e, i0),
_enum(e._enum)
{ }


std::unique_ptr<pse::PSEnum> PSNoteEnum::clone(size_t i0, size_t i1) const
{
    return std::make_unique<PSNoteEnum>(*this, i0, i1);
}


PSNoteEnum::PSNoteEnum(const PSNoteEnum& e, size_t i0, size_t i1):
PSEnum(e, i0, i1),
_enum(e._enum)
{ }


std::unique_ptr<pse::PSEnum> PSNoteEnum::clone(size_t i0) const
{
    return std::make_unique<PSNoteEnum>(*this, i0);
}


PSNoteEnum::~PSNoteEnum()
{
    TRACE("delete PSNoteEnum");
}


/// @todo TBR: def in PSEnum
//size_t PSNoteEnum::first() const
//{
//    return _first;
//}


/// @todo TBR: def in PSEnum
//size_t PSNoteEnum::stop() const
//{
//    //assert(_last != NoteEnum::ID_UNDEF);
//    return _stop;
//}


/// @todo TBR: def in PSEnum
//size_t PSNoteEnum::length() const
//{
//    assert(_stop != NoteEnum::ID_UNDEF);
//    return _stop - _first;
//}


/// @todo TBR: def in PSEnum
//bool PSNoteEnum::empty() const
//{
//    if(_stop == NoteEnum::ID_UNDEF)
//        return (_enum.note(_first) == nullptr);
//    else
//        return _first == _stop;
//}


/// @todo TBR: def in PSEnum
//bool PSNoteEnum::inside(size_t i) const
//{
//    return ((_first <= i) && (i < _stop));
//}


bool PSNoteEnum::inside(size_t i) const
{
    if (i < _first)
        return false;

    if (open())
        return (_enum.note(i) != nullptr);
    else
        return (i < _stop);
}


long PSNoteEnum::measure(size_t i) const
{
    assert(inside(i));
    Note* n = _enum.note(i);
    assert(n);
    return MTU::measure(n->date());
}


unsigned int PSNoteEnum::midipitch(size_t i) const
{
    assert(inside(i));
    Note* n = _enum.note(i);
    assert(n);
    int mp = n->pitch().midi();
    assert(0 <= mp);
    assert(mp <= 128);
    return mp;
}


void PSNoteEnum::rename(size_t i, const pse::NoteName& name,
                        const pse::Accid& accid, int oct, bool altprint)
{
    assert(inside(i));
    assert(defined(name));  // not Undef
    assert(defined(accid)); // not Undef
    Note* n = _enum.note(i);
    assert(n);
    int mp = n->pitch().midi();
    assert(0 <= mp);
    assert(mp <= 128);
    n->namePitch(name, accid, Pitch::midi_to_octave(mp, name, accid), altprint);
}


// TBR
// Note* PSNoteEnum::note(size_t i) const
// {
//     assert(inside(i));
//     return _enum.note(i);
// }


} // end namespace scoremodel

/// @}

