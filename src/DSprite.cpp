#include "Distillate/DSprite.hpp"
#include "Distillate/DPoint.hpp"
#include "Distillate/Data/DAnim.hpp"
#include "Distillate/DGlobals.hpp"

namespace Distillate
{
    SDL::Sprite* DSprite::_gfxSprite = new SDL::Sprite();
    SDL::Graphics* DSprite::_gfx = new SDL::Graphics();

    DSprite::DSprite(unsigned int X, unsigned int Y, void* SimpleGraphics):
        DObject(),
        offset(new DPoint()),
        scale(new DPoint(1,1)),
        blend(""),
        antialiasing(false),

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
        x = X;
        y = Y;
    }

    DSprite::~DSprite()
    {
        delete offset;
        delete scale;
        delete _curAnim;

        delete_all(_animations);
    }

    DSprite* DSprite::loadGraphic(void *Graphic, bool Animated, bool Reverse, unsigned int Width, unsigned int Height, bool Unique)
    {
        _bakedRotation = 0;
        _pixels = DGlobals::addBitmap(Graphic, Reverse, Unique);

        /*

        if(Reverse)
            _flipped = _pixels->width>>1;
        else
            _flipped = 0;

        if(Width == 0)
        {
            if(Animated)
                Width = _pixels->height;
            else if(_flipped > 0)
                Width = _pixels->width*0.5;
            else 
                Width = _pixels->width;
        }

        width = frameWidth = Width;
        
        if(Height == 0)
        {
            if(Animated)
                Height = width;
            else
                Height = _pixels->height;
        }

        height = frameHeight = Height;
        resetHelpers();
        return this;
        */

        return NULL;
    }
}
