#ifndef __GENERIC_HPP__
#define __GENERIC_HPP__

#include <string>

namespace Distillate
{
    namespace SDL
    {
        class Object
        {
        public:
            Object(){};
            ~Object(){}

            virtual std::string toString()
            {
                return "";            
            };
        };
    }
}

#endif
