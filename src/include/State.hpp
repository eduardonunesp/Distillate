/**
 * Copyright (c) 2010 - 2011 Distillate Project
 *
 *  ______ ________________________            _____________________
 *  |     \  |  |______   |     |  |     |     |_____|   |   |______
 *  |_____/__|________|   |   __|__|_____|_____|     |   |   |______
 *
 *
 * License: BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Wintermoon nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>
#include "Defs.hpp"
#include "Group.hpp"

NAMESPACE_BEGIN

/* Forwards */
class Object;
class Sprite;

/**
 * This is the basic game "state" object - e.g. in a simple game
 * you might have a menu state and a play state.
 * It acts as a kind of container for all your game objects.
 * You can also access the game's background color
 * and screen buffer through this object.
 * State is kind of a funny class from the technical side,
 * it is just a regular Sprite display object,
 * with one member variable: a Distillate <code>Group</code>.
 * This means you can load it up with regular stuff
 * or with Distillate elements, whatever works!
 */
class State {
public:
    std::string name;

    /**
     * This static variable indicates the "clear color"
     * or default background color of the game.
     * Change it at ANY time using <code>State.bgColor</code>.
     */
    static unsigned int bgColor;

    /**
     * Internal group used to organize and display objects you add to this state.
     */
    Group defaultGroup;

    /**
     * Creates a new <code>State</code> object,
     * instantiating <code>screen</code> if necessary.
     */
    State(const std::string &Name);
    virtual ~State();

    /**
     * Override this function to set up your game state.
     * This is where you create your groups and game objects and all that good stuff.
     */
    virtual void  create() {
        //nothing to create initially
    }

    /**
     * Adds a new DCore subclass (Sprite, DBlock, etc) to the game loop.
     * State is adding this object to its built-in Group to automate updating and rendering.
     *
     * @param   Core    The object you want to add to the game loop.
     */
    Object *add(Object* Core);

    /**
     * Override this function to do special pre-processing FX like motion blur.
     * You can use scaling or blending modes or whatever you want against
     * <code>State.screen</code> to achieve all sorts of cool FX.
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
     * <code>State.screen</code> to achieve all sorts of cool FX.
     */
    virtual void postProcess() {
        //no fx by default
    }
};

NAMESPACE_END
#endif /* __STATE_HPP__ */
