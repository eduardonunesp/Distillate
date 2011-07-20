#include <iostream>
#include "Distillate.hpp"

using namespace Distillate;

class State : public DState {
private:
    DSprite player;

public:
    State() : 
        DState("State1"),
        player(10,10) {};
    ~State() {}

    void create() {
        DSprite::AnimationFrames frames;
        frames.push_back(0);
        frames.push_back(1);
        frames.push_back(2);

        player.loadGraphic("player.png", true, false, 52, 21, true);
        player.addAnimation("flying", frames, 100, true);
        player.play("flying");

        add(&player);
    }

    void update() {
        DState::update();        

        player.velocity.x = 0;
        player.velocity.y = 0;

        if(DGlobals::keys(DKeyboard::Key::ESCAPE))
            DGlobals::quit();

        if(DGlobals::keys(DKeyboard::Key::UP))
            player.velocity.y -= 100;
        if(DGlobals::keys(DKeyboard::Key::DOWN))
            player.velocity.y += 100;
        if(DGlobals::keys(DKeyboard::Key::RIGHT))
            player.velocity.x += 100;
        if(DGlobals::keys(DKeyboard::Key::LEFT))
            player.velocity.x -= 100;
    }
};

class Test : public DGame {
public:
     Test() : DGame("Space Ship") {};
};

int main(int argc, char* argv[])
{
     Test test;
     test.setup(640, 480, 16);

     State state;
     test.add(&state, true);

     return test.run();
}
