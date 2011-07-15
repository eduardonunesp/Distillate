#include "DText.hpp"

namespace Distillate
{

DText::DText(float X, float Y, unsigned int Width, const std::string &Text, bool EmbeddedFont):
DSprite(X,Y),
_text(Text),
_font_file(""),
_alignment(""),
_regen(false),
_resize(false),
_shadow(false),
_embedded(false),
_size(8),
_color(0xffffffff),
_shadow_color(0x00000000)

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
    if(Font.empty()) 
        return NULL;
    
    TextField = TTF_OpenFont(Font.c_str(), Size);
    
    if(!TextField) 
        return NULL;

    createGraphic(width,TTF_FontHeight(TextField), _color);

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
        if(_resize)
        {
            _resize = false;
            TextField = TTF_OpenFont(_font_file.c_str(), _size);
            if(!TextField) return;
        }

        SDL_Color fg = { 
            ((_color >> 26) & 0xff0000ff), 
            ((_color >> 26) & 0xff00ff00), 
            ((_color >> 26) & 0xffff0000), 0};

        printf("fg %d %d %d\n", fg.r, fg.g, fg.b);

        SDL_Color bg = { 
            ((_color >> 26) & 0xff0000ff), 
            ((_color >> 26) & 0xff00ff00), 
            ((_color >> 26) & 0xffff0000), 0};

        printf("bg %d %d %d\n", fg.r, fg.g, fg.b);

        SDL_Surface *fgs = 0;
        SDL_Surface *bgs = 0;
        
        SDL_FillRect(_pixels, &_pixels->clip_rect, 0x00000000);

        if(_shadow_color != 0)
        {
            fgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), fg); 
            bgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), bg); 

            if(fgs && bgs)
            {
                SDL_BlitSurface(bgs, &bgs->clip_rect,_pixels, &_pixels->clip_rect);
                SDL_BlitSurface(fgs, &fgs->clip_rect,_pixels, &_pixels->clip_rect);
            }
        }
        else
        {
            fgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), fg);
            if(fgs)
                SDL_BlitSurface(fgs, &fgs->clip_rect,_pixels, &_pixels->clip_rect);
        }
        
        _regen = false;
    }
}

}
