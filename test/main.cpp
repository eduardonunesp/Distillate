#include <iostream>
#include <stdexcept>
#include "DState.hpp"
#include "DGame.hpp"

class State : public Distillate::DState
{
public:
    State() : Distillate::DState() {};
};

class Test : public Distillate::DGame
{
public:    
    Test() : Distillate::DGame(640, 480, new State()) {};
};


int main(int argc, char* argv[])
{
    try
    {
        Test* test = new Test();
    }
    catch(std::exception &e)
    {
        std::cout << "Ops: " << e.what() << std::endl;
    }

    return 0;
}
