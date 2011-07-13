#include "DKeyboard.hpp"
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
    if(ret) _keys[k] = 0;
    return ret;
}
    
}
