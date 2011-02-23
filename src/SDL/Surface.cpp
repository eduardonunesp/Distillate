#include "Distillate/SDL/Surface.hpp"

namespace Distillate
{
    SDL_Surface* SDL::Surface::buffer()
    {
        return _buffer;
    }

    SDL_Surface* SDL::Surface::buffer(SDL_Surface* b)
    {
        _buffer = b;
        return b;
    }
}
