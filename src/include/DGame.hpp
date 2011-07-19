#ifndef __DGAME_HPP__
#define __DGAME_HPP__

/* SDL already cross */
#if defined(SDL_RENDER)
#include <SDL/SDL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#endif

/* If we only use the INPUT system */
#if defined(SDL_INPUT)
#include <SDL/SDL_events.h>    
#endif

/* Make it cross */
#if defined(__linux__) && defined(GL_RENDER)
#include <cstdio>
#include <cstdlib>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>        
#endif

#include <string>
#include <map>

namespace Distillate {
    /* Forwards */
    class DPoint;
    class DState;

    /**
     * DGame is the heart of Distillate
     * all Distillate games, and contains a bunch of basic game loops and things.
     * It is a long and sloppy file that you shouldn't have to worry about too much!
     * It is basically only used to create your game object in the first place,
     * after that DGlobals and DState have all the useful stuff you actually need.
     */
    class DGame {
         friend class DGlobals;
         typedef std::map<std::string, DState*> States;
    private:
         /* Display & Event stuff */
#if defined(SDL_RENDER)
         SDL_Surface* _screen;
#endif

#if defined(SDL_INPUT) || defined(SDL_RENDER)
         SDL_Event _event;
#endif

#if defined(GL_RENDER) && defined(__linux__)
         typedef struct {
             Display *dpy;
             int screen;
             Window win;
             GLXContext ctx;
             XSetWindowAttributes attr;
             bool fs; 
             bool doublebuffer;
             Colormap cmap;
             int x, y;
             unsigned int width, height;
             unsigned int bpp;
             int dummy;
             GLboolean needRedraw;
             XF86VidModeModeInfo deskMode;
         } GLWindow;

        GLWindow GLWin;
        XEvent _event;
#endif

         /* State stuff */
         DState* _state;
         States _states;

         /* Fail result */
         int _failtype;

         /* FPS private stuff */
         const unsigned int _max_frame_count;
         float _elapsed;
         unsigned int _lasttime;
         unsigned int _frametime;
         unsigned int _framecount;
         unsigned int _timeaccum;

    protected:
         /* FPS protected stuff */
         unsigned int minFPS;
         unsigned int maxFPS;

    public:
         /**
          * Game object constructor - sets up the basic properties of your game.
          *
          * @param	GameSizeX		The width of your game in pixels (e.g. 320).
          * @param	GameSizeY		The height of your game in pixels (e.g. 240).
          * @param	InitialState	The class name of the state you want to create and switch to first (e.g. MenuState).
          * @param	Zoom			The level of zoom (e.g. 2 means all pixels are now rendered twice as big).
          */
         DGame(const std::string &GameTitle);
         virtual ~DGame();

         /**
          * Used to instantiate the guts of flixel once we have a valid pointer to the root.
          */
         bool setup(unsigned int GameSizeX, unsigned int GameSizeY, unsigned int BPP);

         /**
          * Used to states
          */
         void add(DState *State, bool Curr = false);

         /**
          * Switch from one <code>DState</code> to another.
          * Usually called from <code>DG</code>.
          *
          * @param	State		The class name of the state you want (e.g. PlayState)
          */
         bool switchState(const std::string &Name);

         /**
          * This is the main game loop.  It controls all the updating and rendering.
          */
         int run();
    };
}
#endif  /* __DGAME_HPP__ */
