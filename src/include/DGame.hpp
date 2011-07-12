#ifndef __DGAME_HPP__
#define __DGAME_HPP__

#include <SDL/SDL.h>
#include "App.hpp"

namespace Distillate
{
    /* Forwards */
    class DPoint;
    class DState;

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
    protected:
        //startup
        SDL_Surface* _screen;
        SDL_Event _event;

    private:
        //basic display stuff
        DState* _state;

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
         * Switch from one <code>DState</code> to another.
         * Usually called from <code>DG</code>.
         *
         * @param	State		The class name of the state you want (e.g. PlayState)
         */
        void switchState(DState *State);

        /**
         * Used to instantiate the guts of flixel once we have a valid pointer to the root.
         */
        void create();

    protected:
        /**
         * This is the main game loop.  It controls all the updating and rendering.
         */
        void update();

    };
}
#endif // DGAME_HPP
