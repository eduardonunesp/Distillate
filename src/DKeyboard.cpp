#include "DKeyboard.hpp"
#include <SDL/SDL.h>
#include <stdexcept>

namespace Distillate
{
    DKeyboard::DKeyboard(){}
    DKeyboard::~DKeyboard(){}

    void DKeyboard::setKeyState(int state, int k)
    {
        if(k < SDLK_FIRST || k > SDLK_LAST)
            throw std::runtime_error("Key out of range");
        _keys[k] = state;
    }

    int DKeyboard::checkKeyState(int state, int k)
    {
        if(k < SDLK_FIRST || k > SDLK_LAST)
            throw std::runtime_error("Key out of range");
        bool ret = (_keys[k] == state);

        if(ret) _keys[k] = 0;
        return ret;
    }
}
