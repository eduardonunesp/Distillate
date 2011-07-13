#include "include/DGame.hpp"
#include "include/DPoint.hpp"
#include "include/DGroup.hpp"
#include "include/DState.hpp"
#include "include/DGlobals.hpp"
#include "include/DConsole.hpp"
#include "include/DKeyboard.hpp"
#include "include/DSprite.hpp"
#include "include/DMouse.hpp"

namespace Distillate
{

DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom):
_max_frame_count(10),
_elapsed(0),
_state(InitialState)
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

        if(_state)
        {
           _state->update();
           _state->render();
           SDL_BlitSurface(DGlobals::_buffer, 0, _screen, 0);
        }

        SDL_UpdateRect(_screen, 0,0,0,0);

        SDL_Delay(15);
        unsigned int frametimesindex;
        unsigned int getticks;
        unsigned int count;
        unsigned int i;
        
        frametimesindex = _framecount % _max_frame_count;

        getticks = SDL_GetTicks();
        _frametimes[frametimesindex] = getticks - _frametimelast;
        _frametimelast = getticks;
        _framecount++;
        
        if (_framecount < _max_frame_count) 
            count = _framecount;
        else
            count = _max_frame_count;

        _framespersecond = 0;
        for (i = 0; i < count; i++) 
            _framespersecond += _frametimes[i];

        _framespersecond /= count;
        _framespersecond = 1000.f / _framespersecond;

        DGlobals::elapsed = _frametimelast;
        if(DGlobals::elapsed > DGlobals::maxElapsed)
            DGlobals::elapsed = DGlobals::maxElapsed;
        DGlobals::elapsed *= DGlobals::timeScale;
    }
}

}
