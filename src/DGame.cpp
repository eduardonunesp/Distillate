#include "include/DGame.hpp"
#include "include/DPoint.hpp"
#include "include/DGroup.hpp"
#include "include/DState.hpp"
#include "include/DGlobals.hpp"
#include "include/DConsole.hpp"
#include "include/DKeyboard.hpp"
#include "include/DSprite.hpp"
#include <stdexcept>

namespace Distillate
{
    DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom):
    useDefaultHotKeys(true),
    pause(new DGroup()),
    _created(false),
    _state(InitialState),
    _screen(NULL),
    _zoom(Zoom),
    _gameXOffset(0),
    _gameYOffset(0),
    _elapsed(0),
    _total(0),
    _paused(false),
    _framerate(0),
    _frameratePaused(false),
    _soundTray(new DSprite()),
    _soundTrayTimer(0),
    _console(new DConsole())
    {
        DGlobals::log("CREATE");
        DState::bgColor = 0xff000000;
        DGlobals::setGameData(this, GameSizeX, GameSizeY, Zoom);
        create();
    }

    DGame::~DGame()
    {
        //dtor
        delete pause;
        delete _screen;
        delete _state;
        delete _soundTray;
        delete _console;
    }

    void DGame::switchState(DState* State)
    {
        if(_state)
        {
            _state->destroy();
        }

        _state = State;
        _state->create();
    }

    void DGame::create()
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
            throw std::runtime_error("Cannot initialize SDL");

        _screen = SDL_SetVideoMode(DGlobals::width, DGlobals::height, 16, SDL_HWSURFACE);
        update();
    }

    void DGame::update()
    {
        while(DGlobals::_running)
        {
            while(SDL_PollEvent(&_event))
            {
                switch(_event.type) 
                {
                    case SDL_KEYUP:
                        DGlobals::keys->setKeyState(SDL_KEYUP, _event.key.keysym.sym);
                        break;
                    case SDL_KEYDOWN:
                        DGlobals::keys->setKeyState(SDL_KEYDOWN, _event.key.keysym.sym);
                        break;
                }
            }

            if(_paused)
            {
                pause->update();
            }
            else
            {
                _state->update();
            }
        }
    }
}
