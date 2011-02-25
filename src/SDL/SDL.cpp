#include "Distillate/SDL/SDL.hpp"
#include <cstring>

namespace Distillate
{
    bool SDL::initSDL()
    {
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
            return false;

        int flags = MIX_INIT_OGG;
        int initted = Mix_Init(flags);

        if((initted & flags) != flags)
        {
            return false;
        }

        return true;
    }

    SDL_Surface* SDL::setVideoMode(unsigned int w, unsigned int h)
    {
        return SDL_SetVideoMode(w, h, 16, SDL_HWSURFACE);
    }

    unsigned int SDL::getTimer()
    {
        return SDL_GetTicks();
    }

    SDL_Event SDL::Event::event;
    std::multimap<SDL::Event::Type, SDL::Event::callbackFunctionEvent*> SDL::Event::_events;
    void SDL::Event::addEvent(Type t, callbackFunctionEvent func)
    {
        _events.insert(std::make_pair(t, func));     
    }

    void SDL::Event::pollEvent()
    {
        Type current = NONE;
        void* curr_data = NULL;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    current = CLOSE_WINDOW;
                    curr_data = (void*) "Command: SDL_QUIT\0";
                    break;
               case SDL_KEYUP:                    
               {
                    current = KEY_UP;
                } break;
                default:
                    current = NONE;
                    break;
            }
        }

        std::multimap<Type, callbackFunctionEvent*>::iterator itr = _events.begin();
        while(itr != _events.end())
        {
            if((*itr).first == current)
                (*itr).second(new Event(current, curr_data));
            itr++;
        }
    }
}
