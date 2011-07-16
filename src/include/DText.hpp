#ifndef __DTEXT_HPP__
#define __DTEXT_HPP__

#ifdef SDL_RENDER
#include <SDL/SDL_ttf.h>
#endif

#include <string>
#include "DSprite.hpp"

namespace Distillate 
{

/**
 * Extends <code>DSprite</code> to support rendering text.
 * Can tint, fade, rotate and scale just like a sprite.
 * Doesn't really animate though, as far as I know.
 * Also does nice pixel-perfect centering on pixel fonts
 * as long as they are only one liners.
 */
class DText : public DSprite
{
protected:
#ifdef SDL_RENDER
    TTF_Font *TextField;
#endif
    std::string _text;
    std::string _font_file;
    std::string _alignment;
    bool _regen;
    bool _resize;
    bool _shadow;
    bool _embedded;
    unsigned int _size;
    unsigned int _color;
    unsigned int _shadow_color;

public:    
    /**
     * Creates a new <code>DText</code> object at the specified position.
     * 
     * @param	X				The X position of the text.
     * @param	Y				The Y position of the text.
     * @param	Width			The width of the text object (height is determined automatically).
     * @param	Text			The actual text you would like to display initially.
     * @param	EmbeddedFont	Whether this text field uses embedded fonts or nto
     */
    DText(float X, float Y, unsigned int Width, const std::string &Text="", bool EmbeddedFont=false);
    ~DText(); 
    
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
     * @return	This DText instance (nice for chaining stuff together, if you're into that).
     */
    DText * setFormat(const std::string &Font="", unsigned int Size=8, unsigned int Color=0xffffffff, const std::string &Alignment= "", unsigned int ShadowColor=0);
    
    /**
     * The text being displayed.
     */
    const std::string getText()
    {
        return _text;
    }
    
    /**
     * @private
     */
    void setText(const std::string &Text)
    {
        _text = Text;
        _regen = true;
        calcFrame();
    }
    
    /**
     * The size of the text being displayed.
     */
    unsigned int getSize()
    {
        return _size;
    }
    
    /**
     * @private
     */
    void setSize(unsigned int Size)
    {
        _size = Size;
        _regen = true;
        _resize = true;
        calcFrame();
    }
    
    /**
     * The color of the text being displayed.
     */
    unsigned int getColor()
    {
        return _color;
    }
    
    /**
     * @private
     */
    void setColor(unsigned int Color)
    {
        _color = Color;
        _regen = true;
        calcFrame();
    }
    
    /**
     * The font used for this text.
     */
    const std::string getFont()
    {
        return _font_file;  
    }

    
    /**
     * @private
     */
    void setFont(const std::string &Font)
    {
        _font_file = Font;
        _regen = true;
        calcFrame();
    }
    
    /**
     * The alignment of the font ("left", "right", or "center").
     */
    const std::string getAlignment()
    {
        return _alignment;
    }
    
    /**
     * @private
     */
    void setAlignment(const std::string &Alignment)
    {
        _alignment = Alignment;
        calcFrame();
    }
    
    /**
     * The alignment of the font ("left", "right", or "center").
     */
    unsigned int getShadow()
    {
        return _shadow;
    }
    
    /**
     * @private
     */
    void setShadow(unsigned int Color)
    {
        _shadow = Color;
        calcFrame();
    }
   
protected:   
    /**
     * Internal function to update the current animation frame.
     */
    void calcFrame();
};

}

#endif
