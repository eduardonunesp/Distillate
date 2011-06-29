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
            App() { dispatchEvent(Event(Event::ENTER_APP)); }
            static bool init(unsigned int w, unsigned int h);
            static SDL_Surface* getScreen() { return _screen; }
        private:
            static SDL_Surface* _screen;
            static SDL_Event    _event;
        };
    }
}

#endif // __APP__
