#include "DObject.hpp"
#include "DGlobals.hpp"
#include "DUtils.hpp"
#include "DPoint.hpp"
#include "DSprite.hpp"
#include <cmath>

namespace Distillate
{

DObject::DObject(float X, float Y, float Width, float Height):
    DRect(X,Y,Width,Height),
    exists(true),
    active(true),
    visible(true),
    _solid(true),
    _fixed(false),
    _is_sprite(false),
    maxVelocity(10000,10000),
    angle(0),
    angularVelocity(0),
    angularAcceleration(0),
    angularDrag(0),
    maxAngular(10000),
    thrust(0),
    maxThrust(0.0f),
    scrollFactor(1,1),
    _flicker(0),
    _flickerTimer(-1),
    health(1),
    dead(false),
    moves(true),
    _group(false),
    collideLeft(true),
    collideRight(true),
    collideTop(true),
    collideBottom(true) {}

DObject::~DObject()
{
    delete_all(colOffsets);
}

void destroy()
{

}

void DObject::refreshHulls()
{
    colHullX.x = x;
    colHullX.y = y;
    colHullX.width = width;
    colHullX.height = height;
    colHullY.x = x;
    colHullY.y = y;
    colHullY.width = width;
    colHullY.height = height;
}

void DObject::updateMotion()
{

    if(!moves)
        return;

    if(_solid)
        refreshHulls();
    onFloor = false;
    float vc;

    vc = (DUtils::computeVelocity(angularVelocity, angularAcceleration, angularDrag, maxAngular) - angularVelocity)/2;

    angularVelocity += vc;
    angle = fmod(angle + angularVelocity*DGlobals::elapsed, 360);
    angularVelocity += vc;

    DPoint* thrustComponents;
    if(thrust != 0)
    {
        thrustComponents = DUtils::rotatePoint(-thrust,0,0,0,angle);
        DPoint* maxComponents = DUtils::rotatePoint(-maxThrust,0,0,0,angle);
        float max = ((maxComponents->x>0)?maxComponents->x:-maxComponents->x);
        if(max > ((maxComponents->y>0)?maxComponents->y:-maxComponents->y))
            maxComponents->y = max;
        else
            max = ((maxComponents->y>0)?maxComponents->y:-maxComponents->y);
        maxVelocity.x = maxVelocity.y = ((max>0)?max:-max);
    }
    else
        thrustComponents = new DPoint();

    vc = (DUtils::computeVelocity(velocity.x,acceleration.x+thrustComponents->x,drag.x,maxVelocity.x) - velocity.x)/2;
    velocity.x += vc;
    float xd = velocity.x*DGlobals::elapsed;
    velocity.x += vc;

    vc = (DUtils::computeVelocity(velocity.y,acceleration.y+thrustComponents->y,drag.y,maxVelocity.y) - velocity.y)/2;
    velocity.y += vc;
    float yd = velocity.y*DGlobals::elapsed;
    velocity.y += vc;

    x += xd;
    y += yd;

    if(!_solid)
        return;
    colVector.x = xd;
    colVector.y = yd;
    colHullX.width += ((colVector.x>0)?colVector.x:-colVector.x);
    if(colVector.x < 0)
        colHullX.x += colVector.x;
    colHullY.x = x;
    colHullY.height += ((colVector.y>0)?colVector.y:-colVector.y);
    if(colVector.y < 0)
        colHullY.y += colVector.y;
}

void DObject::updateFlickering()
{
    if(flickering())
    {
        if(_flickerTimer > 0)
        {
            _flickerTimer -= DGlobals::elapsed;
            if(_flickerTimer == 0)
                _flickerTimer = -1;
        }
        if(_flickerTimer < 0)
            flicker(-1);
        else
        {
            _flicker = !_flicker;
            visible = !_flicker;
        }
    }
}

void DObject::update()
{
    updateMotion();
    updateFlickering();
}

bool DObject::overlaps(DObject *Object)
{
    getScreenXY(_point);
    float tx = _point.x;
    float ty = _point.y;

    Object->getScreenXY(_point);
    if((_point.x <= tx-Object->width) || (_point.x >= tx+width) || (_point.y <=ty-Object->height) || (_point.y >= ty+height))
        return false;
    return true;
}

bool DObject::overlapsPoint(float X,float Y,bool PerPixel)
{
    X += DUtils::floorValue(DGlobals::scroll.x);
    Y += DUtils::floorValue(DGlobals::scroll.y);
    getScreenXY(_point);
    if((X <= _point.x) || (X >= _point.x+width) || (Y <= _point.y) || (Y >= _point.y+height))
        return false;
    return true;
}

bool DObject::collide(DObject *Object)
{
    return DUtils::collide(this,((!Object)?this:Object));
}

void DObject::hitLeft(DObject *Contact, float Velocity)
{
    if(!_fixed) velocity.x = Velocity;
}

void DObject::hitRight(DObject *Contact, float Velocity)
{
    hitLeft(Contact,Velocity);
}

void DObject::hitTop(DObject *Contact, float Velocity)
{
    if(!_fixed) velocity.y = Velocity;
}

void DObject::hitBottom(DObject *Contact, float Velocity)
{
    onFloor = true;
    if(!_fixed) velocity.y = Velocity;
}

void DObject::hurt(int Damage)
{
    if((health -= Damage) <= 0)	kill();
}

void DObject::kill()
{
    exists = false;
    dead = true;
}

void DObject::flicker(int Duration)
{
    _flickerTimer = Duration;
    if(_flickerTimer < 0)
    {
        _flicker = false;
        visible = true;
    }
}

bool DObject::flickering()
{
    return _flickerTimer >= 0;
}

DPoint* DObject::getScreenXY(DPoint &Point)
{
    Point.x = DUtils::floorValue(x + DUtils::roundingError)+DUtils::floorValue(DGlobals::scroll.x*scrollFactor.x);
    Point.y = DUtils::floorValue(y + DUtils::roundingError)+DUtils::floorValue(DGlobals::scroll.y*scrollFactor.y);
    return &Point;
}

bool DObject::onScreen()
{
    getScreenXY(_point);
    if((_point.x + width < 0) || (_point.x > (int) DGlobals::width) || (_point.y + height < 0) || (_point.y > (int) DGlobals::height))
        return false;
    return true;
}

void DObject::reset(float X, float Y)
{
    x = X;
    y = Y;
    exists = true;
    dead = false;
}

unsigned int DObject::getBoundingColor()
{
    if(_solid)
    {
        if(_fixed)
            return 0x7f00f225;
        else
            return 0x7fff0012;
    }
    else
        return 0x7f0090e9;
}

}
