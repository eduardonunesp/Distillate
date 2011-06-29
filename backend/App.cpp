#include "App.hpp"

namespace Distillate
{
    SDL_Surface *Backend::App::_screen = 0;

    bool Backend::App::init(unsigned int w, unsigned int h)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING))  return false;
        _screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE);

        if(!_screen) return false;
        Event e(Event::RUNNING_APP);

        return true;
    }
}
