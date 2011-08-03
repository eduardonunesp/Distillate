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

#include "include/Sprite.hpp"
#include "include/Point.hpp"
#include "include/Anim.hpp"
#include "include/Globals.hpp"
#include "include/ResourceManager.hpp"
#include "include/TextureResource.hpp"
#include "include/Utils.hpp"
#include <cmath>

#if defined(SDL_VIDEO) && defined(SW_RENDER)
#include <SDL/SDL_rotozoom.h>
#endif

namespace Distillate {
    Sprite::Sprite(float X, float Y, const std::string &SimpleGraphics):
        Object(X, Y),
        finished(false),
        _flipped(0),
        _curAnim(NULL),
        _curFrame(0),
        _caf(0),
        _frameTimer(0),
        _callback(NULL),
        _facing(RIGHT),
        _alpha(1)
    {

        if(!SimpleGraphics.empty()) {
            if(Globals::resourceManager.loadTexture(SimpleGraphics)) {
                _pixels = Globals::resourceManager.texture(SimpleGraphics);
            }
        }
    }

    Sprite::~Sprite()
    {
        delete_all(_animations);
    }

    Sprite* Sprite::loadGraphic(const std::string &Graphic, bool Animated, bool Reverse, unsigned int Width, unsigned int Height, unsigned int Textures, bool Unique)
    {
        if(!Globals::resourceManager.loadTexture(Graphic, Animated, Width, Height,  Textures, Unique))
            return NULL;

        _bakedRotation = 0;
        _pixels = Globals::resourceManager.texture(Graphic);

        if(Width == 0) {
            if(Animated)
                Width = _pixels->w;
            else if(_flipped > 0)
                Width = _pixels->w/2;
            else
                Width = _pixels->w;
        }

        width = frameWidth = Width;
        if(Height == 0) {
            if(Animated)
                Height = width;
            else
                Height = _pixels->h;
        }

        height = frameHeight = Height;
        return this;
    }

    Sprite *Sprite::createGraphic(const std::string &Key, unsigned int Width, unsigned int Height, unsigned int Color)
    {
        _bakedRotation = 0;

        if(!Globals::resourceManager.createTexture(Key, Width, Height, Color))
            return NULL;

        _pixels = Globals::resourceManager.texture(Key);

        width = frameWidth = _pixels->w;
        height = frameHeight = _pixels->h;
        resetHelpers();

        return this;
    }

    void Sprite::resetHelpers()
    {
        origin.x = frameWidth/2;
        origin.y = frameHeight/2;
        _caf = 0;
        refreshHulls();
    }

    void Sprite::update()
    {
        Object::update();
        updateAnimation();
    }

    void Sprite::renderSprite()
    {
        getScreenXY(_point);
#if defined(SDL_VIDEO) && defined(SW_RENDER)
        SDL_Rect rect_src;
        rect_src.x = _rendering_rect.x;
        rect_src.y = _rendering_rect.y;
        rect_src.w = width;
        rect_src.h = height;

        SDL_Rect rect_dst;
        rect_dst.x = _point.x;
        rect_dst.y = _point.y;

        if((angle == 0) || (_bakedRotation > 0))
            SDL_BlitSurface(_pixels->data[0], &rect_src, Globals::_buffer, &rect_dst);
        else {
            SDL_Surface *tmp_surface;
            tmp_surface = rotozoomSurface(_pixels->data[0], angle, 1, 0);
            SDL_BlitSurface(tmp_surface, 0, Globals::_buffer, &rect_dst);
            SDL_FreeSurface(tmp_surface);
        }
#elif defined(HW_RENDER) && !defined(VBO)

        if(!_pixels->animated)
            glBindTexture( GL_TEXTURE_2D, _pixels->data[0]);
        else
            glBindTexture( GL_TEXTURE_2D, _pixels->data[_caf]);

        glTranslated(_point.x, _point.y, 0.0f);
        glBegin( GL_QUADS );
        {
            glTexCoord2f(0.0f, 0.0f); 
            glVertex2f(0.f,0.0f);

            glTexCoord2f(1.0f,0.0f); 
            glVertex2f(width,0.0f);

            glTexCoord2f(1.0f, 1.0f); 
            glVertex2f(width, height);

            glTexCoord2f(0.0f, 1.0f); 
            glVertex2f(0.0f,height);
        }
        glEnd();
#endif
    }

    void Sprite::render()
    {
        renderSprite();
    }

    bool Sprite::overlapsPoint(unsigned int X, unsigned int Y, bool PerPixel)
    {
        X -= Utils::floorValue(Globals::scroll.x);
        Y -= Utils::floorValue(Globals::scroll.y);

        getScreenXY(_point);

        if((X <= _point.x) || (X >= _point.x+frameWidth) || (Y <= _point.y) || (Y >= _point.y+frameHeight))
            return false;
        return true;
    }

    void Sprite::addAnimation(const std::string &Name, std::vector<unsigned int> &Frames, float FrameRate, bool Looped)
    {
        _animations.push_back(new Anim(Name, Frames, FrameRate, Looped));
    }

    void Sprite::play(const std::string &AnimName, bool Force)
    {
        if(!Force && (_curAnim != NULL) && (AnimName == _curAnim->name) && (_curAnim->looped || !finished)) return;
        _curFrame = 0;
        _caf = 0;
        _frameTimer = 0;
        unsigned int al = _animations.size();
        for(unsigned int i = 0; i < al; i++) {
            if(_animations[i]->name == AnimName) {
                _curAnim = _animations[i];
                if(_curAnim->delay <= 0)
                    finished = true;
                else
                    finished = false;
                _caf = _curAnim->frames[_curFrame];
                calcFrame();
                return;
            }
        }
    }

    Point * Sprite::getScreenXY(Point &Point)
    {
        Point.x = Utils::floorValue(x + Utils::roundingError)+Utils::floorValue(Globals::scroll.x*scrollFactor.x) - offset.x;
        Point.y = Utils::floorValue(y + Utils::roundingError)+Utils::floorValue(Globals::scroll.y*scrollFactor.y) - offset.y;
        return &Point;
    }

    void Sprite::calcFrame()
    {
        unsigned int rx = _caf*frameWidth;
        unsigned int ry = 0;

        /* Handle sprite sheets */
        unsigned int w = _flipped ? _flipped : _pixels->w;
        if(rx >= w) {
            ry = (rx/w)*frameHeight;
            rx = rx % w;
        }

        /* handle reversed sprites */
        if(_flipped && (_facing == LEFT))
            rx = (_flipped<<1)-rx-frameWidth;

        _rendering_rect.x = rx;
        _rendering_rect.y = ry;

        /* Update display bitmap */
        if(_callback) _callback(_curAnim, _caf);
    }

    /**
     * Internal function for updating the sprite's animation.
     * Useful for cases when you need to update this but are buried down in too many supers.
     * This function is called automatically by <code>FlxSprite.update()</code>.
     */
    void  Sprite::updateAnimation()
    {
        if(_bakedRotation) {
            unsigned int oc = _caf;
            _caf = fmod(angle,360) / _bakedRotation;
            if(oc != _caf)
                calcFrame();
            return;
        }

        if((_curAnim) && (_curAnim->delay > 0) && (_curAnim->looped || !finished)) {
            _frameTimer += Globals::elapsed;
            if(_frameTimer > _curAnim->delay) {
                _frameTimer -= _curAnim->delay;
                if(_curFrame == _curAnim->frames.size()-1) {
                    if(_curAnim->looped) _curFrame = 0;
                    finished = true;
                } else {
                    _curFrame++;
                }

                _caf = _curAnim->frames[_curFrame];
                calcFrame();
            }
        }
    }
}
