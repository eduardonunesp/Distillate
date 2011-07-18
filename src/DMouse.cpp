#include "include/DMouse.hpp"
#include "include/DSprite.hpp"
#include <SDL/SDL.h>

namespace Distillate {
    DMouse::DMouse() {}
    DMouse::~DMouse() {}

    void DMouse::setMousePos(int x, int y)
    {
         _x = x;
         _y = y;
    }

    void DMouse::showCursor(bool show)
    {
         SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
    }

    void DMouse::setButtonState(int state, int b)
    {
         _buttons[b] = state;
    }

    bool DMouse::checkButtonState(int state, int b)
    {
         return  (_buttons[b] == state);
    }
}
