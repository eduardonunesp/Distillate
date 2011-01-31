#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__ 

#include "Object.hpp"

namespace Distillate 
{
    namespace SDL
    {
        class Matrix : public Object
        {
        public:
            float a;
            float b;
            float c;
            float d;
            float tx;
            float ty;
            
            Matrix();
            ~Matrix();
        };
    }
}
#endif
