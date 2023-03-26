//
//  PSChord.cpp
//  pse
//
//  Created by Florent Jacquemard on 05/02/2023.
//

#include "PSChord.hpp"



namespace pse {

// main constructor
PSChord::PSChord(const PSEnum& e, size_t i0):
PSEnum(i0, firstNonSimult(e, i0)),
_enum(e)
{
    assert(i0 <= this->stop());
    assert(this->stop() <= e.stop());
    init();
}


// copy constructor
PSChord::PSChord(const PSChord& rhs):
PSEnum(rhs.first(), rhs.stop()),
_occurences(rhs._occurences),  // array, vector copy
_enum(rhs._enum)
{ }


PSChord::~PSChord()
{ }


void PSChord::init()
{
    assert(first() < stop());
    //unsigned int bass = _enum.midipitch(first());
    size_t bass = first();
    
    // enumerate all notes in this sequence
    for (size_t i = first(); i < stop(); ++i)
    {
        unsigned int pm = _enum.midipitch(i);
        assert(0 <= pm);
        assert(pm <= 127);
        if (pm < _enum.midipitch(bass))
            bass = i;
        int c = pm % 12; // chroma (pitch class) of note, in 0..11
        //_occurences[c].push_back(i-first());
        insert_occurrence(c, i);
    }
    init_constitution(bass);
}


void PSChord::insert_occurrence(int c, size_t i)
{
    std::vector<size_t>& vo = _occurences[c];
    unsigned int pmi = _enum.midipitch(i);
    
    // search the correct position to insert i
    // to have _occurences[c] sorted by pitch
    for (auto it = vo.begin(); it != vo.end(); ++it)
    {
        if (pmi <= _enum.midipitch(*it))
        {
            vo.insert(it, i);
            return;
        }
    }
    // i was not inserted : insert it at the end
    vo.push_back(i);
}


void PSChord::init_constitution(size_t bass)
{
    assert(first() <= bass);
    assert(bass < stop());
    
    // pitch class of bass note, in 0..11
    int c0 = _enum.midipitch(bass) % 12;
    assert(! _occurences[c0].empty());
    for (int c = c0; c < 12; ++c)
    {
        if (! _occurences[c].empty())
        {
            // we add the lowest (first) pitch in occurrence class of c
            /// @todo does the choice of occurrence matter?
            _constitution.push_back(_occurences[c][0]);
        }
    }
    for (int c = 0; c < c0; ++c)
    {
        if (! _occurences[c].empty())
            _constitution.push_back(_occurences[c][0]);
    }
}


// should not be called
std::unique_ptr<PSEnum> PSChord::clone() const
{
    ERROR("PSChord clone");
    return std::make_unique<PSChord>(*this);
}


// should not be called
std::unique_ptr<PSEnum> PSChord::clone(size_t i0) const
{
    ERROR("PSChord clone {}", i0);
    return std::make_unique<PSChord>(*this, i0);
}


// should not be called
std::unique_ptr<PSEnum> PSChord::clone(size_t i0, size_t i1) const
{
    ERROR("PSChord clone {} {}", i0, i1);
    return std::make_unique<PSChord>(*this, i0); // i1 ignored
}


size_t PSChord::occurences(unsigned int c) const
{
    assert(c < 12);
    return _occurences[c].size();
}


size_t PSChord::occurence(unsigned int c, size_t i) const
{
    assert(c < 12);
    assert(i < occurences(c));
    return _occurences[c][i];
}


// static
size_t PSChord::firstNonSimult(const PSEnum& e, size_t i0)
{
    assert(e.inside(i0));
    for (size_t i1 = i0; ; ++i1)
    {
        // end of enumerator reached
        // should not happen because simult flag = false for last note
        if (i1 == e.stop())
        {
            ERROR("PSChord: end of enum");
            return e.stop();
        }

        if (e.simultaneous(i1))
            continue;
        else
            // index of next note or index of the note after the last note of enum
            return i1 + 1;
    }
}


// static
size_t PSChord::_length(const PSEnum& e, size_t i0)
{
    return firstNonSimult(e, i0) - i0;
}


unsigned int PSChord::midipitch(size_t i) const
{
    return _enum.midipitch(i);
}


long PSChord::measure(size_t i) const
{
    return _enum.measure(i);
}


bool PSChord::simultaneous(size_t i) const
{
    return _enum.simultaneous(i);
}


// should not be called. do nothing.
void PSChord::rename(size_t i,
                     const enum NoteName& name, const enum Accid& accid,
                     int oct, bool altprint)
{
    ERROR("PSChord rename {}", i);
}


} // namespace scoremodel

