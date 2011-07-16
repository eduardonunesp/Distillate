#include "include/DSprite.hpp"
#include "include/DPoint.hpp"
#include "include/DAnim.hpp"
#include "include/DGlobals.hpp"
#include <cmath>

namespace Distillate
{
    DSprite::DSprite(float X, float Y, const std::string &SimpleGraphics):
        DObject(X, Y),
        finished(false),
        _flipped(0),
        _curAnim(NULL),
        _curFrame(0),
        _caf(0),
        _frameTimer(0),
        _callback(NULL),
        _facing(RIGHT),
        _alpha(1),
        _color(0x00ffffff)
    {
        _is_sprite = true;

#ifdef GL_RENDER

#else
        if(!SimpleGraphics.empty())
            _pixels = DGlobals::addBitmap(SimpleGraphics, false);
#endif
    }

    DSprite::~DSprite()
    {
        delete _curAnim;
        delete_all(_animations);
    }

    DSprite* DSprite::loadGraphic(const std::string &Graphic, bool Animated, bool Reverse, unsigned int Width, unsigned int Height, bool Unique)
    {
#ifdef GL_RENDER       
#else
        _bakedRotation = 0;
        _pixels = DGlobals::addBitmap(Graphic,Reverse,Unique);
        
        if(Width == 0)
        {   
            if(Animated)
                Width = _pixels->w;
            else if(_flipped > 0)
                Width = _pixels->w/2;
            else
                Width = _pixels->w;
        }   

        width = frameWidth = Width;
        if(Height == 0)
        {   
            if(Animated)
                Height = width;
            else
                Height = _pixels->h;
        }   

        height = frameHeight = Height;
#endif
        return this;
    }

    DSprite *DSprite::createGraphic(unsigned int Width, unsigned int Height, unsigned int Color, bool Unique, const std::string &Key)
    {
#ifdef GL_RENDER
#else
        unsigned int rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif
        _bakedRotation = 0;
        _pixels = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,32,rmask, gmask, bmask, amask);
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = Width;
        rect.h = Height;
        SDL_FillRect(_pixels, &rect, Color);
        width = frameWidth = _pixels->w;
        height = frameHeight = _pixels->h;
        resetHelpers();
#endif
        return this;
    }   

    void DSprite::resetHelpers()
    {
        origin.x = frameWidth/2;
        origin.y = frameHeight/2;
        _caf = 0;
        refreshHulls();
    }

    void DSprite::update()
    {
        DObject::update();
        updateAnimation();
    }

    void DSprite::render()
    {
        getScreenXY(_point);
#ifdef GL_RENDER
        
        glBindTexture( GL_TEXTURE_2D, texture );

        glBegin( GL_QUADS );
        glTexCoord2i( 0, 0 );
        glVertex3f( 100.f, 100.f, 0.0f );

        glTexCoord2i( 1, 0 );
        glVertex3f( 228.f, 100.f, 0.f );

        glTexCoord2i( 1, 1 );
        glVertex3f( 228.f, 228.f, 0.f );

        glTexCoord2i( 0, 1 );
        glVertex3f( 100.f, 228.f, 0.f );
        glEnd();
#else
        SDL_Rect rect_dst;

        rect_dst.x = _point.x;
        rect_dst.y = _point.y;

        _rendering_rect.h = height;
        _rendering_rect.w = width;

        if((angle == 0) || (_bakedRotation > 0))
            SDL_BlitSurface(_pixels, &_rendering_rect, DGlobals::_buffer, &rect_dst);
        else
            SDL_BlitSurface(_pixels, &_rendering_rect, DGlobals::_buffer, &rect_dst);

        _rendering_rect.x = 0;
        _rendering_rect.y = 0;
#endif
    }

    bool DSprite::overlapsPoint(unsigned int X, unsigned int Y, bool PerPixel)
    {   
        X -= DUtils::floorValue(DGlobals::scroll.x);
        Y -= DUtils::floorValue(DGlobals::scroll.y);

        getScreenXY(_point);

        if((X <= _point.x) || (X >= _point.x+frameWidth) || (Y <= _point.y) || (Y >= _point.y+frameHeight))
            return false;
        return true;
    }   
        
    void DSprite::addAnimation(const std::string &Name, std::vector<int> &Frames, float FrameRate, bool Looped)
    {   
        _animations.push_back(new DAnim(Name, Frames, FrameRate, Looped));
    }   

    void DSprite::play(const std::string &AnimName, bool Force)
    {
        if(!Force && (_curAnim != NULL) && (AnimName == _curAnim->name) && (_curAnim->looped || !finished)) return;
        _curFrame = 0;
        _caf = 0;
        _frameTimer = 0;
        unsigned int al = _animations.size();
        for(unsigned int i = 0; i < al; i++)
        {   
            if(_animations[i]->name == AnimName)
            {   
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

    DPoint * DSprite::getScreenXY(DPoint &Point)
    {   
        Point.x = DUtils::floorValue(x + DUtils::roundingError)+DUtils::floorValue(DGlobals::scroll.x*scrollFactor.x) - offset.x;
        Point.y = DUtils::floorValue(y + DUtils::roundingError)+DUtils::floorValue(DGlobals::scroll.y*scrollFactor.y) - offset.y;
        return &Point;
    }   

    void DSprite::calcFrame()
    {   
#ifdef GL_RENDER
#else
        unsigned int rx = _caf*frameWidth;
        unsigned int ry = 0;

        /* Handle sprite sheets */
        unsigned int w = _flipped ? _flipped : _pixels->w;
        if(rx >= w)
        {   
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
#endif
    }   

    /** 
     * Internal function for updating the sprite's animation.
     * Useful for cases when you need to update this but are buried down in too many supers.
     * This function is called automatically by <code>FlxSprite.update()</code>.
     */
    void  DSprite::updateAnimation()   
    {
        if(_bakedRotation)
        {   
            unsigned int oc = _caf;
            _caf = fmod(angle,360) / _bakedRotation;
            if(oc != _caf)
                calcFrame();
            return;
        }   
        
        if((_curAnim) && (_curAnim->delay > 0) && (_curAnim->looped || !finished))
        {   
            _frameTimer += DGlobals::elapsed;
            if(_frameTimer > _curAnim->delay)
            {   
                _frameTimer -= _curAnim->delay;
                if(_curFrame == _curAnim->frames.size()-1)
                {   
                    if(_curAnim->looped) _curFrame = 0;
                    finished = true;
                }   
                else
                {
                    _curFrame++;
                }

                _caf = _curAnim->frames[_curFrame];
                calcFrame();
            }   
        }   
    }
}
