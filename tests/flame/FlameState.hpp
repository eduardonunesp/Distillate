#ifndef MENU_STATE
#define MENU_STATE

#include "Distillate.hpp"

using namespace Distillate;

class FlameState : public DState
{
protected:
    DEmitter* _flames_r;
    DEmitter* _flames_y;
    static const unsigned int _max_particles = 2000;

public:
    FlameState() {};
    ~FlameState() 
    {
        delete _flames_r;
        delete _flames_y;
    };

    void create();
    void update();
};

#endif
