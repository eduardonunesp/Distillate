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

#include "include/Text.hpp"
#include "include/ResourceManager.hpp"
#include "include/TTFResource.hpp"
#include "include/TextureResource.hpp"
#include "include/Globals.hpp"
#include "include/Utils.hpp"

NAMESPACE_BEGIN

Text::Text(float X, float Y, unsigned int Width, const std::string &Text, bool EmbeddedFont):
    Sprite(X,Y),
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

Text::~Text()
{
}

unsigned int Text::getSize() {
    return ttf->size;
}

void Text::setSize(unsigned int Size) {
    ttf->size = Size;
    _regen = true;
    _resize = true;
    calcFrame();
}

Text * Text::setFormat(const std::string &Font, unsigned int Size, unsigned int Color, const std::string &Alignment, unsigned int ShadowColor)
{
    if(Font.empty())
        return NULL;

    if(!Globals::resourceManager.loadTTF(Font, width, Size, Color))
        return NULL;

    ttf = Globals::resourceManager.ttf(Font);

    if(!ttf)
        return NULL;

    if(!createGraphic(_text.substr(0,1),width, ttf->h, Color))
        return NULL;

    _pixels = Globals::resourceManager.texture(_text.substr(0,1));

    _color = Color;
    _alignment = Alignment;
    _shadow_color = ShadowColor;
    _regen = true;
    calcFrame();
    return this;
}
void Text::calcFrame()
{
    if(_regen) {
        if(_resize) {
            _resize = false;
            Globals::resourceManager.unloadTTF(ttf->resourceid);
            if(!Globals::resourceManager.loadTTF(ttf->filename, ttf->w, ttf->size, ttf->color))
                return;
            ttf = Globals::resourceManager.ttf(ttf->resourceid);
        }
#if defined(SDL_VIDEO) && defined(SW_RENDER)
        SDL_Color fg = { SDL_COLOR_FROM_UI32(ttf->color) };
        SDL_Color bg = { SDL_COLOR_FROM_UI32(ttf->color) };

        SDL_Surface *fgs = 0;
        SDL_Surface *bgs = 0;

        SDL_FillRect(_pixels->data[0], &_pixels->data[0]->clip_rect, 0);

        if(_shadow) {
            fgs = TTF_RenderUTF8_Solid(ttf->data, _text.c_str(), fg);
            bgs = TTF_RenderUTF8_Solid(ttf->data, _text.c_str(), bg);

            if(fgs && bgs) {
                SDL_BlitSurface(bgs, 0,_pixels->data[0], 0);
                SDL_BlitSurface(fgs, 0,_pixels->data[0], 0);
            }
        } else {
            fgs = TTF_RenderUTF8_Solid(ttf->data, _text.c_str(), fg);
            if(fgs) {
                SDL_BlitSurface(fgs, 0,_pixels->data[0], 0);
            }
        }
#endif
        _regen = false;
    }
}

NAMESPACE_END
