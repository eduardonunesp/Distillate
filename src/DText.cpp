#include "DText.hpp"

namespace Distillate
{

DText::DText(float X, float Y, unsigned int Width, const std::string &Text, bool EmbeddedFont):
DSprite(X,Y),
_text(""),
_font_file(""),
_alignment(""),
_regen(false),
_shadow(false),
_embedded(false),
_size(14),
_color(0),
_shadow_color(0xff0000ff)

{
    width = Width;
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
        createGraphic(width,TTF_FontHeight(TextField), _color);
        SDL_Color fg = { 255, 255, 255, 0 };
        SDL_Color bg = { 255, 0, 0, 0 };
        _shadow_color = 0xff0000ff;
        SDL_Surface *fgs = 0;
        SDL_Surface *bgs = 0;

        if(_shadow_color != 0)
        {
            fgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), fg); 
            bgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), bg); 

            if(fgs && bgs)
            {
                SDL_Rect r;
                r.x = fgs->clip_rect.x - 1;
                r.y = fgs->clip_rect.y - 1;
                SDL_BlitSurface(bgs,0,_pixels,0);
                SDL_BlitSurface(fgs,0,_pixels,&r);
            }
        }
        else
        {
            fgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), fg);
            
            if(fgs)
                SDL_BlitSurface(fgs,0,_pixels,0);
        }

        _regen = false;
    }
}

}
