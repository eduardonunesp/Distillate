#include "Distillate/SDL/SDL.hpp"

namespace Distillate
{
    bool SDL::initSDL()
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
            return false;
        return true;
    }

    SDL_Surface* SDL::setVideoMode(unsigned int w, unsigned int h)
    {
        return SDL_SetVideoMode(w, h, 16, SDL_HWSURFACE);
    }
}
