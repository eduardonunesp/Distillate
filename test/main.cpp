#include <iostream>
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
    Test() : Distillate::DGame(640,480,new StatePlay()) {}
};

int main(int argc, char *argv[])
{
    try
    {
        Test* test = new Test();
    }
    catch(...)
    {
        std::cout << "Error while initialize" << std::endl;
    }

    return 0;
}
