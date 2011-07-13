#ifndef __DGLOBALS_HPP__
#define __DGLOBALS_HPP__

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <map>

namespace Distillate
{
    /* Forward */
    class DObject;
    class DPoint;
    class DGame;
    class DMouse;
    class DKeyboard;

    /**
     * This is a global helper class full of useful functions for audio,
     * input, basic info, and the camera system among other things.
     */
    class DGlobals
    {
        /* Internal */
        friend class DGame;
        friend class DSprite;

    protected:
        static SDL_Surface *_buffer;
    
    public:
        /**
         * If you build and maintain your own version of flixel,
         * you can give it your own name here.  Appears in the console.
         */
        static const std::string LIBRARY_NAME;

        /**
         * Assign a major version to your library.
         * Appears before the decimal in the console.
         */
        static const unsigned int LIBRARY_MAJOR_VERSION = 1;

        /**
         * Assign a minor version to your library.
         * Appears after the decimal in the console.
         */
        static const unsigned int LIBRARY_MINOR_VERSION = 0;

    protected:
        /**
         * Internal tracker for game object (so we can pause & unpause)
         */
        static DGame *_game;

        /**
         * Internal tracker for game pause state.
         */
        static bool _pause;

    public:
        /**
         * Whether you are running in Debug or Release mode.
         * Set automatically by <code>DFactory</code> during startup.
         */
        static bool debug;

    protected:
        /**
         * Internal tracker for bounding box visibility.
         */
        static bool _showBounds;

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
         * The width of the screen in game pixels.
         */
        static unsigned int width;

        /**
         * The height of the screen in game pixels.
         */
        static unsigned int height;

        /**
         * <code>DG.levels</code> and <code>DG.scores</code> are generic
         * global variables that can be used for various cross-state stuff.
         */
        static std::vector<int> levels;
        static int level;
        static std::vector<int> scores;
        static int score;

        /**
         * <code>DG.saves</code> is a generic bucket for storing
         * DSaves so you can access them whenever you want.
         */
        static std::vector<int> saves;
        static int save;

        /**

         * A reference to a <code>DMouse</code> object.  Important for input!
         */
        static DMouse *mouse;

        /**
         * A reference to a <code>DKeyboard</code> object.  Important for input!
         */
        static DKeyboard *keys;


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
         * Tells the camera to follow this <code>DCore</code> object around.
         */
        static DObject *followTarget;

        /**
         * Used to force the camera to look ahead of the <code>followTarget</code>.
         */
        static DPoint *followLead;

        /**
         * Used to smoothly track the camera as it follows.
         */
        static float followLerp;

        /**
         * Stores the top and left edges of the camera area.
         */
        static DPoint *followMin;

        /**
         * Stores the bottom and right edges of the camera area.
         */
        static DPoint *followMax;

    protected:
        /**
         * Internal, used to assist camera and scrolling.
         */
        static DPoint *_scrollTarget;

        /**
         * Internal storage system to prevent graphics from being used repeatedly in memory.
         */
        static std::map<std::string, SDL_Surface*> _cache;

    public:
        /**
         * Stores the basic parallax scrolling values.
         */
        static DPoint *scroll;

        /**
         * Log data to the developer console.
         *
         * @param	Data		Anything you want to log to the console.
         */
        static void log(const std::string& log);

        /**
         * Set <code>pause</code> to true to pause the game, all sounds, and display the pause popup.
         */
        static bool pause();
        static void pause(bool Pause);

        /**
         * Setting running to false
         */
        static void quit()
        {
            _running = false;
        }

        static SDL_Surface * addBitmap(const std::string &GraphicFile, bool Reverse = false, bool Unique = false, const std::string &Key = "");
    private:

        /**
         * Indicates if current game is running, used in main loop
         */
        static bool _running;

        /**
         * Called by <code>DGame</code> to set up <code>DGlobals</code> during <code>FlxGame</code>'s constructor.
         */
        static void setGameData(DGame* Game, unsigned int Width, unsigned int Height, unsigned int Zoom);

        DGlobals() {};
        virtual ~DGlobals() {};
    };
}
#endif // DGLOBALS_HPP
