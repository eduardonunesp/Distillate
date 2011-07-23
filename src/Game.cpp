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

#include "include/Game.hpp"
#include "include/State.hpp"
#include "include/Globals.hpp"
#include "include/Keyboard.hpp"
#include "include/Mouse.hpp"
#include <cstdio>

NAMESPACE_BEGIN

Game::Game(const std::string &GameTitle):
#if defined(SDL_ENGINE)
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
    fprintf(stdout, "Game constructor\n");
#endif
    Globals::gameTitle = GameTitle;
}

Game::~Game()
{
#ifdef DEBUG
    fprintf(stdout, "Game destructor\n");
#endif

#if defined(SDL_ENGINE)
    SDL_FreeSurface(_screen);
#elif defined(GL_ENGINE) && defined(__linux__)         
    if(_GLWin.ctx)
    {
        if(!glXMakeCurrent(_GLWin.dpy, None, NULL))
        {
            fprintf(stderr, "Error releasing drawing context\n");
        }

        glXDestroyContext(_GLWin.dpy, _GLWin.ctx);
        _GLWin.ctx = NULL;
    }

    if(_GLWin.fs)
    {
        XF86VidModeSwitchToMode(_GLWin.dpy, _GLWin.screen, &_GLWin.deskMode);
        XF86VidModeSetViewPort(_GLWin.dpy, _GLWin.screen, 0, 0);
    }

    XCloseDisplay(_GLWin.dpy);
#endif
}

bool Game::setup(unsigned int GameSizeX, unsigned int GameSizeY, unsigned int BPP)
{
    Globals::setGameData(this, GameSizeX, GameSizeY, BPP);
    State::bgColor = 0xff000000;

#if defined(SDL_ENGINE)
#ifdef DEBUG
    fprintf(stdout, "Initializing SDL Everything\n");
#endif
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        _failtype = -1;
        fprintf(stderr, "Cannot initialize SDL\n");
        return false;
    }

#ifdef DEBUG
    fprintf(stdout, "Setup Game (W: %d H: %d BPP: %d) \n", GameSizeX, GameSizeY, BPP);
#endif
    _screen = SDL_SetVideoMode(Globals::width, Globals::height, BPP, SDL_SWSURFACE);

    if(!_screen) {
        _failtype = -1;
        fprintf(stderr, "Cannot initialize Video Mode\n");
        return false;
    }

    if(TTF_WasInit() == 0) {
#ifdef DEBUG
        fprintf(stdout, "Initializing SDL_TTF\n");
#endif
        if(TTF_Init() < 0) {
            _failtype = -1;
            fprintf(stderr, "Cannot initialize TTF system\n");
            return false;
        }
    }

    SDL_WM_SetCaption(Globals::gameTitle.c_str(), NULL);

    atexit(SDL_Quit);
    atexit(TTF_Quit);

#elif defined(GL_ENGINE) && defined(__linux__) 
    int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 4,
        GLX_GREEN_SIZE, 4,
        GLX_BLUE_SIZE, 4,
        GLX_DEPTH_SIZE, 16, 
        None};

    int attrListDbl[] = {GLX_RGBA, GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 4,
        GLX_GREEN_SIZE, 4,
        GLX_BLUE_SIZE, 4,
        GLX_DEPTH_SIZE, 16, 
        None};

    XVisualInfo *vi;
    Colormap cmap;
    int dpyWidth, dpyHeight;
    int i;
    int vidModeMajorVersion, vidModeMinorVersion;
    XF86VidModeModeInfo **modes;
    int modeNum;
    int bestMode;
    Atom wmDelete;
    Window winDummy;
    unsigned int borderDummy;

    _GLWin.fs = false;
    bestMode = 0;

    _GLWin.dpy = XOpenDisplay(0);
    _GLWin.screen = DefaultScreen(_GLWin.dpy);
    XF86VidModeQueryVersion(_GLWin.dpy, &vidModeMajorVersion,
            &vidModeMinorVersion);

    XF86VidModeGetAllModeLines(_GLWin.dpy, _GLWin.screen, &modeNum, &modes);

    _GLWin.deskMode = *modes[0];

    for (i = 0; i < modeNum; i++)
    {
        if ((modes[i]->hdisplay == GameSizeX) && (modes[i]->vdisplay == GameSizeY))
        {
            bestMode = i;
        }
    }

    vi = glXChooseVisual(_GLWin.dpy, _GLWin.screen, attrListDbl);
    if(NULL == vi)
    {
        vi = glXChooseVisual(_GLWin.dpy, _GLWin.screen, attrListSgl);
        _GLWin.doublebuffer = false;
    }
    else
    {
        _GLWin.doublebuffer = true;
    }

    _GLWin.ctx = glXCreateContext(_GLWin.dpy, vi, 0, GL_TRUE);

    cmap = XCreateColormap(_GLWin.dpy, RootWindow(_GLWin.dpy, vi->screen),vi->visual, AllocNone);
    _GLWin.attr.colormap = cmap;
    _GLWin.attr.border_pixel = 0;

    if(_GLWin.fs)
    {
        XF86VidModeSwitchToMode(_GLWin.dpy, _GLWin.screen, modes[bestMode]);
        XF86VidModeSetViewPort(_GLWin.dpy, _GLWin.screen, 0, 0);
        dpyWidth = modes[bestMode]->hdisplay;
        dpyHeight = modes[bestMode]->vdisplay;
        XFree(modes);
        _GLWin.attr.override_redirect = True;
        _GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
            StructureNotifyMask;
        _GLWin.win = XCreateWindow(_GLWin.dpy, RootWindow(_GLWin.dpy, vi->screen),
                0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
                CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                &_GLWin.attr);
        XWarpPointer(_GLWin.dpy, None, _GLWin.win, 0, 0, 0, 0, 0, 0);
        XMapRaised(_GLWin.dpy, _GLWin.win);
        XGrabKeyboard(_GLWin.dpy, _GLWin.win, True, GrabModeAsync,GrabModeAsync, CurrentTime);
        XGrabPointer(_GLWin.dpy, _GLWin.win, True, ButtonPressMask,
                GrabModeAsync, GrabModeAsync, _GLWin.win, None, CurrentTime);
    }
    else
    {
        _GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
            StructureNotifyMask;
        _GLWin.win = XCreateWindow(_GLWin.dpy, RootWindow(_GLWin.dpy, vi->screen),
                0, 0, GameSizeX, GameSizeY, 0, vi->depth, InputOutput, vi->visual,
                CWBorderPixel | CWColormap | CWEventMask, &_GLWin.attr);
        wmDelete = XInternAtom(_GLWin.dpy, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(_GLWin.dpy, _GLWin.win, &wmDelete, 1);
        XSetStandardProperties(_GLWin.dpy, _GLWin.win, Globals::gameTitle.c_str(),
                Globals::gameTitle.c_str(), None, NULL, 0, NULL);
        XMapRaised(_GLWin.dpy, _GLWin.win);
    }

    glXMakeCurrent(_GLWin.dpy, _GLWin.win, _GLWin.ctx);
    XGetGeometry(_GLWin.dpy, _GLWin.win, &winDummy, &_GLWin.x, &_GLWin.y,
            &_GLWin.width, &_GLWin.height, &borderDummy, &_GLWin.bpp);

    glEnable( GL_TEXTURE_2D );
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, Globals::width, Globals::height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)Globals::width / (GLfloat)Globals::height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    XSetStandardProperties(_GLWin.dpy, _GLWin.win,Globals::gameTitle.c_str(),Globals::gameTitle.c_str(), None, NULL, 0, NULL);
#endif

    return true;
}

void Game::add(State *State, bool Curr)
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

bool Game::switchState(const std::string &Name)
{
    State *new_state = _states[Name];
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

int Game::run()
{
    if (!_state) {
        fprintf(stderr, "No state provided \n");
        return -1;
    }

    if (_failtype < 0)
        return _failtype;

    while(Globals::_running) {
#if defined(SDL_ENGINE)
        while(SDL_PollEvent(&_event)) {
            switch(_event.type) {
                case SDL_QUIT:
                    Globals::quit();
#ifdef DEBUG
                    fprintf(stdout, "Quit pressed\n");
#endif
                    break;
                case SDL_KEYUP:
                    Globals::keys.setKeyState(Keyboard::Key::State::RELEASED, _event.key.keysym.sym);
                    break;
                case SDL_KEYDOWN:
                    Globals::keys.setKeyState(Keyboard::Key::State::PRESSED, _event.key.keysym.sym);
                    break;
                case SDL_MOUSEMOTION:
                    Globals::mouse.setMousePos(_event.motion.x, _event.motion.y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    Globals::mouse.setButtonState(SDL_MOUSEBUTTONUP, _event.button.button);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    Globals::mouse.setButtonState(SDL_MOUSEBUTTONDOWN, _event.button.button);
                    break;
            }
        }

#elif defined(GL_ENGINE) && defined(__linux__)
        while(XPending(_GLWin.dpy) > 0)
        {   
            XNextEvent(_GLWin.dpy, &_event);
            switch(_event.type)
            {   
                case Expose:
                    if (_event.xexpose.count != 0)
                        break;
                    break;
                case ConfigureNotify:
                    if ((_event.xconfigure.width  != (signed) _GLWin.width) ||
                            (_event.xconfigure.height != (signed) _GLWin.height))
                    {   
                        _GLWin.width  = _event.xconfigure.width;
                        _GLWin.height = _event.xconfigure.height;

#ifdef DEBUG
                        fprintf(stdout, "Resize Event\n");
#endif                              
                    }   
                    break;
                case KeyPress:
                    Globals::keys.setKeyState(Keyboard::Key::State::PRESSED, XLookupKeysym(&_event.xkey,0));
                    break;
                case ClientMessage:
                    if (*XGetAtomName(_GLWin.dpy, _event.xclient.message_type) == *"WM_PROTOCOLS")
                    {   
#ifdef DEBUG
                        fprintf(stdout, "Quit pressed\n");
#endif
                        Globals::quit();
                    }   
                    break;
                default:
                    break;
            }   
        }   
#endif

        if(_state) {
            _state->update();
            _state->render();
        } else {
            fprintf(stderr, "State not found\n");
        }

#if defined(GL_ENGINE)
        if(_GLWin.doublebuffer)
            glXSwapBuffers(_GLWin.dpy, _GLWin.win);
        else
            glFlush();
        glClearColor(0,0,0,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
#elif defined(SDL_ENGINE)
        SDL_BlitSurface(Globals::_buffer, 0, _screen, 0);
        SDL_UpdateRect(_screen, 0,0,0,0);
        SDL_FillRect(Globals::_buffer,0, State::bgColor);
#endif

        unsigned int now;
        _frametime = 0;

        do {
#if defined(GL_ENGINE) && defined(__linux__)
            timeb tb;
            ftime( &tb );
            now = tb.millitm + (tb.time & 0xfffff) * 1000;
#elif defined(SDL_ENGINE)
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
            Globals::FPS = _frametime;
            _framecount   = 0;
            _timeaccum    = 0;
        }

        Globals::elapsed = _elapsed;
        _lasttime = now;
    }

    return 0;
}

NAMESPACE_END
