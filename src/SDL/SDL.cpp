#include "Distillate/SDL/SDL.hpp"

namespace Distillate
{
    namespace SDL
    {
        bool initSDL()
        {
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
                return false;
            return true;
        }
    }
}
