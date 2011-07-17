#include "include/DState.hpp"
#include "include/DObject.hpp"
#include "include/DUtils.hpp"
#include "include/DSprite.hpp"

namespace Distillate
{

unsigned int DState::bgColor;

DState::DState(const std::string &Name):
name(Name)
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
