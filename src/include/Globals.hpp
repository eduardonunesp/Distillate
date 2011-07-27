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

#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#if defined(SDL_VIDEO)
#include <SDL/SDL.h>
#endif

#include <string>
#include <vector>
#include <map>

#include "Defs.hpp"

NAMESPACE_BEGIN

/* Forward */
class Object;
class Point;
class Game;
class Mouse;
class Keyboard;
class State;
class ResourceManager;
class TextureResource;

/**
 * This is a global helper class full of useful functions for audio,
 * input, basic info, and the camera system among other things.
 */
class Globals {
    /* Internal */
    friend class Game;
    friend class Sprite;

protected:
#if defined(SDL_VIDEO)
    /**
     * Keep the global buffer, useful for SDL global draw
     */
    static SDL_Surface *_buffer;
#endif

    /**
     * Internal tracker for game object (so we can pause & unpause)
     */
    static Game *_game;

    /**
     * Internal tracker for game pause state.
     */
    static bool _pause;

public:
    /**
     * Represents the amount of time in seconds that passed since last frame.
     */
    static float elapsed;

    /**
     * Essentially locks the framerate to a minimum value - any slower and you'll get slowdown instead of frameskip; default is 1/30th of a second.
     */
    static float maxElapsed;

    /**
     * How fast or slow time should pass in the game; default is 1.0.
     */
    static float timeScale;

    /**
     * Game name
     */
    static std::string gameTitle;

    /**
     * The width of the screen in game pixels.
     */
    static unsigned int width;

    /**
     * The height of the screen in game pixels.
     */
    static unsigned int height;

    /**
     * Bytes per pixel
     */
    static unsigned int bpp;
    
    /**
     * Frame per second
     */
    static unsigned int FPS;

    /**
     * <code>DG.levels</code> and <code>DG.scores</code> are generic
     * global variables that can be used for various cross-state stuff.
     */
    static std::vector<int> levels;
    static int level;
    static std::vector<int> scores;
    static int score;

    /**
     * A reference to a <code>Mouse</code> object.  Important for input!
     */
    static Mouse mouse;

    /**
     * A reference to a <code>Keyboard</code> object.  Important for input!
     */
    static Keyboard keys;

    /**
     * Control and canche all resources
     */
    static ResourceManager resourceManager;

protected:
    /**
     * Internal flag for whether or not the game is muted.
     */
    static bool _mute;

    /**
     * Internal volume level, used for global sound control.
     */
    static bool _volume;

public:
    /**
     * Stores the basic parallax scrolling values.
     */
    static Point scroll;

    /**
     * Set <code>pause</code> to true to pause the game, all sounds, and display the pause popup.
     */
    static bool pause();
    static void pause(bool Pause);

    /**
     * Setting running to false
     */
    static void quit() {
        _running = false;
    }

    /**
     * Sets a new state
     */
    static void setState(const std::string &Name);

private:

    /**
     * Indicates if current game is running, used in main loop
     */
    static bool _running;

    /**
     * Called by <code>Game</code> to set up <code>Globals</code> during <code>Game</code>'s constructor.
     */
    static void setGameData(Game* Game, unsigned int Width, unsigned int Height, unsigned int BPP);

    Globals() {};
    virtual ~Globals() {};
};

NAMESPACE_END
#endif /* __GLOBALS_HPP__ */
