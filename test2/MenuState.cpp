#include "MenuState.hpp"

void MenuState::create() 
{
    _flames =  new DEmitter(DGlobals::height, DGlobals::width/2); 
    _flames->delay = 0;
    _flames->gravity = -10;
    _flames->maxRotation = 0;
    _flames->setYSpeed(20, 25);
    _flames->setXSpeed(20, -20);

    DSprite *particle = NULL;
    for(unsigned int i = 0;i < MenuState::_max_particles;i++) {
        particle = new DSprite();
        particle->createGraphic(4, 4, 0xff0000ff);
        _flames->add(particle);
    }   

    add(_flames);
    _flames->start(false);
}

void MenuState::update()
{
    DState::update();
}
