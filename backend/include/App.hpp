#ifndef __APP__
#define __APP__

#include <SDL/SDL.h>
#include "EventDispatcher.hpp"

namespace Distillate
{
    namespace Backend
    {
        class App : public EventDispatcher 
        {
        public:
            static const std::string ENTER_APP;
            static const std::string RUNNING_APP;

            App() { /* dispatchEvent(new Event(ENTER_APP)); */ }

            static bool init(unsigned int w, unsigned int h);
            static void Flip();
            static SDL_Surface* _screen;
            static SDL_Event    _event;
        };
    }
}

#endif // __APP__
