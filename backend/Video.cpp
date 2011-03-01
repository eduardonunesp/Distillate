#include "Video.hpp"

namespace Distillate
{
    Backend::Video::Surface* Backend::Video::_screen = NULL;

    bool Backend::Video::init(unsigned int w, unsigned int h)
    {
#ifdef SDL_BACKEND
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
            return false;

        SDL_Surface* tmp_s = NULL;
        tmp_s = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE);

        if(!tmp_s)
            return false;

        _screen = new Surface(tmp_s);
#endif

        return true;
    }
}
