#ifndef MENU_STATE
#define MENU_STATE

#include "Distillate.hpp"

using namespace Distillate;

class MenuState : public DState
{
protected:
    DEmitter* _flames;
    static const unsigned int _max_particles = 20;

public:
    MenuState() {};
    ~MenuState() {};

    void create();
    void update();
};

#endif
