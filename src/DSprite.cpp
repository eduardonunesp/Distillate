#include "Distillate/DSprite.hpp"
#include "Distillate/DPoint.hpp"
#include "Distillate/data/DAnim.hpp"

namespace Distillate
{
    SDL::Sprite* DSprite::_gfxSprite = new SDL::Sprite();
    SDL::Graphics* DSprite::_gfx = new SDL::Graphics();

    DSprite::DSprite(unsigned int X, unsigned int Y, void* SimpleGraphics):
        DObject(),
        offset(new DPoint()),
        scale(new DPoint(1,1)),
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
        return NULL;
    }
}
