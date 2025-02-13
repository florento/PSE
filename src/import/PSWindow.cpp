//
//  PSWindow.cpp
//  pse
//
//  Created by Florent Jacquemard on 13/02/2025.
//

#include "PSWindow.hpp"


namespace pse {


PSWindow::PSWindow(PSEnum& e, size_t i0, size_t i1):
PSEnum(i0, i1),
_container(e)
{
    assert(i0 != ID_INF);
    assert(i1 != ID_INF);
    assert(i0 <= i1);
    assert(e.inside(i0));
    assert(i1 == e.stop() or e.inside(i1));
}


PSWindow::PSWindow(const PSWindow& rhs):
PSEnum(rhs),
_container(rhs._container)
{ }


PSWindow::~PSWindow()
{ }


/// @todo only for overriding method od PSEnum. useless.
std::unique_ptr<PSEnum> PSWindow::clone() const
{
    assert(not open());
    return std::unique_ptr<PSWindow>(new PSWindow(_container, first(), stop()));
}


/// @todo only for overriding method od PSEnum. useless.
std::unique_ptr<PSEnum> PSWindow::clone(size_t i0) const
{
    assert(not open());
    return std::unique_ptr<PSWindow>(new PSWindow(_container, i0, stop()));
}


/// @todo only for overriding method od PSEnum. useless.
std::unique_ptr<PSEnum> PSWindow::clone(size_t i0, size_t i1) const
{
    return std::unique_ptr<PSWindow>(new PSWindow(_container, i0, i1));
}


unsigned int PSWindow::midipitch(size_t i) const
{
    return _container.midipitch(i);
}


long PSWindow::measure(size_t i) const
{
    return _container.measure(i);
}


bool PSWindow::simultaneous(size_t i) const
{
    return _container.simultaneous(i);
}


enum NoteName PSWindow::name(size_t i) const
{
    return _container.name(i);
}


enum Accid PSWindow::accidental(size_t i) const
{
    return _container.accidental(i);
}


int PSWindow::octave(size_t i) const
{
    return _container.octave(i);
}


long PSWindow::duration_num(size_t i) const
{
    return _container.duration_num(i);
}


long PSWindow::duration_den(size_t i) const
{
    return _container.duration_den(i);
}


bool PSWindow::printed(size_t i) const
{
    return _container.printed(i);
}
    

void PSWindow::reset(size_t i0, size_t i1)
{
    assert(i0 != ID_INF);
    assert(i1 != PSEnum::ID_INF);
    assert(i0 <= i1);
    PSEnum::reset(i0, i1);
}



void PSWindow::rename(size_t i, const enum NoteName& name,
                      const enum Accid& accid, int oct, bool printed)
{
    _container.rename(i, name, accid, oct, printed);
}

} // end namespace pse
