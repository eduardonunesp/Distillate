#include "DGlobals.hpp"
#include "DGame.hpp"
#include "DPoint.hpp"
#include "DConsole.hpp"
#include "DKeyboard.hpp"
#include "DMouse.hpp"
#include "DUtils.hpp"
#include "DState.hpp"
#include <iostream>
#include <SDL/SDL_image.h>

namespace Distillate
{

const std::string DGlobals::LIBRARY_NAME = "Distillate";
float DGlobals::elapsed;
float DGlobals::maxElapsed = 0.333333f;
float DGlobals::timeScale = 1.0f;
DPoint *DGlobals::scroll = new DPoint();
unsigned int DGlobals::width  = 0;
unsigned int DGlobals::height = 0;
DGame* DGlobals::_game = NULL;
bool DGlobals::_pause = false;
bool DGlobals::_running = false;
DKeyboard *DGlobals::keys = new DKeyboard();
DMouse *DGlobals::mouse = new DMouse();
std::map<std::string, SDL_Surface*> DGlobals::_cache;
SDL_Surface *DGlobals::_buffer = NULL;

bool DGlobals::pause()
{
    return _pause;
}

void DGlobals::setGameData(DGame* Game, unsigned int Width, unsigned int Height, unsigned int Zoom)
{
    _running = true;
    _game = Game;
    width = Width;
    height = Height;
    _buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,32,0,0,0,0);
    if(!_buffer) 
    {
        std::cerr << "Cannot create buffer \n";
        DGlobals::quit();
    }

    DUtils::setWorldBounds(0,0,Width, Height);
}

SDL_Surface * DGlobals::addBitmap(const std::string &GraphicFile, bool Reverse, bool Unique, const std::string &Key)
{
    const std::string &key = Key;            
        
    if(key.empty())
    {   
        if(Unique && !_cache[key])
        {   
        }       
    }       
    
    SDL_Surface *pixels = NULL;

    if(!_cache[key])
    {   
        pixels = IMG_Load(GraphicFile.c_str());
        if(!pixels)
        {
            std::cerr << "IMG_Load:" << IMG_GetError() << std::endl;
            return NULL;
        }

        _cache.insert(std::make_pair<std::string, SDL_Surface*>(key, pixels));
    }       

    if(Reverse)
    {   
    } 

    return pixels;
}

void DGlobals::setState(DState *state)
{
    _game->switchState(state);
}

}
