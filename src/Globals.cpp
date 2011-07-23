#include "include/DGlobals.hpp"
#include "include/DGame.hpp"
#include "include/DPoint.hpp"
#include "include/DKeyboard.hpp"
#include "include/DMouse.hpp"
#include "include/DUtils.hpp"
#include "include/DState.hpp"
#include "include/DResourceManager.hpp"
#include "include/DTextureResource.hpp"

#if defined(SDL_RENDER)
#include <SDL/SDL_image.h>
#endif

namespace Distillate {
    const std::string DGlobals::LIBRARY_NAME = "Distillate";
    std::string DGlobals::gameTitle = "Distillate Game";
    float DGlobals::elapsed;
    float DGlobals::maxElapsed = 0.333333f;
    float DGlobals::timeScale = 1.0f;
    DPoint DGlobals::scroll;
    unsigned int DGlobals::width  = 0;
    unsigned int DGlobals::height = 0;
    unsigned int DGlobals::bpp    = 32;
    unsigned int DGlobals::FPS = 0;
    DGame* DGlobals::_game = NULL;
    DResourceManager DGlobals::resourceManager;
    bool DGlobals::_pause = false;
    bool DGlobals::_running = false;
    DKeyboard DGlobals::keys;
    DMouse DGlobals::mouse;

#if defined(SDL_RENDER)
    SDL_Surface *DGlobals::_buffer = NULL;
#endif

    bool DGlobals::pause()
    {
         return _pause;
    }

    void DGlobals::setGameData(DGame* Game, unsigned int Width, unsigned int Height, unsigned int BPP)
    {
         _running = true;
         _game = Game;
         width = Width;
         height = Height;
         bpp = BPP;

#ifdef SDL_RENDER
         _buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,DGlobals::bpp,0,0,0,0);
         if(!_buffer) {
              fprintf(stderr, "%s", "Cannot create buffer \n");
              DGlobals::quit();
         }
#endif

         DUtils::setWorldBounds(0,0,Width, Height);
    }

    void DGlobals::setState(const std::string &Name)
    {
         _game->switchState(Name);
    }
}
