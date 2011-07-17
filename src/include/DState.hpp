#ifndef DSTATE_HPP
#define DSTATE_HPP

#include <SDL/SDL.h>
#include <string>
#include "DGroup.hpp"

namespace Distillate
{

class DObject;
class DSprite;

/**
 * This is the basic game "state" object - e.g. in a simple game
 * you might have a menu state and a play state.
 * It acts as a kind of container for all your game objects.
 * You can also access the game's background color
 * and screen buffer through this object.
 * DState is kind of a funny class from the technical side,
 * it is just a regular Sprite display object,
 * with one member variable: a Distillate <code>DGroup</code>.
 * This means you can load it up with regular stuff
 * or with Distillate elements, whatever works!
 */
class DState
{
public:
    std::string name;

    /**
     * This static variable indicates the "clear color"
     * or default background color of the game.
     * Change it at ANY time using <code>DState.bgColor</code>.
     */
    static unsigned int bgColor;

    /**
     * Internal group used to organize and display objects you add to this state.
     */
    DGroup defaultGroup;

    /**
     * Creates a new <code>DState</code> object,
     * instantiating <code>screen</code> if necessary.
     */
    DState(const std::string &Name);
    virtual ~DState();

    /**
     * Override this function to set up your game state.
     * This is where you create your groups and game objects and all that good stuff.
     */
    virtual void  create()
    {
        //nothing to create initially
    }

    /**
     * Adds a new DCore subclass (DSprite, DBlock, etc) to the game loop.
     * DState is adding this object to its built-in DGroup to automate updating and rendering.
     *
     * @param   Core    The object you want to add to the game loop.
     */
    DObject *add(DObject* Core);

    /**
     * Override this function to do special pre-processing FX like motion blur.
     * You can use scaling or blending modes or whatever you want against
     * <code>DState.screen</code> to achieve all sorts of cool FX.
     */
    virtual void preProcess();

    /**
     * Automatically goes through and calls update on everything you added to the game loop,
     * override this function to handle custom input and perform collisions/
     */
    virtual void update();

    /**
     * This function collides <code>defaultGroup</code> against <code>defaultGroup</code>
     * (basically everything you added to this state).
     */
    virtual void collide();

    /*
     * Automatically goes through and calls render on everything you added to the game loop,
     * override this loop to manually control the rendering process.
     */
    virtual void render();

    /**
     * Override this function to do special pre-processing FX like light bloom.
     * You can use scaling or blending modes or whatever you want against
     * <code>DState.screen</code> to achieve all sorts of cool FX.
     */
    virtual void postProcess()
    {
        //no fx by default
    }

    /**
     * Override this function to handle any deleting or "shutdown" type operations you
     * might need (such as removing traditional Flash children like Sprite objects).
     */
    virtual void destroy();
};

}
#endif // DSTATE_HPP
