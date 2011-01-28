#include "Distillate/DState.hpp"
#include "Distillate/DGroup.hpp"
#include "Distillate/DObject.hpp"
#include "Distillate/DUtils.hpp"

namespace Distillate
{

DState::DState()
{
    //ctor
}

DState::~DState()
{
    //dtor
}

DObject *DState::add(DObject* Core)
{
    return defaultGroup->add(Core);
}

void DState::preProcess()
{
    //screen->fill(bgColor);   //Default behavior - just overwrite buffer with background color
}

void DState::update()
{
    defaultGroup->update();
}

void DState::collide()
{
    DUtils::collide(defaultGroup,defaultGroup);
}

void DState::render()
{
    defaultGroup->render();
}

void DState::destroy()
{
    defaultGroup->destroy();
}

}
