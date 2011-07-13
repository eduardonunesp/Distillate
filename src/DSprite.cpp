#include "DSprite.hpp"
#include "DPoint.hpp"
#include "DAnim.hpp"
#include "DGlobals.hpp"
#include <cmath>

namespace Distillate
{
    DSprite::DSprite(float X, float Y, const std::string &SimpleGraphics):
        DObject(X, Y),
        offset(new DPoint()),
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

        if(!SimpleGraphics.empty())
            _pixels = DGlobals::addBitmap(SimpleGraphics, false);
    }

    DSprite::~DSprite()
    {
        delete _curAnim;
        delete_all(_animations);
    }

    DSprite* DSprite::loadGraphic(const std::string &Graphic, bool Animated, bool Reverse, unsigned int Width, unsigned int Height, bool Unique)
    {
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
        return this;
    }

    DSprite *DSprite::createGraphic(unsigned int Width, unsigned int Height, unsigned int Color, bool Unique, const std::string &Key)
    {
        unsigned int rmask, gmask, bmask, amask;
        rmask = (Color>>0);
        gmask = (Color>>2);
        bmask = (Color>>4);
        amask = (Color>>6);

        _bakedRotation = 0;
        _pixels = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,32,rmask, gmask, bmask, amask);
        
        width = frameWidth = _pixels->w;
        height = frameHeight = _pixels->h;
        resetHelpers();
        return this;
    }   

    void DSprite::resetHelpers()
    {
        origin->x = frameWidth/2;
        origin->y = frameHeight/2;
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
        
        SDL_Rect rect_dst;
        rect_dst.x = _point->x;
        rect_dst.y = _point->y;

        SDL_Rect rect_src;
        rect_src.x = 0;
        rect_src.y = 0;
        rect_src.h = height;
        rect_src.w = width;
       
        if((angle == 0) || (_bakedRotation > 0))
            SDL_BlitSurface(_pixels, &rect_src, DGlobals::_buffer, &rect_dst);
    }

    bool DSprite::overlapsPoint(unsigned int X, unsigned int Y, bool PerPixel)
    {   
        X -= DUtils::floorValue(DGlobals::scroll->x);
        Y -= DUtils::floorValue(DGlobals::scroll->y);

        getScreenXY(_point);

        if((X <= _point->x) || (X >= _point->x+frameWidth) || (Y <= _point->y) || (Y >= _point->y+frameHeight))
            return false;
        return true;
    }   
        
    void DSprite::addAnimation(const std::string &Name, std::vector<int> &Frames, float FrameRate, bool Looped)
    {   
        _animations.push_back(new DAnim(Name, Frames, FrameRate, Looped));
    }   

    void DSprite::play(const std::string &AnimName, bool Force)
    {
        if(!Force && _curAnim && (AnimName == _curAnim->name)) return;
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

    DPoint * DSprite::getScreenXY(DPoint *Point)
    {   
        if(!Point) Point = new DPoint();
        Point->x = DUtils::floorValue(x + DUtils::roundingError)+DUtils::floorValue(DGlobals::scroll->x*scrollFactor->x) - offset->x;
        Point->y = DUtils::floorValue(y + DUtils::roundingError)+DUtils::floorValue(DGlobals::scroll->y*scrollFactor->y) - offset->y;
        return Point;
    }   

    void DSprite::calcFrame()
    {   
        unsigned int rx = _caf*frameWidth;
        unsigned int ry = 0;

        /* Handle sprite sheets */
        unsigned int w = _flipped ? _flipped : _pixels->w;
        if(rx >= w)
        {   
            ry = (rx/w)*frameHeight;
            rx = fmod(rx,w);
        }   

        /* handle reversed sprites */
        if(_flipped && (_facing == LEFT))
            rx = (_flipped<<1)-rx-frameWidth;

        /* Update display bitmap */
        if(_callback) _callback(_curAnim, _caf);
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
                    _curFrame++;
                _caf = _curAnim->frames[_curFrame];
                calcFrame();
            }   
        }   
    }
}
