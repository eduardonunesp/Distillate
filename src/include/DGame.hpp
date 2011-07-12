#ifndef __DGAME_HPP__
#define __DGAME_HPP__

#include <SDL/SDL.h>

namespace Distillate
{
    /* Forwards */
    class DPoint;
    class DState;
    class DGroup;
    class DConsole;
    class DSprite;

    /**
     * DGame is the heart of Distillate
     * all Distillate games, and contains a bunch of basic game loops and things.
     * It is a long and sloppy file that you shouldn't have to worry about too much!
     * It is basically only used to create your game object in the first place,
     * after that DGlobals and DState have all the useful stuff you actually need.
     */
    class DGame
    {
        /* Internal */
        friend class DGlobals;

    public:
        /**
         * Sets 0, -, and + to control the global volume and P to pause.
         * @default true
         */
        bool useDefaultHotKeys;

        /**
         * Displayed whenever the game is paused.
         * Override with your own <code>DLayer</code> for hot custom pause action!
         * Defaults to <code>data.DPause</code>.
         */
        DGroup *pause;

    protected:
        //startup
        bool _created;

    private:
        //basic display stuff
        DState* _state;
        SDL_Surface *_screen;
        SDL_Event _event;
        unsigned int _zoom;
        int _gameXOffset;
        int _gameYOffset;

        //basic update stuff
        float _elapsed;
        unsigned int _total;
        bool _paused;
        unsigned int _framerate;
        unsigned int _frameratePaused;

        //Pause screen, sound tray, support panel, dev console, and special effects objects
        DSprite *_soundTray;
        float _soundTrayTimer;
        //std::vector<???*> _soundTrayBars;
        DConsole *_console;

    public:
        /**
         * Game object constructor - sets up the basic properties of your game.
         *
         * @param	GameSizeX		The width of your game in pixels (e.g. 320).
         * @param	GameSizeY		The height of your game in pixels (e.g. 240).
         * @param	InitialState	The class name of the state you want to create and switch to first (e.g. MenuState).
         * @param	Zoom			The level of zoom (e.g. 2 means all pixels are now rendered twice as big).
         */
        DGame(unsigned int GameSizeX, unsigned int GameSizeY, DState *InitialState, unsigned int Zoom = 2);
        virtual ~DGame();

        /**
         * Makes the little volume tray slide out.
         *
         * @param	Silent	Whether or not it should beep.
         */
        void showSoundTray(bool Silent=false);

        /**
         * Switch from one <code>DState</code> to another.
         * Usually called from <code>DG</code>.
         *
         * @param	State		The class name of the state you want (e.g. PlayState)
         */
        void switchState(DState *State);

    protected:
        /**
         * Internal event handler for input and focus.
         */
        void onFocus(/*event:Event=null*/);

        /**
         * Internal event handler for input and focus.
         */
        void onFocusLost(/*event:Event=null*/);

    private:
        /**
         * Internal function to help with basic pause game functionality.
         */
        void unpauseGame();

        /**
         * Internal function to help with basic pause game functionality.
         */
        void pauseGame();

        /**
         * Used to instantiate the guts of flixel once we have a valid pointer to the root.
         */
        void create(/*event:Event*/);

    protected:
        /**
         * This is the main game loop.  It controls all the updating and rendering.
         */
        void update(/*event:Event*/);

    };
}
#endif // DGAME_HPP
