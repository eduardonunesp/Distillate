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
    Test* test = NULL;
    try
    {
        test = new Test();
    }
    catch(std::exception &e)
    {
        std::cout << "Ops: " << e.what() << std::endl;
    }
    
    delete test;
    return 0;
}
