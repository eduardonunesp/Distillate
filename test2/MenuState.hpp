#ifndef MENU_STATE
#define MENU_STATE

#include "Distillate.hpp"

using namespace Distillate;

class MenuState : public DState
{
public:
    MenuState() {};
    ~MenuState() {};

    void create();
    void update();
};

#endif
