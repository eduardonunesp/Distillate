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

#ifndef __ANIM_HPP__
#define __ANIM_HPP__

#include <cstdio>
#include <string>
#include <vector>
#include "Defs.hpp"

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
