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

    void DGame::switchState(DState* State)
    {
    }
    
    void onKeyUp(SDL::Event* e)
    {
        if(e->data)
            DGlobals::log((char*) e->data);
    }

    void onFocus(/*event:Event=null*/)
    {

    }

    void onFocusLost(/*event:Event=null*/)
    {
        
    }

    void DGame::create()
    {
        if(!SDL::initSDL()) throw std::runtime_error("Cannot initialize SDL");
        _screen->buffer(SDL::setVideoMode(DGlobals::width, DGlobals::height));
        SDL::Event::addEvent(SDL::Event::KEY_UP, &onKeyUp);
        update();
    }

    void DGame::showSoundTray(bool Silent)
    {
        /*
        if(!Silent)
            DGlobals::play(SndBeep);        
        _soundTrayTimer = 1;
        _sonndTray.y = _gameYOffset;
        _sonndTray.visible = true;

        unsigned int gv = DUtils::roundValue(DGlobals::volume*10);
        if(DGlobals::mute)
            gv = 0;
        for(unsigned int i = 0; i < _sonndTrayBars.length; i++)
        {
            if(i < gv) _sonndTrayBars[i].alpha = 1;
            else _sonndTrayBars[i].alpha = 0.5;
        }
        */
    }

    void DGame::update()
    {
        while(DGlobals::_running)
        {
            // Polling events
            SDL::Event::pollEvent();

            // Frame timing
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
