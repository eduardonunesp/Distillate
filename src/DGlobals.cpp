#include "include/DGlobals.hpp"
#include "include/DGame.hpp"
#include "include/DPoint.hpp"
#include "include/DKeyboard.hpp"
#include "include/DMouse.hpp"
#include "include/DUtils.hpp"
#include "include/DState.hpp"
#include <SDL/SDL_image.h>

namespace Distillate
{

const std::string DGlobals::LIBRARY_NAME = "Distillate";
std::string DGlobals::gameTitle = "Distillate Game";
float DGlobals::elapsed;
float DGlobals::maxElapsed = 0.333333f;
float DGlobals::timeScale = 1.0f;
DPoint DGlobals::scroll;
unsigned int DGlobals::width  = 0;
unsigned int DGlobals::height = 0;
unsigned int DGlobals::FPS = 0;
DGame* DGlobals::_game = NULL;
bool DGlobals::_pause = false;
bool DGlobals::_running = false;
DKeyboard DGlobals::keys;
DMouse DGlobals::mouse;

#ifdef GL_RENDER
std::map<std::string, void*> DGlobals::_cache;
#elif SDL_RENDER
std::map<std::string, SDL_Surface*> DGlobals::_cache;
SDL_Surface *DGlobals::_buffer = NULL;
#endif 

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

#ifdef SDL_RENDER
    _buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,32,0,0,0,0);
    if(!_buffer) 
    {
        fprintf(stderr, "%s", "Cannot create buffer \n");
        DGlobals::quit();
    }
#endif

    DUtils::setWorldBounds(0,0,Width, Height);
}

#ifdef GL_RENDER
void * DGlobals::addBitmap(const std::string &GraphicFile, bool Reverse, bool Unique, const std::string &Key)
#elif SDL_RENDER
SDL_Surface * DGlobals::addBitmap(const std::string &GraphicFile, bool Reverse, bool Unique, const std::string &Key)
#endif
{
    const std::string &key = Key;            
        
    if(key.empty())
    {   
        if(Unique && !_cache[key])
        {   
        }       
    }       
   
#ifdef GL_RENDER
    return NULL;
#elif SDL_RENDER
    SDL_Surface *pixels = NULL;

    if(!_cache[key])
    {   
        pixels = IMG_Load(GraphicFile.c_str());
        if(!pixels)
        {
            fprintf(stderr, "IMG_Load: %s", IMG_GetError());
            return NULL;
        }

        _cache.insert(std::make_pair<std::string, SDL_Surface*>(key, pixels));
    }       

    if(Reverse)
    {   
    } 

    return pixels;
#endif
}

void DGlobals::setState(const std::string &Name)
{
    _game->switchState(Name);
}

}
