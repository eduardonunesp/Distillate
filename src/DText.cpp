#include "include/DText.hpp"
#include "include/DResourceManager.hpp"
#include "include/DTTFResource.hpp"
#include "include/DTextureResource.hpp"
#include "include/DGlobals.hpp"

namespace Distillate {
    DText::DText(float X, float Y, unsigned int Width, const std::string &Text, bool EmbeddedFont):
        DSprite(X,Y),
        _text(Text),
        _alignment(""),
        _regen(false),
        _resize(false),
        _shadow(false),
        _embedded(false),
        _color(0xffffffff),
        _shadow_color(0x00000000)

    {
        width = Width;
        solid(false);
    }

    DText::~DText()
    {
    }

    unsigned int DText::getSize() {
        return ttf->size;
    }

    void DText::setSize(unsigned int Size) {
        ttf->size = Size;
        _regen = true;
        _resize = true;
        calcFrame();
    }

    DText * DText::setFormat(const std::string &Font, unsigned int Size, unsigned int Color, const std::string &Alignment, unsigned int ShadowColor)
    {
        if(Font.empty())
            return NULL;

        if(!DGlobals::resourceManager.loadTTF(Font, width, Size, Color))
            return NULL;

        ttf = DGlobals::resourceManager.ttf(Font);

        if(!ttf)
            return NULL;

        if(!createGraphic(_text.substr(0,1),width, ttf->h, Color))
            return NULL;

        _pixels = DGlobals::resourceManager.texture(_text.substr(0,1));

        _color = Color;
        _alignment = Alignment;
        _shadow_color = ShadowColor;
        _regen = true;
        calcFrame();
        return this;
    }
    void DText::calcFrame()
    {
        if(_regen) {
            if(_resize) {
                _resize = false;
                DGlobals::resourceManager.unloadTTF(ttf->resourceid);
                if(!DGlobals::resourceManager.loadTTF(ttf->filename, ttf->w, ttf->size, ttf->color))
                    return;
                ttf = DGlobals::resourceManager.ttf(ttf->resourceid);
            }
#ifdef SDL_RENDER
            SDL_Color fg = { 255, 255, 255, 0 };
            SDL_Color bg = { 99, 99, 99, 0 };

            SDL_Surface *fgs = 0;
            SDL_Surface *bgs = 0;

            SDL_FillRect(_pixels->data, &_pixels->data->clip_rect, 0xff000000);

            if(_shadow) {
                fgs = TTF_RenderUTF8_Solid(ttf->data, _text.c_str(), fg);
                bgs = TTF_RenderUTF8_Solid(ttf->data, _text.c_str(), bg);

                if(fgs && bgs) {
                    SDL_BlitSurface(bgs, 0,_pixels->data, 0);
                    SDL_BlitSurface(fgs, 0,_pixels->data, 0);
                }
            } else {
                fgs = TTF_RenderUTF8_Solid(ttf->data, _text.c_str(), fg);
                if(fgs) {
                    SDL_BlitSurface(fgs, 0,_pixels->data, 0);
                }
            }
#endif

            _regen = false;
        }
    }
}
