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

#ifndef __RECT_HPP__
#define __RECT_HPP__

#include "Defs.hpp"
#include "Point.hpp"

NAMESPACE_BEGIN

/**
 * Stores a rectangle
 */
class Rect : public Point {
public:
    /**
     * @default 0
     */
    float width;

    /**
     * @default 0
     */
    float height;

    /**
     * Instantiate a new rectangle.
     *
     * @param   X       The X-coordinate of the point in space.
     * @param   Y       The Y-coordinate of the point in space.
     * @param   Width   Desired width of the rectangle.
     * @param   Height  Desired height of the rectangle.
     */
    Rect(float X=0, float Y=0, float Width=0, float Height=0)
        : Point(X,Y), width(Width), height(Height) {}
    ~Rect() {}

    /**
     * The X coordinate of the left side of the rectangle.  Read-only.
     */
    unsigned int getLeft() {
        return x;
    }

    /**
     * The X coordinate of the right side of the rectangle.  Read-only.
     */
    unsigned int getRight() {
        return x + width;
    }

    /**
     * The Y coordinate of the top of the rectangle.  Read-only.
     */
    unsigned int getTop() {
        return y;
    }

    /**
     * The Y coordinate of the bottom of the rectangle.  Read-only.
     */
    unsigned int getBottom() {
        return y + height;
    }
};

NAMESPACE_END
#endif /* __RECT_HPP */
