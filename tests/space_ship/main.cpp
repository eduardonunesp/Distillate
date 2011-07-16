#include <iostream>
#include "../../src/include/Distillate.hpp"

using namespace Distillate;

class StateWin : public DState
{
public:
    StateWin() : DState() {};
    ~StateWin() {}

    void update() 
    {
        DState::update();
        printf("OK\n");
        sleep(2);
        DGlobals::quit();
    }
};

class State : public DState
{
public:
    DSprite *player;
    DSprite *enemy;

    State() : DState() {};
    ~State() { destroy(); }

    void update() 
    {
        DState::update();

        player->velocity.x = 0;
        player->velocity.y = 0;

        if(DGlobals::keys.checkKeyState(SDL_KEYUP, SDLK_ESCAPE))
            DGlobals::quit();

        if(DGlobals::keys.checkKeyState(SDL_KEYDOWN, SDLK_RIGHT))
            player->velocity.x += 100;
        if(DGlobals::keys.checkKeyState(SDL_KEYDOWN, SDLK_LEFT))
            player->velocity.x -= 100;
        if(DGlobals::keys.checkKeyState(SDL_KEYDOWN, SDLK_UP))
            player->velocity.y -= 100;
        if(DGlobals::keys.checkKeyState(SDL_KEYDOWN, SDLK_DOWN))
            player->velocity.y += 100;

        DUtils::overlap(enemy, player, State::Collide); 

        if(enemy->health <= 0)
            DGlobals::setState(new StateWin());
    }

    static bool Collide(DObject* e, DObject* p)
    {
        printf("Enemy energy: %f\n", e->health);
        e->health--;
        return true;
    }

    void create() 
    {
        player = new DSprite(10,10);
        player->loadGraphic("player.png", true, false, 52, 21, false);

        std::vector<int> anim_frame;
        anim_frame.push_back(0);
        anim_frame.push_back(1);
        anim_frame.push_back(2);

        player->addAnimation("flying", anim_frame, 3, true);
        player->play("flying");

        add(player);

        enemy = new DSprite(50,50);
        enemy->createGraphic(20, 20);
        enemy->health = 10;
        add(enemy);
    }
};

class Test : public DGame
{
public:    
    Test() : DGame("SPACE SHIP", 640, 480, new State()) {};
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
