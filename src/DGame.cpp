#include "Distillate/DGame.hpp"
#include "Distillate/DPoint.hpp"
#include "Distillate/DGroup.hpp"
#include "Distillate/DState.hpp"

namespace Distillate
{
    DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom)
    {
        _zoom = Zoom;
        DState::bgColor = 0xff000000;
        //DGlobals::setGameData(this,GameSizeX,GameSizeY,Zoom);
        _elapsed = 0;
        _total = 0;
        //pause = new DPauseState();
        _state = NULL;
        _iState = InitialState;
        _zeroPoint = new DPoint();

        useDefaultHotKeys = true;

        //_frame = NULL;
        _gameXOffset = 0;
        _gameYOffset = 0;

        _paused = false;
        _created = false;


        //    _buffer = SDL_SetVideoMode(GameSizeX,GameSizeY,16,SDL_HWSURFACE);
        create();
    }

    DGame::~DGame()
    {
        //dtor
        delete pause;
        delete _iState;
        delete _buffer;
        delete _zeroPoint;
        delete _soundTray;

    }

    void DGame::create()
    {
        unsigned int i;
        unsigned int l;
        //DSave* soundPrefs = NULL;
        //TODO: Set frame rate !!! 
        update();
    }

    void DGame::update()
    {
        //unsigned int mark = getTimer();
        //TODO: put time controller

        while(true)
        {
            //       SDL_Delay(10);
        }
    }

}
