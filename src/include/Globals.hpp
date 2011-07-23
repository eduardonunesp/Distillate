#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#endif

#include <string>
#include <vector>
#include <map>

#include "defs.hpp"

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
class Global {
    /* Internal */
    friend class Game;
    friend class DSprite;

protected:
#if defined(SDL_ENGINE)
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
     * Called by <code>Game</code> to set up <code>Global</code> during <code>Game</code>'s constructor.
     */
    static void setGameData(Game* Game, unsigned int Width, unsigned int Height, unsigned int BPP);

    Global() {};
    virtual ~Global() {};
};

NAMESPACE_END
#endif /* __GLOBALS_HPP__ */
