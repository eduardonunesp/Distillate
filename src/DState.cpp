#include "DState.hpp"
#include "DGroup.hpp"
#include "DObject.hpp"
#include "DUtils.hpp"
#include "DSprite.hpp"

namespace Distillate
{
    unsigned int DState::bgColor;

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
