#ifndef __DANIM_HPP__
#define __DANIM_HPP__

#include <string>
#include <vector>

namespace Distillate
{
    /**
     * Just a helper structure for the FlxSprite animation system
     */
    class DAnim
    {
    public:
        /**
         * String name of the animation (e.g. "walk")
         */
        std::string name;

        /**
         * Seconds between frames (basically the framerate)
         */
        int delay;

        /**
         * A list of frames stored as <code>uint</code> objects
         */
        std::vector<unsigned int> frames;

        /**
         * Whether or not the animation is looped
         */
        bool looped;

        /**
         * Constructor
         *
         * @param   Name        What this animation should be called (e.g. "run")
         * @param   Frames      An array of numbers indicating what frames to play in what order (e.g. 1, 2, 3)
         * @param   FrameRate   The speed in frames per second that the animation should play at (e.g. 40)
         * @param   Looped      Whether or not the animation is looped or just plays once
         */
        DAnim(const std::string &Name, std::vector<unsigned int> Frames, int FrameRate = 0, bool Looped = true)
        {
            name = Name;
            delay = 0;
            if(FrameRate > 0)
                delay = 1.0/FrameRate;
            frames = Frames;
            looped = Looped;
        };
    };
}
#endif
