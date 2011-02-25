#ifndef __SDL_HPP__
#define __SDL_HPP__

extern "C" 
{
    #include "SDL/SDL.h"
    #include "SDL/SDL_mixer.h"
}

#include <map>

namespace Distillate
{
    namespace SDL
    {
        bool initSDL();
        SDL_Surface* setVideoMode(unsigned int w, unsigned int h);
        unsigned int getTimer();

        struct Event
        {
            typedef void (callbackFunctionEvent)(Event* e);
            typedef enum
            {
                NONE = 0,
                KEY_UP,
                KEY_DOWN,
                CLOSE_WINDOW
            } Type;

            Type type;
            void* data;

            Event(Type t, void* d) : type(t), data(d) {} 
            virtual ~Event() { delete data; }

            static void addEvent(Type t, callbackFunctionEvent* func);
            static void pollEvent();
        protected:
            static SDL_Event event;
            static std::multimap<Type, callbackFunctionEvent*> _events;
        };
    }
}

#endif
