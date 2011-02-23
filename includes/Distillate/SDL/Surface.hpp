#ifndef __SURFACE_HPP__
#define __SURFACE_HPP__ 

#ifdef WIN32 
#include "SDL.h" 
#else 
#include "SDL/SDL.h" 
#endif 

namespace Distillate 
{
    namespace SDL
    {
        class Surface 
        {
        public:
            Surface() : _buffer(NULL) {};
            Surface(SDL_Surface* b) : _buffer(b) {};
            ~Surface() { delete _buffer; }

            SDL_Surface* buffer();
            SDL_Surface* buffer(SDL_Surface* b);

        private:
            SDL_Surface* _buffer;
        };
    }
}

#endif
