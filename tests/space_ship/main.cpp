#include <iostream>
#include "Distillate.hpp"

using namespace Distillate;

class StateTest : public State {
private:
    Sprite player;
    Emitter emitter;
    Text label;

public:
    StateTest() : 
        State("State1"),
        player(10,10),
        emitter(Globals::height, Globals::width/2),
        label(110, 110, Globals::width,"A GAME"){};
    ~StateTest() {}

    void create() {
        bgColor = 0xff999999;

        Sprite::AnimationFrames frames;
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

        Sprite *particle = NULL;
        for(unsigned int i = 0;i < 10;i++) {
            particle = new Sprite();
            particle->visible = false;
            particle->createGraphic("particle",4, 4, 0x990000ff);
            emitter.add(particle);
        }   

        add(&emitter);
        emitter.start(false);

        label.setFormat("nokiafc22.ttf", 12);
        add(&label);
    }

    void update() {
        State::update();        

        player.velocity.x = 0;
        player.velocity.y = 0;

        if(Globals::keys(Keyboard::Key::ESCAPE))
            Globals::quit();

        if(Globals::keys(Keyboard::Key::UP))
            player.velocity.y -= 100;
        if(Globals::keys(Keyboard::Key::DOWN))
            player.velocity.y += 100;
        if(Globals::keys(Keyboard::Key::RIGHT))
            player.velocity.x += 100;
        if(Globals::keys(Keyboard::Key::LEFT))
            player.velocity.x -= 100;
    }
};

class GameTest : public Game {
public:
     GameTest() : Game("Space Ship") 
     {
     };
};

int main(int argc, char* argv[])
{

     GameTest test;
     test.setup(640, 480, 32);

     StateTest state;
     test.add(&state, true);

     return test.run();
}
