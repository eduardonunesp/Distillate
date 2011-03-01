#ifndef __VIDEO_BACKEND__
#define __VIDEO_BACKEND__

/* 
 * With ifdefs we configure our backend for video,
 * this way we can use configure others libs, but 
 * the default is aways SDL
 */
#ifdef SDL_BACKEND
#include "SDL/SDL.h"    
#endif

namespace Distillate
{
    namespace Backend
    {
        class Video
        {
        public:
            class Surface
            {
#ifdef SDL_BACKEND
            public:
               Surface(SDL_Surface* buffer=NULL) : _buffer(buffer) {}
            private:
               SDL_Surface* _buffer;
#endif      
            };

            static bool init(unsigned int w, unsigned int h);
            static Surface* getScreen() { return _screen; }
        private:
            Video() {};
            virtual ~Video(){};
            static Surface* _screen;
        };
    }
}

#endif
