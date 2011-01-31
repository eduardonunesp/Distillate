#ifndef __DMOUSE_HPP__
#define __DMOUSE_HPP__

#include <string>

namespace Distillate
{
    /* Forward */
    class DSprite;

    /** 
     * This class helps contain and track the mouse pointer in your game.
     * Automatically accounts for parallax scrolling, etc.
     */
    class DMouse
    {
    public:
        /** 
         * Current X position of the mouse pointer in the game world.
         */
        int x;

        /** 
         * Current Y position of the mouse pointer in the game world.
         */
        int y;

        /** 
         * Current "delta" value of mouse wheel.  If the wheel was just scrolled up, it will have a positive value.  If it was just scrolled down, it will have a negative value.  If it wasn't just scroll this frame, it will be 0.
         */
        int wheel;

        /** 
         * Current X position of the mouse pointer on the screen.
         */
        int screenX;

        /** 
         * Current Y position of the mouse pointer on the screen.
         */
        int screenY;

        /** 
         * Graphical representation of the mouse pointer.
         */
        DSprite* cursor;

    protected:
        /**
         * Helper variable for tracking whether the mouse was just pressed or just released.
         */
        int _current;

        /**
         * Helper variable for tracking whether the mouse was just pressed or just released.
         */
        int _last;

        /**
         * Helper for mouse visibility.
         */
        bool _out;

    public:
        /**
         * Constructor.
         */
        DMouse();
        ~DMouse(){};

        /**
         * Either show an existing cursor or load a new one.
         * 
         * @param   Graphic     The image you want to use for the cursor.
         * @param   XOffset     The number of pixels between the mouse's screen position and the graphic's top left corner.
         * * @param YOffset     The number of pixels between the mouse's screen position and the graphic's top left corner. 
         */
        void show(void* Graphic=NULL, int XOffset=0, int YOffset=0);
        
        /**
         * Hides the mouse cursor
         */
        void hide();

        /** 
         * Load a new mouse cursor graphic
         * 
         * @param   Graphic     The image you want to use for the cursor.
         * @param   XOffset     The number of pixels between the mouse's screen position and the graphic's top left corner.
         * * @param YOffset     The number of pixels between the mouse's screen position and the graphic's top left corner. 
         */
        void load(void *Graphic, int XOffset=0, int YOffset=0);

        /**
         * Unload the current cursor graphic.  If the current cursor is visible,
         * then the default system cursor is loaded up to replace the old one.
         */
        void unload();
    
        /**
         * Called by the internal game loop to update the mouse pointer's position in the game world.
         * Also updates the just pressed/just released flags.
         * 
         * @param   X           The current X position of the mouse in the window.
         * @param   Y           The current Y position of the mouse in the window.
         * @param   XScroll     The amount the game world has scrolled horizontally.
         * @param   YScroll     The amount the game world has scrolled vertically.
         */
        void update(int X, int Y, float XScroll, float YScroll);

        /**
         * Resets the just pressed/just released flags and sets mouse to not pressed.
         */
        void reset()
        {
            _current = 0;
            _last = 0;
        }

        /**
         * Check to see if the mouse is pressed.
         * 
         * @return  Whether the mouse is pressed.
         */
        bool pressed() { return _current > 0; }

        /**
         * Check to see if the mouse was just pressed.
         * 
         * @return Whether the mouse was just pressed.
         */
        bool justPressed() { return _current == 2; }

        /**
         * Check to see if the mouse was just released.
         * 
         * @return  Whether the mouse was just released.
         */
        bool justReleased() { return _current == -1; }

        /**
         * Event handler so DGame can update the mouse.
         * 
         * @param   event   A <code>MouseEvent</code> object.
         */
        /*
        void handleMouseDown(event:MouseEvent)
        {
            if(_current > 0) _current = 1;
            else _current = 2;
        }
        */

        /**
         * Event handler so DGame can update the mouse.
         * 
         * @param   event   A <code>MouseEvent</code> object.
         */
        /*
        void handleMouseUp(event:MouseEvent)
        {
            if(_current > 0) _current = -1;
            else _current = 0;
        }
        */

        /**
         * Event handler so DGame can update the mouse.
         * 
         * @param   event   A <code>MouseEvent</code> object.
         */
        /*
        void handleMouseOut(event:MouseEvent)
        {
            if(cursor != null)
            {
                _out = cursor.visible;
                cursor.visible = false;
            }
        }
        */

        /**
         * Event handler so DGame can update the mouse.
         * 
         * @param   event   A <code>MouseEvent</code> object.
         */
        /*
        void handleMouseOver(event:MouseEvent)
        {
            if(cursor != null)
                cursor.visible = _out;
        }
        */

        /**
         * Event handler so DGame can update the mouse.
         * 
         * @param   event   A <code>MouseEvent</code> object.
         */
        /*
        public function handleMouseWheel(event:MouseEvent):void
        {
            wheel = event.delta;
        }
        */
    };
}
#endif
