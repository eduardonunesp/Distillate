#include "include/DText.hpp"

namespace Distillate {
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
#ifdef SDL_RENDER
         TTF_CloseFont(TextField);
#endif
    }

    DText * DText::setFormat(const std::string &Font, unsigned int Size, unsigned int Color, const std::string &Alignment, unsigned int ShadowColor)
    {
#ifdef GL_RENDER
#elif SDL_RENDER
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
#endif
         return this;
    }

    void DText::calcFrame()
    {
         if(_regen) {
#ifdef SDL_RENDER
              if(_resize) {
                   _resize = false;
                   TextField = TTF_OpenFont(_font_file.c_str(), _size);
                   if(!TextField) return;
              }

              SDL_Color fg = { 255, 255, 255, 0 };
              SDL_Color bg = { 99, 99, 99, 0 };

              SDL_Surface *fgs = 0;
              SDL_Surface *bgs = 0;

              SDL_FillRect(_pixels, &_pixels->clip_rect, 0x00000000);

              if(_shadow) {
                   fgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), fg);
                   bgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), bg);

                   if(fgs && bgs) {
                        SDL_BlitSurface(bgs, 0,_pixels, 0);
                        SDL_BlitSurface(fgs, 0,_pixels, 0);
                   }
              } else {
                   fgs = TTF_RenderUTF8_Solid(TextField, _text.c_str(), fg);
                   if(fgs)
                        SDL_BlitSurface(fgs, 0,_pixels, 0);
              }
#endif

              _regen = false;
         }
    }
}
