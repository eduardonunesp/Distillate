#include "include/DGame.hpp"
#include "include/Distillate.hpp"

#ifdef GL_RENDER
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

namespace Distillate
{

DGame::DGame(const std::string &GameTitle, unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom):
_max_frame_count(10),
_elapsed(0),
_lasttime(0),
minFPS(20),
maxFPS(60),
_state(InitialState)
{
    DState::bgColor = 0xff000000;
    DGlobals::setGameData(this, GameTitle, GameSizeX, GameSizeY, Zoom);
    create();
}

DGame::~DGame()
{
#ifdef SDL_RENDER
    delete _screen;
#endif

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
    
    unsigned int flags;

#ifdef GL_RENDER    
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    flags = SDL_OPENGL;
    SDL_SetVideoMode(DGlobals::width, DGlobals::height, 16, flags);
#elif SDL_RENDER
    flags = SDL_SWSURFACE;
    _screen = SDL_SetVideoMode(DGlobals::width, DGlobals::height, 16, flags);
    if(!_screen)
        throw std::runtime_error("Cannot initialize screen");
    if(TTF_Init() < 0)
        throw std::runtime_error("Cannot initialize TTF system");
    atexit(TTF_Quit);
#endif

#ifdef GL_RENDER
    glEnable( GL_TEXTURE_2D );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glViewport( 0, 0, 640, 480 );
    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
#endif

    SDL_WM_SetCaption(DGlobals::gameTitle.c_str(), NULL);
    switchState(_state);
    _lasttime = SDL_GetTicks();
    atexit(SDL_Quit);
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
                    DGlobals::keys.setKeyState(SDL_KEYUP, _event.key.keysym.sym);
                    break;
                case SDL_KEYDOWN:
                    DGlobals::keys.setKeyState(SDL_KEYDOWN, _event.key.keysym.sym);
                    break;
                case SDL_MOUSEMOTION:
                    DGlobals::mouse.setMousePos(_event.motion.x, _event.motion.y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    DGlobals::mouse.setButtonState(SDL_MOUSEBUTTONUP, _event.button.button);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    DGlobals::mouse.setButtonState(SDL_MOUSEBUTTONDOWN, _event.button.button);
                    break;
            }
        }

        _state->update();
        _state->render();
        
#ifdef GL_RENDER       
        SDL_GL_SwapBuffers();
#elif SDL_RENDER
        SDL_BlitSurface(DGlobals::_buffer, 0, _screen, 0);
        SDL_UpdateRect(_screen, 0,0,0,0);
        SDL_FillRect(DGlobals::_buffer,0, DState::bgColor);
#endif

        unsigned int now;
        _frametime = 0;
        
        do
        {
            now = SDL_GetTicks();
            _frametime = (now > _lasttime) ? now - _lasttime : 0;
            _lasttime  = (now >= _lasttime) ? _lasttime : now;
        }
        while(!(_frametime >= minFPS));

        if(_frametime > maxFPS)
            _frametime = maxFPS;

        _timeaccum += +_frametime;
        _framecount++;
        _elapsed = (float) _frametime * 0.001f;

        if(_timeaccum >= 1000)
        {
            DGlobals::FPS = _frametime;    
            _framecount   = 0;
            _timeaccum    = 0;
        }

        DGlobals::elapsed = _elapsed;
        _lasttime = now;
    }
}

}
