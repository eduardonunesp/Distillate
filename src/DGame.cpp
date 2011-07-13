#include "include/DGame.hpp"
#include "include/DPoint.hpp"
#include "include/DGroup.hpp"
#include "include/DState.hpp"
#include "include/DGlobals.hpp"
#include "include/DConsole.hpp"
#include "include/DKeyboard.hpp"
#include "include/DSprite.hpp"
#include "include/Event.hpp"
#include "include/DMouse.hpp"
#include <stdexcept>
#include <functional>

namespace Distillate
{
    DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom):
    _state(InitialState),
    _elapsed(0),
    _total(0)
    {
        DState::bgColor = 0xff000000;
        DGlobals::setGameData(this, GameSizeX, GameSizeY, Zoom);
        create();
    }

    DGame::~DGame()
    {
        delete _screen;
        delete _state;
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

        _screen = SDL_SetVideoMode(DGlobals::width, DGlobals::height, 32, SDL_SWSURFACE);
        switchState(_state);
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
                    case SDL_QUIT:
                        DGlobals::quit();
                        break;
                    case SDL_KEYUP:
                        DGlobals::keys->setKeyState(SDL_KEYUP, _event.key.keysym.sym);
                        break;
                    case SDL_KEYDOWN:
                        DGlobals::keys->setKeyState(SDL_KEYDOWN, _event.key.keysym.sym);
                        break;
                    case SDL_MOUSEMOTION:
                        DGlobals::mouse->setMousePos(_event.motion.x, _event.motion.y);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        DGlobals::mouse->setButtonState(SDL_MOUSEBUTTONUP, _event.button.button);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        DGlobals::mouse->setButtonState(SDL_MOUSEBUTTONDOWN, _event.button.button);
                        break;
                }
            }
            
            unsigned int mark = SDL_GetTicks();
            unsigned int ems = mark - _total;
            _elapsed = ems/1000;
            _total = mark;

            DGlobals::elapsed = _elapsed;
            if(DGlobals::elapsed > DGlobals::maxElapsed)
                DGlobals::elapsed = DGlobals::maxElapsed;
            DGlobals::elapsed *= DGlobals::timeScale;

            if(_state)
            {
               _state->update();
               _state->render();
               SDL_BlitSurface(DGlobals::_buffer, 0, _screen, 0);
            }

            SDL_UpdateRect(_screen, 0,0,0,0);
        }
    }
}
