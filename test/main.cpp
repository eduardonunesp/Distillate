#include "Distillate/DState.hpp"
#include "Distillate/DGame.hpp"

class StatePlay : public Distillate::DState
{
public:
    StatePlay() : Distillate::DState() {}
};

class Test : public Distillate::DGame 
{
public:
    Test() : Distillate::DGame(640,480,new StatePlay())
    {}
};

int main() 
{
    Test* test = new Test();
    return 0;
}
