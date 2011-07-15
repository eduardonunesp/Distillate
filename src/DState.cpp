#include "DState.hpp"
#include "DObject.hpp"
#include "DUtils.hpp"
#include "DSprite.hpp"

namespace Distillate
{

unsigned int DState::bgColor;

DState::DState()
{
}

DState::~DState()
{
}

DObject *DState::add(DObject* Core)
{
    return defaultGroup.add(Core);
}

void DState::preProcess()
{
}

void DState::update()
{
    defaultGroup.update();
}

void DState::collide()
{
    DUtils::collide(&defaultGroup, &defaultGroup);
}

void DState::render()
{
    defaultGroup.render();
}

void DState::destroy()
{
    defaultGroup.destroy();
}

}
