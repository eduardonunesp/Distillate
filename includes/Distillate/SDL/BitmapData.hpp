#ifndef __BITMAPDATA_HPP__
#define __BITMAPDATA_HPP__ 

#include "SDL.hpp"

namespace Distillate 
{
    namespace SDL
    {
        class BitmapData 
        {
        public:
            int width;
            int height;

            BitmapData();
            ~BitmapData();
        };
    }
}
#endif
