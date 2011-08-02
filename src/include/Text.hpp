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

#ifndef __TEXT_HPP__
#define __TEXT_HPP__

#if defined(SDL_VIDEO)
#include <SDL/SDL_ttf.h>
#endif

#include <string>
#include "Defs.hpp"
#include "Sprite.hpp"

NAMESPACE_BEGIN

/* Forwards */ 
class TTFResource;

/**
 * Extends <code>Sprite</code> to support rendering text.
 * Can tint, fade, rotate and scale just like a sprite.
 * Doesn't really animate though, as far as I know.
 * Also does nice pixel-perfect centering on pixel fonts
 * as long as they are only one liners.
 */
class Text : public Sprite {
protected:
    TTFResource *ttf;
    std::string _text;
    std::string _alignment;
    bool _regen;
    bool _resize;
    bool _shadow;
    bool _embedded;
    unsigned int _color;
    unsigned int _shadow_color;

public:
    /**
     * Creates a new <code>Text</code> object at the specified position.
     *
     * @param	X				The X position of the text.
     * @param	Y				The Y position of the text.
     * @param	Width			The width of the text object (height is determined automatically).
     * @param	Text			The actual text you would like to display initially.
     * @param	EmbeddedFont	Whether this text field uses embedded fonts or nto
     */
    Text(float X, float Y, unsigned int Width, const std::string &Text="", bool EmbeddedFont=false);
    ~Text();

    /**
     * You can use this if you have a lot of text parameters
     * to set instead of the individual properties.
     *
     * @param	Font		The name of the font face for the text display.
     * @param	Size		The size of the font (in pixels essentially).
     * @param	Color		The color of the text in traditional flash 0xRRGGBB format.
     * @param	Alignment	A string representing the desired alignment ("left,"right" or "center").
     * @param	ShadowColor	A uint representing the desired text shadow color in flash 0xRRGGBB format.
     *
     * @return	This Text instance (nice for chaining stuff together, if you're into that).
     */
    Text * setFormat(const std::string &Font, unsigned int Size=8, unsigned int Color=0xffffffff, const std::string &Alignment= "", unsigned int ShadowColor=0);

    /**
     * The text being displayed.
     */
    const std::string getText() {
        return _text;
    }

    /**
     * @private
     */
    void setText(const std::string &Text) {
        _text = Text;
        _regen = true;
        calcFrame();
    }

    /**
     * The size of the text being displayed.
     */
    unsigned int getSize();

    /**
     * @private
     */
    void setSize(unsigned int Size);

    /**
     * The color of the text being displayed.
     */
    unsigned int getColor() {
        return _color;
    }

    /**
     * @private
     */
    void setColor(unsigned int Color) {
        _color = Color;
        _regen = true;
        calcFrame();
    }

    /**
     * The alignment of the font ("left", "right", or "center").
     */
    const std::string getAlignment() {
        return _alignment;
    }

    /**
     * @private
     */
    void setAlignment(const std::string &Alignment) {
        _alignment = Alignment;
        calcFrame();
    }

    /**
     * The alignment of the font ("left", "right", or "center").
     */
    unsigned int getShadow() {
        return _shadow;
    }

    /**
     * @private
     */
    void setShadow(unsigned int Color) {
        _shadow = Color;
        calcFrame();
    }

protected:
    /**
     * Internal function to update the current animation frame.
     */
    void calcFrame();
};

NAMESPACE_END
#endif /* __TEXT_HPP__ */
