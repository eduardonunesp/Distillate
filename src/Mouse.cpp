#include "include/Mouse.hpp"

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#endif

NAMESPACE_BEGIN

DMouse::DMouse() {}
DMouse::~DMouse() {}

void DMouse::setMousePos(int x, int y)
{
    _x = x;
    _y = y;
}

void DMouse::showCursor(bool show)
{
#if defined(SDL_ENGINE)
    SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
#endif   
}

void DMouse::setButtonState(int state, int b)
{
    _buttons[b] = state;
}

bool DMouse::checkButtonState(int state, int b)
{
    return  (_buttons[b] == state);
}

NAMESPACE_END
