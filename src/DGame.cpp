#include "Distillate/DGame.hpp"
#include "Distillate/DPoint.hpp"
#include "Distillate/DGroup.hpp"
#include "Distillate/DState.hpp"
#include "Distillate/DGlobals.hpp"
#include "Distillate/Data/DConsole.hpp"
#include "Distillate/SDL/SDL.hpp"
#include <stdexcept>

namespace Distillate
{
    DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom):
    useDefaultHotKeys(true),
    pause(new DGroup()),
    _iState(InitialState),
    _created(false),
    _state(NULL),
    _screen(new SDL::Sprite()),
    _zoom(Zoom),
    _gameXOffset(0),
    _gameYOffset(0),
    _zeroPoint(new SDL::Point),
    _elapsed(0),
    _total(0),
    _paused(false),
    _framerate(0),
    _frameratePaused(false),
    _soundTray(new DSprite()),
    _soundTrayTimer(0),
    _console(new DConsole())
    {
        DState::bgColor = 0xff000000;
        DGlobals::setGameData(this, GameSizeX, GameSizeY, Zoom);
        create();
    }

    DGame::~DGame()
    {
        //dtor
        delete pause;
        delete _screen;
        delete _zeroPoint;
        delete _iState;
        delete _soundTray;
        delete _console;
    }

    void DGame::create()
    {
        if(!SDL::initSDL()) throw std::runtime_error("Cannot initialize SDL");
        unsigned int i;
        unsigned int l;
        _screen->buffer(SDL::setVideoMode(DGlobals::width, DGlobals::height));
        update();
    }

    void DGame::update()
    {
        while(true)
        {
            SDL::pollEvent();
            //Frame timing
            unsigned int mark = SDL::getTimer();
            unsigned int ems = mark-_total;
            _elapsed = ems/1000;
            _total = mark;
            DGlobals::elapsed = _elapsed;
            if(DGlobals::elapsed > DGlobals::maxElapsed)
                DGlobals::elapsed = DGlobals::maxElapsed;
            DGlobals::elapsed *= DGlobals::timeScale;
        }
    }
}
