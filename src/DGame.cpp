#include "include/DGame.hpp"
#include "include/DState.hpp"
#include "include/DGlobals.hpp"
#include "include/DKeyboard.hpp"
#include "include/DMouse.hpp"

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
#elif defined(GL_RENDER) && defined(__linux__)         
        if(GLWin.ctx)
        {
            if(!glXMakeCurrent(GLWin.dpy, None, NULL))
            {
                fprintf(stderr, "Error releasing drawing context\n");
            }

            glXDestroyContext(GLWin.dpy, GLWin.ctx);
            GLWin.ctx = NULL;
        }

        if(GLWin.fs)
        {
            XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, &GLWin.deskMode);
            XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
        }

        XCloseDisplay(GLWin.dpy);
#endif
    }

    bool DGame::setup(unsigned int GameSizeX, unsigned int GameSizeY, unsigned int BPP)
    {
        DGlobals::setGameData(this, GameSizeX, GameSizeY, 2);
        DState::bgColor = 0xff000000;

#if defined(SDL_RENDER)
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
        _screen = SDL_SetVideoMode(DGlobals::width, DGlobals::height, BPP, SDL_SWSURFACE);

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

        SDL_WM_SetCaption(DGlobals::gameTitle.c_str(), NULL);

        atexit(SDL_Quit);
        atexit(TTF_Quit);
#elif defined(SDL_INPUT)
        if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) 
        {
            _failtype = -1;
            fprintf(stderr, "Cannot initialize SDL\n");
            return false;
        }

        atexit(SDL_Quit);
#endif         

#if defined(__linux__) && defined(GL_RENDER)
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

        GLWin.fs = false;
        bestMode = 0;

        GLWin.dpy = XOpenDisplay(0);
        GLWin.screen = DefaultScreen(GLWin.dpy);
        XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion,
                &vidModeMinorVersion);

        XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);

        GLWin.deskMode = *modes[0];

        for (i = 0; i < modeNum; i++)
        {
            if ((modes[i]->hdisplay == GameSizeX) && (modes[i]->vdisplay == GameSizeY))
            {
                bestMode = i;
            }
        }

        vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListDbl);
        if(NULL == vi)
        {
            vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListSgl);
            GLWin.doublebuffer = false;
        }
        else
        {
            GLWin.doublebuffer = true;
        }

        GLWin.ctx = glXCreateContext(GLWin.dpy, vi, 0, GL_TRUE);

        cmap = XCreateColormap(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),vi->visual, AllocNone);
        GLWin.attr.colormap = cmap;
        GLWin.attr.border_pixel = 0;

        if(GLWin.fs)
        {
            XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, modes[bestMode]);
            XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
            dpyWidth = modes[bestMode]->hdisplay;
            dpyHeight = modes[bestMode]->vdisplay;
            XFree(modes);
            GLWin.attr.override_redirect = True;
            GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
                StructureNotifyMask;
            GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
                    0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
                    CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                    &GLWin.attr);
            XWarpPointer(GLWin.dpy, None, GLWin.win, 0, 0, 0, 0, 0, 0);
            XMapRaised(GLWin.dpy, GLWin.win);
            XGrabKeyboard(GLWin.dpy, GLWin.win, True, GrabModeAsync,GrabModeAsync, CurrentTime);
            XGrabPointer(GLWin.dpy, GLWin.win, True, ButtonPressMask,
                    GrabModeAsync, GrabModeAsync, GLWin.win, None, CurrentTime);
        }
        else
        {
            GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
                StructureNotifyMask;
            GLWin.win = XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
                    0, 0, GameSizeX, GameSizeY, 0, vi->depth, InputOutput, vi->visual,
                    CWBorderPixel | CWColormap | CWEventMask, &GLWin.attr);
            wmDelete = XInternAtom(GLWin.dpy, "WM_DELETE_WINDOW", True);
            XSetWMProtocols(GLWin.dpy, GLWin.win, &wmDelete, 1);
            XSetStandardProperties(GLWin.dpy, GLWin.win, DGlobals::gameTitle.c_str(),
                    DGlobals::gameTitle.c_str(), None, NULL, 0, NULL);
            XMapRaised(GLWin.dpy, GLWin.win);
        }

        glXMakeCurrent(GLWin.dpy, GLWin.win, GLWin.ctx);
        XGetGeometry(GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
                &GLWin.width, &GLWin.height, &borderDummy, &GLWin.bpp);

        glEnable( GL_TEXTURE_2D );
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_2D);

        glViewport(0, 0, DGlobals::width, DGlobals::height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (GLfloat)DGlobals::width / (GLfloat)DGlobals::height, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        XSetStandardProperties(GLWin.dpy, GLWin.win,DGlobals::gameTitle.c_str(),DGlobals::gameTitle.c_str(), None, NULL, 0, NULL);

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
                        DGlobals::keys.setKeyState(DKeyboard::Key::State::RELEASED, _event.key.keysym.sym);
                        break;
                    case SDL_KEYDOWN:
                        DGlobals::keys.setKeyState(DKeyboard::Key::State::PRESSED, _event.key.keysym.sym);
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

#elif defined(GL_RENDER) && defined(__linux__)
            while(XPending(GLWin.dpy) > 0)
            {   
                XNextEvent(GLWin.dpy, &_event);
                switch(_event.type)
                {   
                    case Expose:
                        if (_event.xexpose.count != 0)
                            break;
                        break;
                    case ConfigureNotify:
                        if ((_event.xconfigure.width  != (signed) GLWin.width) ||
                                (_event.xconfigure.height != (signed) GLWin.height))
                        {   
                            GLWin.width  = _event.xconfigure.width;
                            GLWin.height = _event.xconfigure.height;

#ifdef DEBUG
                            fprintf(stdout, "Resize Event\n");
#endif                              
                        }   
                        break;
                    case KeyPress:
                        DGlobals::keys.setKeyState(DKeyboard::Key::State::PRESSED, XLookupKeysym(&_event.xkey,0));
                        break;
                    case ClientMessage:
                        if (*XGetAtomName(GLWin.dpy, _event.xclient.message_type) == *"WM_PROTOCOLS")
                        {   
#ifdef DEBUG
                            fprintf(stdout, "Quit pressed\n");
#endif
                            DGlobals::quit();
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

#if defined(GL_RENDER)
            if(GLWin.doublebuffer)
                glXSwapBuffers(GLWin.dpy, GLWin.win);
            else
                glFlush();
            glClearColor(0,0,0,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glLoadIdentity();
#elif defined(SDL_RENDER)
            SDL_BlitSurface(DGlobals::_buffer, 0, _screen, 0);
            SDL_UpdateRect(_screen, 0,0,0,0);
            SDL_FillRect(DGlobals::_buffer,0, DState::bgColor);
#endif

            unsigned int now;
            _frametime = 0;

            do {
#if defined(GL_RENDER) && defined(__linux__)
                timeb tb;
                ftime( &tb );
                now = tb.millitm + (tb.time & 0xfffff) * 1000;
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
