#ifndef __TEXT_HPP__
#define __TEXT_HPP__

#if defined(SDL_ENGINE)
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
