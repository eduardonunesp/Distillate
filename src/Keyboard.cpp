#include "include/Keyboard.hpp"

NAMESPACE_BEGIN

Keyboard::DKeyboard() {}
Keyboard::~DKeyboard() {}

void Keyboard::setKeyState(int state, int k)
{
    _keys[k] = state;
}

bool Keyboard::checkKeyState(int state, int k)
{
    return (_keys[k] == state);;
}

NAMESPACE_END
