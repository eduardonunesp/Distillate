#ifndef __DCONSOLE_HPP__
#define __DCONSOLE_HPP__

#include <string>
#include <iostream>
#include "Distillate/SDL/Object.hpp"

namespace Distillate
{
    class DConsole
    {
    public:
        DConsole() {};
        virtual ~DConsole() {};
        void log(const std::string& Text) 
        {
            std::cout << Text << "\n";    
        }
    };
}

#endif
