#include "include/DGame.hpp"
#include "include/DPoint.hpp"
#include "include/DGroup.hpp"
#include "include/DState.hpp"
#include "include/DGlobals.hpp"
#include "include/DConsole.hpp"
#include <stdexcept>
#include <functional>
#include <algorithm>

namespace Distillate
{
    DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom):
    useDefaultHotKeys(true),
    pause(new DGroup()),
    _iState(InitialState),
    _created(false),
    _state(NULL),
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
        addEventListener(Backend::Event::ENTER_APP, DGame::create);
    }

    DGame::~DGame()
    {
        //dtor
        delete pause;
        delete _iState;
        delete _soundTray;
        delete _console;
    }

    void DGame::switchState(DState* State)
    {
    }

    void onKeyUp()
    {

    }

    void onFocus(/*event:Event=null*/)
    {

    }

    void onFocusLost(/*event:Event=null*/)
    {

    }

    void DGame::create(const Backend::Event &e)
    {
        if(!init(DGlobals::width, DGlobals::height)) 
            throw std::runtime_error("Cannot initialize SDL");
        addEventListener(Backend::Event::RUNNING_APP, DGame::update);
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

    void DGame::update(const Backend::Event &e)
    {
        while(DGlobals::_running)
        {
            // Polling events
//            SDL::Event::pollEvent();

            // Frame timing
//            unsigned int mark = SDL::getTimer();
  //          unsigned int ems = mark-_total;
  //          _elapsed = ems/1000;
  //          _total = mark;
  //          DGlobals::elapsed = _elapsed;
  //          if(DGlobals::elapsed > DGlobals::maxElapsed)
   //             DGlobals::elapsed = DGlobals::maxElapsed;
   //         DGlobals::elapsed *= DGlobals::timeScale;
        }
    }
}
