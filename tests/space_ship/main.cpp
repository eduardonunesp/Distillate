#include <iostream>
#include "Distillate.hpp"

using namespace Distillate;

class State : public DState {
private:
    DSprite player;
    DEmitter emitter;
    DText label;

public:
    State() : 
        DState("State1"),
        player(10,10),
        emitter(DGlobals::height, DGlobals::width/2),
        label(110, 110, DGlobals::width,"A GAME"){};
    ~State() {}

    void create() {
        bgColor = 0xff999999;

        DSprite::AnimationFrames frames;
        frames.push_back(0);
        frames.push_back(1);
        frames.push_back(2);

        player.loadGraphic("player.png", true, false, 52, 21, true);
        player.addAnimation("flying", frames, 100, true);
        player.play("flying");

        add(&player);

        emitter.delay = 0.5;
        emitter.gravity = 1000;;
        emitter.maxRotation = 0;
        emitter.setYSpeed(-300,- 500);
        emitter.setXSpeed(-20, 20);

        DSprite *particle = NULL;
        for(unsigned int i = 0;i < 10;i++) {
            particle = new DSprite();
            particle->createGraphic("particle",4, 4, 0x990000ff);
            emitter.add(particle);
        }   

        add(&emitter);
        emitter.start(false);

        label.setFormat("nokiafc22.ttf", 12);
        add(&label);
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
     Test() : DGame("Space Ship") 
     {
     };
};

int main(int argc, char* argv[])
{

     Test test;
     test.setup(640, 480, 32);

     State state;
     test.add(&state, true);

     return test.run();
}
