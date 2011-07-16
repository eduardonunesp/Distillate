#include "include/DKeyboard.hpp"
#include <SDL/SDL.h>
#include <stdexcept>

namespace Distillate
{

DKeyboard::DKeyboard(){}
DKeyboard::~DKeyboard(){}

void DKeyboard::setKeyState(int state, int k)
{
    _keys[k] = state;
}

bool DKeyboard::checkKeyState(int state, int k)
{
    bool ret = (_keys[k] == state);
    return ret;
}
    
}
