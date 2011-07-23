#ifndef __ANIM_HPP__
#define __ANIM_HPP__

#include <cstdio>
#include <string>
#include <vector>
#include "Defs.h"

NAMESPACE_BEGIN

/**
 * Just a helper structure for the Sprite animation system
 */
class Anim {
public:
    /**
     * String name of the animation (e.g. "walk")
     */
    std::string name;

    /**
     * Seconds between frames (basically the framerate)
     */
    float delay;

    /**
     * A list of frames stored as unsigned int objects
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
    Anim(const std::string &Name, std::vector<unsigned int> &Frames, float FrameRate = 0, bool Looped = true) {
        name = Name;
        delay = 0;
        if(FrameRate > 0)
            delay = 1.0/FrameRate;
        frames = Frames;
        looped = Looped;
    };
};

NAMESPACE_END    
#endif /* __ANIM_HPP__ */
