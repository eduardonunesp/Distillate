#ifndef __DINPUT_HPP__
#define __DINPUT_HPP__

#include <vector>
#include <string>

namespace Distillate
{
    class DInput
    {
    private:
        /**
         * @private
         */
        const unsigned int _t;

        /**
         * Constructor
         */
    public:
        DInput();
        ~DInput(){}

        /**
         * Updates the key states (for tracking just pressed, just released, etc).
         */
        void update();

        /**
         * Resets all the keys.
         */
        void reset();

        /**
         * Check to see if this key is pressed.
         *
         * @param   Key     One of the key constants listed above (e.g. "LEFT" or "A").
         *
         * @return  Whether the key is pressed
         */
        bool pressed(const std::string Key) { /* return this[Key]; WTF ??? */ return false; }

        /**
         * Check to see if this key was just pressed.
         *
         * @param   Key     One of the key constants listed above (e.g. "LEFT" or "A").
         *
         * @return  Whether the key was just pressed
         */
        bool justPressed(const std::string& Key) { /*return _map[_lookup[Key]].current == 2; */ return false; }

        /**
         * Check to see if this key is just released.
         *
         * @param   Key     One of the key constants listed above (e.g. "LEFT" or "A").
         *
         * @return  Whether the key is just released.
         */
        bool justReleased(const std::string& Key) { /* return _map[_lookup[Key]].current == -1; */ return false; }
    };
}
#endif
