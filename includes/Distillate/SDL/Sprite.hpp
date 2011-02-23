#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__ 

#include "Surface.hpp"

namespace Distillate 
{
    namespace SDL
    {
        class Sprite : public Surface
        {
        public:
            Sprite();
            ~Sprite();
        };
    }
}

#endif
