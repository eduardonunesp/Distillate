#include "include/DGame.hpp"
#include "include/DState.hpp"
#include "include/DGlobals.hpp"
#include "include/DKeyboard.hpp"
#include "include/DMouse.hpp"

#if defined(GL_RENDER)
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif

namespace Distillate {
    DGame::DGame(const std::string &GameTitle):
#if defined(SDL_RENDER)
         _screen(NULL),
#endif
         _state(NULL),
         _failtype(0),
         _max_frame_count(10),
         _elapsed(0),
         _lasttime(0),
         _timeaccum(0),
         minFPS(20),
         maxFPS(60)
    {
#ifdef DEBUG
         fprintf(stdout, "DGame constructor\n");
#endif
         DGlobals::gameTitle = GameTitle;
    }

    DGame::~DGame()
    {
#ifdef DEBUG
         fprintf(stdout, "DGame destructor\n");
#endif

#if defined(SDL_RENDER)
         SDL_FreeSurface(_screen);
#endif         
    }

    bool DGame::setup(unsigned int GameSizeX, unsigned int GameSizeY, unsigned int BPP)
    {
#if defined(SDL_RENDER) || defined(GL_RENDER) || defined(SDL_INPUT)
#if defined(SDL_RENDER) || GL_RENDER        
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
#elif defined(SDL_INPUT)
        if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) 
#endif
#endif
        {
              _failtype = -1;
              fprintf(stderr, "Cannot initialize SDL\n");
              return false;
        }

         DState::bgColor = 0xff000000;
         DGlobals::setGameData(this, GameSizeX, GameSizeY, 2);
#ifdef DEBUG
         fprintf(stdout, "Setup Game (W: %d H: %d BPP: %d) \n", GameSizeX, GameSizeY, BPP);
#endif

#if defined(GL_RENDER)
#ifdef DEBUG
         fprintf(stdout, "GL_RENDER\n");
#endif
#elif defined(SDL_RENDER)
#ifdef DEBUG
         fprintf(stdout, "SDL_RENDER\n");
#endif
         unsigned int flags = SDL_SWSURFACE;
         _screen = SDL_SetVideoMode(DGlobals::width, DGlobals::height, BPP, flags);

         if(!_screen) {
              _failtype = -1;
              fprintf(stderr, "Cannot initialize Video Mode\n");
              return false;
         }

         if(TTF_Init() < 0) {
              _failtype = -1;
              fprintf(stderr, "Cannot initialize TTF system\n");
              return false;
         }


         SDL_WM_SetCaption(DGlobals::gameTitle.c_str(), NULL);

         atexit(SDL_Quit);
         atexit(TTF_Quit);
#endif

#if defined(GL_RENDER)
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

#ifdef DEBUG
         fprintf(stdout, "Setup for %s ok!\n", DGlobals::gameTitle.c_str());
#endif

         return true;
    }

    void DGame::add(DState *State, bool Curr)
    {
         if(!State) {
              fprintf(stderr, "Invalid State passed [%p]\n", (void*) State);
              return;
         }

         _states[State->name] = State;
#ifdef DEBUG
         fprintf(stdout, "State %s added\n", State->name.c_str());
#endif

         if(Curr) switchState(State->name);
    }

    bool DGame::switchState(const std::string &Name)
    {
         DState *new_state = _states[Name];
         if(!new_state) {
              fprintf(stderr, "State '%s' not found", Name.c_str());
              return false;
         }

         _state = new_state;
         _state->create();

#ifdef DEBUG
         fprintf(stdout, "Current state has switched for state %s\n", new_state->name.c_str());
#endif

         return true;
    }

    int DGame::run()
    {
         if (!_state) {
              fprintf(stderr, "No state provided \n");
              return -1;
         }

         if (_failtype < 0)
              return _failtype;

         while(DGlobals::_running) {
#if defined(SDL_RENDER) || defined(SDL_INPUT)
              while(SDL_PollEvent(&_event)) {
                   switch(_event.type) {
                   case SDL_QUIT:
                        DGlobals::quit();
#ifdef DEBUG
                        fprintf(stdout, "Quit pressed\n");
#endif
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
#endif

              if(_state) {
                   //_state->update();
                   //_state->render();
              } else {
                   fprintf(stderr, "State not found\n");
              }

#if defined(GL_RENDER)
#elif defined(SDL_RENDER)
              SDL_BlitSurface(DGlobals::_buffer, 0, _screen, 0);
              SDL_UpdateRect(_screen, 0,0,0,0);
              SDL_FillRect(DGlobals::_buffer,0, DState::bgColor);
#endif

              unsigned int now;
              _frametime = 0;

              do {
#if defined(GL_RENDER)
                   now = 0;
#elif defined(SDL_RENDER)
                   now = SDL_GetTicks();
#endif
                   _frametime = (now > _lasttime) ? now - _lasttime : 0;
                   _lasttime  = (now >= _lasttime) ? _lasttime : now;
              } while(!(_frametime >= minFPS));

              if(_frametime > maxFPS)
                   _frametime = maxFPS;

              _timeaccum += _frametime;
              _framecount++;
              _elapsed = (float) _frametime * 0.001f;

              if(_timeaccum >= 1000) {
                   DGlobals::FPS = _frametime;
                   _framecount   = 0;
                   _timeaccum    = 0;
              }

              DGlobals::elapsed = _elapsed;
              _lasttime = now;
         }

         return 0;
    }
}
