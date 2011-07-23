#include "include/State.hpp"
#include "include/Object.hpp"
#include "include/Utils.hpp"
#include "include/Sprite.hpp"

NAMESPACE_BEGIN

unsigned int State::bgColor;

State::State(const std::string &Name):
    name(Name)
{
}

State::~State()
{
}

Object *State::add(Object* Core)
{
#ifdef DEBUG
    fprintf(stdout, "Added object\n");
#endif
    return defaultGroup.add(Core);
}

void State::preProcess()
{
}

void State::update()
{
    defaultGroup.update();
}

void State::collide()
{
    Utils::collide(&defaultGroup, &defaultGroup);
}

void State::render()
{
    defaultGroup.render();
}

void State::destroy()
{
    defaultGroup.destroy();
}
}

NAMESPACE_END
