#ifndef __SDL_HPP__
#define __SDL_HPP__

#include "SDL/SDL.h"

namespace Distillate
{
    namespace SDL
    {
        bool initSDL();
        SDL_Surface* setVideoMode(unsigned int w, unsigned int h);
    }
}

#endif
