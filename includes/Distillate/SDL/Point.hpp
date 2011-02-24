#ifndef __POINT_HPP__
#define __POINT_HPP__ 

#include "Object.hpp"

namespace Distillate 
{
    namespace SDL
    {
        class Point : public Object
        {
        public:
            int x;
            int y;

            Point() {};
            ~Point() {};
        };
    }
}
#endif
