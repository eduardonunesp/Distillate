#include "FlameState.hpp"

void FlameState::create() 
{
    _flames_y =  new DEmitter(DGlobals::height, DGlobals::width/2); 
    _flames_y->delay = 0.2;
    _flames_y->gravity = 1000;
    _flames_y->maxRotation = 0;
    _flames_y->setYSpeed(-300,-500);
    _flames_y->setXSpeed(-20, 20);

    DSprite *particle = NULL;
    for(unsigned int i = 0;i < FlameState::_max_particles;i++) {
        particle = new DSprite();
        particle->createGraphic(4, 4, 0x9900ffff);
        _flames_y->add(particle);
    }   

    add(_flames_y);
    _flames_y->start(false);

    _flames_r =  new DEmitter(DGlobals::height, DGlobals::width/2); 
    _flames_r->delay = 0.5;
    _flames_r->gravity = 1000;;
    _flames_r->maxRotation = 0;
    _flames_r->setYSpeed(-300,- 500);
    _flames_r->setXSpeed(-20, 20);

    for(unsigned int i = 0;i < FlameState::_max_particles;i++) {
        particle = new DSprite();
        particle->createGraphic(4, 4, 0x990000ff);
        _flames_r->add(particle);
    }   

    add(_flames_r);
    _flames_r->start(false);

    DState::bgColor = 0xff999999;
}

void FlameState::update()
{
    DState::update();

    if(DGlobals::keys.checkKeyState(SDL_KEYDOWN, SDLK_ESCAPE))
        DGlobals::quit();
}
