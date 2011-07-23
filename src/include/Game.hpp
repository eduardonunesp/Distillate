/**
 * Copyright (c) 2010 - 2011 Distillate Project
 *
 *  ______ ________________________            _____________________
 *  |     \  |  |______   |     |  |     |     |_____|   |   |______
 *  |_____/__|________|   |   __|__|_____|_____|     |   |   |______
 *
 *
 * License: BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Wintermoon nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GAME_HPP__
#define __GAME_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#endif

#if defined(__linux__) && defined(GL_ENGINE)
#include <cstdio>
#include <cstdlib>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>        
#include <sys/timeb.h> 
#endif

#include <string>
#include <map>
#include "Defs.hpp"

NAMESPACE_BEGIN

/* Forwards */
class Point;
class State;

/**
 * Game is the heart of Distillate
 * all Distillate games, and contains a bunch of basic game loops and things.
 * It is a long and sloppy file that you shouldn't have to worry about too much!
 * It is basically only used to create your game object in the first place,
 * after that Globals and State have all the useful stuff you actually need.
 */
class Game {
    friend class Globals;
    typedef std::map<std::string, State*> States;
private:
    /* Display & Event stuff */
#if defined(SDL_ENGINE)
    SDL_Surface* _screen;
    SDL_Event _event;
#endif

#if defined(GL_ENGINE) && defined(__linux__)
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

    GLWindow _GLWin;
    XEvent _event;
#endif

    /* State stuff */
    State* _state;
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
    Game(const std::string &GameTitle);
    virtual ~Game();

    /**
     * Used to instantiate the guts of Distillate once we have a valid pointer to the root.
     */
    bool setup(unsigned int GameSizeX, unsigned int GameSizeY, unsigned int BPP);

    /**
     * Used to states
     */
    void add(State *State, bool Curr = false);

    /**
     * Switch from one <code>State</code> to another.
     * Usually called from <code>Globals</code>.
     *
     * @param	State		The class name of the state you want (e.g. PlayState)
     */
    bool switchState(const std::string &Name);

    /**
     * This is the main game loop.  It controls all the updating and rendering.
     */
    int run();
};

NAMESPACE_END
#endif  /* __GAME_HPP__ */
