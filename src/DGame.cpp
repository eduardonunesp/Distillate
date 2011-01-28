#include "DGame.hpp"

namespace Distillate
{

DGame::DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState* InitialState, unsigned int Zoom)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        exit(1);
    }

    _zoom = Zoom;
    //DState::bgColor = 0xff000000;
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


    _buffer = SDL_SetVideoMode(GameSizeX,GameSizeY,16,SDL_HWSURFACE);
    create();
}

DGame::~DGame()
{
    //dtor

}

void DGame::create()
{
    //unsigned int i;
    //DSave* soundPrefs;

    update();
}

void DGame::update()
{
    //unsigned int mark = getTimer();
    //TODO: put time controller

    while(true)
    {
        SDL_Delay(10);
    }
}

}
