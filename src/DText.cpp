#include "DText.hpp"

namespace Distillate
{

DText::DText(float X, float Y, unsigned int Width, const std::string &Text, bool EmbeddedFont):
DSprite(X,Y)
{
    width = Width;
    _regen = true;
    _shadow = 0;
    _text = Text;
    _embedded = EmbeddedFont;
    solid(false);
}
DText::~DText()
{ 
    TTF_CloseFont(TextField);
}

DText * DText::setFormat(const std::string &Font, unsigned int Size, unsigned int Color, const std::string &Alignment, unsigned int ShadowColor)
{
    if(Font.empty()) return NULL;
    TextField = TTF_OpenFont(Font.c_str(), Size);
    if(!TextField) return NULL;

    createGraphic(width,TTF_FontHeight(TextField), 0x00000000);
    _font_file = Font;
    _size = Size;
    _color = Color;
    _alignment = Alignment;
    _shadow_color = ShadowColor;
    _regen = true;
    calcFrame();
    return this;
}

void DText::calcFrame()
{
    if(_regen)
    {
        createGraphic(width,TTF_FontHeight(TextField), 0x00000000);
        DSprite::update();
        SDL_Color c = { 255, 255, 255, 0 };
        SDL_Surface *s = TTF_RenderUTF8_Solid(TextField, _text.c_str(), c);
        SDL_BlitSurface(s,0,_pixels,0);
        _regen = false;
    }
    /*
    if(_regen)
    {
        //Need to generate a new buffer to store the text graphic
        var i:uint = 0;
        var nl:uint = _tf.numLines;
        height = 0;
        while(i < nl)
            height += _tf.getLineMetrics(i++).height;
        height += 4; //account for 2px gutter on top and bottom
        _pixels = new BitmapData(width,height,true,0);
        _bbb = new BitmapData(width,height,true,0);
        frameHeight = height;
        _tf.height = height*1.2;
        _flashRect.x = 0;
        _flashRect.y = 0;
        _flashRect.width = width;
        _flashRect.height = height;
        _regen = false;
    }
    else	//Else just clear the old buffer before redrawing the text
        _pixels.fillRect(_flashRect,0);
    
    if((_tf != null) && (_tf.text != null) && (_tf.text.length > 0))
    {
        //Now that we've cleared a buffer, we need to actually render the text to it
        var tf:TextFormat = _tf.defaultTextFormat;
        var tfa:TextFormat = tf;
        _mtx.identity();
        //If it's a single, centered line of text, we center it ourselves so it doesn't blur to hell
        if((tf.align == "center") && (_tf.numLines == 1))
        {
            tfa = new TextFormat(tf.font,tf.size,tf.color,null,null,null,null,null,"left");
            _tf.setTextFormat(tfa);				
            _mtx.translate(Math.floor((width - _tf.getLineMetrics(0).width)/2),0);
        }
        //Render a single pixel shadow beneath the text
        if(_shadow > 0)
        {
            _tf.setTextFormat(new TextFormat(tfa.font,tfa.size,_shadow,null,null,null,null,null,tfa.align));				
            _mtx.translate(1,1);
            _pixels.draw(_tf,_mtx,_ct);
            _mtx.translate(-1,-1);
            _tf.setTextFormat(new TextFormat(tfa.font,tfa.size,tfa.color,null,null,null,null,null,tfa.align));
        }
        //Actually draw the text onto the buffer
        _pixels.draw(_tf,_mtx,_ct);
        _tf.setTextFormat(new TextFormat(tf.font,tf.size,tf.color,null,null,null,null,null,tf.align));
    }
    
    //Finally, update the visible pixels
    if((_framePixels == null) || (_framePixels.width != _pixels.width) || (_framePixels.height != _pixels.height))
        _framePixels = new BitmapData(_pixels.width,_pixels.height,true,0);
    _framePixels.copyPixels(_pixels,_flashRect,_flashPointZero);
    if(DG.showBounds)
        drawBounds();
    if(solid)
        refreshHulls();
    */        
}

}
