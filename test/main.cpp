#include <iostream>
#include <stdexcept>
#include "Distillate.hpp"

using namespace Distillate;

class State : public DState
{
public:
    State() : DState() {};
    void update() 
    {
        DState::update();
        if(DGlobals::keys->checkKeyState(SDL_KEYUP, SDLK_ESCAPE))
        {
            DGlobals::quit();
        }
    }

    void create() 
    {
        DGlobals::log("create");
        DSprite *player = new DSprite(10,10);
        player->loadGraphic("player.png", true, false, 52, 21, false);
        add(player);
    }
};

class Test : public DGame
{
public:    
    Test() : DGame(640, 480, new State()) {};
};


int main(int argc, char* argv[])
{
    try
    {
        Test* test = new Test();
        delete test;
    }
    catch(std::exception &e)
    {
        std::cout << "Ops: " << e.what() << std::endl;
    }

    return 0;
}
