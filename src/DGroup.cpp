#include "Distillate/DGroup.hpp"
#include "Distillate/DPoint.hpp"
#include "Distillate/DUtils.hpp"

namespace Distillate
{

DGroup::DGroup():
    _last(new DPoint())
{

}

DGroup::~DGroup()
{
    delete _last;
    delete_all(members);
}

void DGroup::render()
{
}

void DGroup::update()
{
}

void DGroup::destroy()
{
}

DObject* DGroup::add(DObject *Object, bool ShareScroll)
{
    return NULL;
}

}
