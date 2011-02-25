#ifndef DGLOBALS_HPP
#define DGLOBALS_HPP

#include <string>
#include <vector>
#include <map>
#include "Distillate/SDL/Object.hpp"
#include "Distillate/SDL/Bitmap.hpp"
#include "Distillate/SDL/BitmapData.hpp"

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

        /**
         * A handy container for a background music object.
         */
        //static DSound *music;

        /**
         * A list of all the sounds being played in the game.
         */
        //static std<DSounds*> sounds;

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

    public:
        /**
         * Stores the basic parallax scrolling values.
         */
        static DPoint *scroll;

        /**
         * Reference to the active graphics buffer.
         * Can also be referenced via <code>FlxState.screen</code>.
         */
        static SDL::Bitmap* buffer;

    protected:
        /**
         * Internal storage system to prevent graphics from being used repeatedly in memory.
         */
        static std::map<std::string, SDL::Object*> _cache;

    public:
        /**
         * Access to the Kongregate high scores and achievements API.
         */
        //static public var kong:DKong;

        /**
         * The support panel (twitter, reddit, stumbleupon, paypal, etc) visor thing
         */
        //static DPanel *panel;

        /**
         * A special effect that shakes the screen.  Usage: DG.quake.start();
         */
        //static DQuake *quake;

        /**
         * A special effect that flashes a color on the screen.  Usage: DG.flash.start();
         */
        //static DFlash *flash;

        /**
         * A special effect that fades a color onto the screen.  Usage: DG.fade.start();
         */
        //static DFade *fade;

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
         * Loads a bitmap from a file, caches it, and generates a horizontally flipped version if necessary.
         *
         * @param   Graphic     The image file that you want to load.
         * @param   Reverse     Whether to generate a flipped version.
         *
         * @return  The <code>BitmapData</code> we just created.
         */
        static SDL::BitmapData* addBitmap(void* Graphic, bool Reverse=false, bool Unique=false, const std::string& Key="");

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
