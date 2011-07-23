#include "include/Sprite.hpp"
#include "include/Point.hpp"
#include "include/Anim.hpp"
#include "include/Globals.hpp"
#include "include/ResourceManager.hpp"
#include "include/TextureResource.hpp"
#include "include/Utils.hpp"
#include <cmath>

namespace Distillate {
    Sprite::Sprite(float X, float Y, const std::string &SimpleGraphics):
        DObject(X, Y),
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
        _is_sprite = true;

#if defined(GL_RENDER) && defined(GL_VBO)
        glGenBuffersARB(1, &vboID);
#elif defined(SDL_RENDER)
        if(!SimpleGraphics.empty()) {
            if(Globals::resourceManager.loadTexture(SimpleGraphics)) {
                _pixels = Globals::resourceManager.texture(SimpleGraphics);
            }
        }
#endif
    }

    Sprite::~Sprite()
    {
        delete_all(_animations);
    }

    Sprite* Sprite::loadGraphic(const std::string &Graphic, bool Animated, bool Reverse, unsigned int Width, unsigned int Height, bool Unique)
    {
        if(!Globals::resourceManager.loadTexture(Graphic))
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
        DObject::update();
        updateAnimation();
    }

    void Sprite::render()
    {
        getScreenXY(_point);
#if defined(GL_RENDER) && defined(GL_VBO)
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DVBO)*3, &spriteVBO[0].x, GL_STREAM_DRAW);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY); 
#elif defined(GL_RENDER)        
        glBegin(GL_QUADS);
        {   
            /* Bottom Left */
            glTexCoord2i(0,0);
            glVertex2f(0,height); 

            /* Bottom Right */
            glTexCoord2i(1,0);
            glVertex2f(width,height);             

            /* Top Right */
            glTexCoord2i(1,1);
            glVertex2f(width,0); 

            /* Top Left */
            glTexCoord2i(0,0);
            glVertex2f(0,1); 
        }   
        glEnd();
#elif defined(SDL_RENDER)
        SDL_Rect rect_dst;

        rect_dst.x = _point.x;
        rect_dst.y = _point.y;

        _rendering_rect.h = height;
        _rendering_rect.w = width;

        if((angle == 0) || (_bakedRotation > 0))
            SDL_BlitSurface(_pixels->data, &_rendering_rect, Globals::_buffer, &rect_dst);
        else
            SDL_BlitSurface(_pixels->data, &_rendering_rect, Globals::_buffer, &rect_dst);

        _rendering_rect.x = 0;
        _rendering_rect.y = 0;
#endif
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

    void Sprite::addAnimation(const std::string &Name, std::vector<int> &Frames, float FrameRate, bool Looped)
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
        unsigned int w = _flipped ? _flipped : _pixels->data->w;
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
