#include "App.hpp"

namespace Distillate
{
    const std::string Distillate::Backend::App::ENTER_APP = "enterApp";
    const std::string Distillate::Backend::App::RUNNING_APP = "runningApp";

    SDL_Surface *Backend::App::_screen = 0;
    SDL_Event    Backend::App::_event;

    bool Backend::App::init(unsigned int w, unsigned int h)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING))  return false;
        _screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE);

        if(!_screen) return false;
        /*Event e(RUNNING_APP);*/

        return true;
    }

    void Backend::App::Flip() 
    {
        SDL_UpdateRect(_screen, 0,0,0,0);
    }
}

