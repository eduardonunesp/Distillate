#ifndef __RECTANGLE5_HPP__
#define __RECTANGLE5_HPP__ 

#include "Object.hpp"
#include "Point.hpp"

namespace Distillate 
{
    namespace SDL
    {
        class Rectangles : public Object
        {
        public:
            float bottom;
            float left;
            float right;
            float top;
            Point* size;
            Point* bottomRight;
            Point* topLeft;
            float width;
            float height;
            float x;
            float y;

            Point() {};
            ~Point() 
            {
                delete size;
                delete bottomRight;
                delete topLeft;
            };
        };
    }
}
