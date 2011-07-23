#include "include/Globals.hpp"
#include "include/Game.hpp"
#include "include/Point.hpp"
#include "include/Keyboard.hpp"
#include "include/Mouse.hpp"
#include "include/Utils.hpp"
#include "include/State.hpp"
#include "include/ResourceManager.hpp"
#include "include/TextureResource.hpp"

#if defined(SDL_ENGINE)
#include <SDL/SDL_image.h>
#endif

NAMESPACE_BEGIN

const std::string Globals::LIBRARY_NAME = "Distillate";
std::string Globals::gameTitle = "Distillate Game";
float Globals::elapsed;
float Globals::maxElapsed = 0.333333f;
float Globals::timeScale = 1.0f;
Point Globals::scroll;
unsigned int Globals::width  = 0;
unsigned int Globals::height = 0;
unsigned int Globals::bpp    = 32;
unsigned int Globals::FPS = 0;
Game* Globals::_game = NULL;
ResourceManager Globals::resourceManager;
bool Globals::_pause = false;
bool Globals::_running = false;
Keyboard Globals::keys;
Mouse Globals::mouse;

#if defined(SDL_ENGINE)
SDL_Surface *Globals::_buffer = NULL;
#endif

bool Globals::pause()
{
    return _pause;
}

void Globals::setGameData(Game* Game, unsigned int Width, unsigned int Height, unsigned int BPP)
{
    _running = true;
    _game = Game;
    width = Width;
    height = Height;
    bpp = BPP;

#if defined(SDL_ENGINE)
    _buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,Globals::bpp,0,0,0,0);
    if(!_buffer) {
        fprintf(stderr, "%s", "Cannot create buffer \n");
        Globals::quit();
    }
#endif

    Utils::setWorldBounds(0,0,Width, Height);
}

void Globals::setState(const std::string &Name)
{
    _game->switchState(Name);
}

NAMESPACE_END    
