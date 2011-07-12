#include "DSprite.hpp"
#include "DPoint.hpp"
#include "DAnim.hpp"
#include "DGlobals.hpp"

namespace Distillate
{
    DSprite::DSprite(unsigned int X, unsigned int Y, const std::string &SimpleGraphics):
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

    void DSprite::update()
    {
        DObject::update();
        //updateAnimation();
    }

    void DSprite::render()
    {
        DObject::render();
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        SDL_BlitSurface(_pixels, NULL, DGlobals::_buffer, &rect);
    }
}
