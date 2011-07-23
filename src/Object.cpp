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

#include "include/Object.hpp"
#include "include/Globals.hpp"
#include "include/Utils.hpp"
#include "include/Point.hpp"
#include "include/Sprite.hpp"
#include <cmath>

NAMESPACE_BEGIN

Object::Object(float X, float Y, float Width, float Height):
    Rect(X,Y,Width,Height),
    exists(true),
    active(true),
    visible(true),
    _solid(true),
    _fixed(false),
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

Object::~Object()
{
    delete_all(colOffsets);
}

void destroy()
{

}

void Object::refreshHulls()
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

void Object::updateMotion()
{
    if(!moves)
        return;

    if(_solid)
        refreshHulls();
    onFloor = false;
    float vc;

    vc = (Utils::computeVelocity(angularVelocity, angularAcceleration, angularDrag, maxAngular) - angularVelocity)/2;

    angularVelocity += vc;
    angle = fmod(angle + angularVelocity*Globals::elapsed, 360);
    angularVelocity += vc;

    Point thrustComponents;
    Point maxComponents;
    if(thrust != 0)
    {
        Utils::rotatePoint(-thrust,0,0,0,angle,&thrustComponents);
        Utils::rotatePoint(-maxThrust,0,0,0,angle,&maxComponents);
        float max = ((maxComponents.x>0)?maxComponents.x:-maxComponents.x);
        if(max > ((maxComponents.y>0)?maxComponents.y:-maxComponents.y))
            maxComponents.y = max;
        else
            max = ((maxComponents.y>0)?maxComponents.y:-maxComponents.y);
        maxVelocity.x = maxVelocity.y = ((max>0)?max:-max);
    }

    vc = (Utils::computeVelocity(velocity.x,acceleration.x+thrustComponents.x,drag.x,maxVelocity.x) - velocity.x)/2;
    velocity.x += vc;
    float xd = velocity.x*Globals::elapsed;
    velocity.x += vc;

    vc = (Utils::computeVelocity(velocity.y,acceleration.y+thrustComponents.y,drag.y,maxVelocity.y) - velocity.y)/2;
    velocity.y += vc;
    float yd = velocity.y*Globals::elapsed;
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

void Object::updateFlickering()
{
    if(flickering())
    {
        if(_flickerTimer > 0)
        {
            _flickerTimer -= Globals::elapsed;
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

void Object::update()
{
    updateMotion();
    updateFlickering();
}

bool Object::overlaps(Object *Obj)
{
    getScreenXY(_point);
    float tx = _point.x;
    float ty = _point.y;

    Obj->getScreenXY(_point);
    if((_point.x <= tx-Obj->width) || (_point.x >= tx+width) || (_point.y <=ty-Obj->height) || (_point.y >= ty+height))
        return false;
    return true;
}

bool Object::overlapsPoint(float X,float Y,bool PerPixel)
{
    X += Utils::floorValue(Globals::scroll.x);
    Y += Utils::floorValue(Globals::scroll.y);
    getScreenXY(_point);
    if((X <= _point.x) || (X >= _point.x+width) || (Y <= _point.y) || (Y >= _point.y+height))
        return false;
    return true;
}

bool Object::collide(Object *Obj)
{
    return Utils::collide(this,((!Obj)?this:Obj));
}

void Object::hitLeft(Object *Contact, float Velocity)
{
    if(!_fixed) velocity.x = Velocity;
}

void Object::hitRight(Object *Contact, float Velocity)
{
    hitLeft(Contact,Velocity);
}

void Object::hitTop(Object *Contact, float Velocity)
{
    if(!_fixed) velocity.y = Velocity;
}

void Object::hitBottom(Object *Contact, float Velocity)
{
    onFloor = true;
    if(!_fixed) velocity.y = Velocity;
}

void Object::hurt(int Damage)
{
    if((health -= Damage) <= 0)	kill();
}

void Object::kill()
{
    exists = false;
    dead = true;
}

void Object::flicker(int Duration)
{
    _flickerTimer = Duration;
    if(_flickerTimer < 0)
    {
        _flicker = false;
        visible = true;
    }
}

bool Object::flickering()
{
    return _flickerTimer >= 0;
}

Point* Object::getScreenXY(Point &Point)
{
    Point.x = Utils::floorValue(x + Utils::roundingError)+Utils::floorValue(Globals::scroll.x*scrollFactor.x);
    Point.y = Utils::floorValue(y + Utils::roundingError)+Utils::floorValue(Globals::scroll.y*scrollFactor.y);
    return &Point;
}

bool Object::onScreen()
{
    getScreenXY(_point);
    if((_point.x + width < 0) || (_point.x > (int) Globals::width) || (_point.y + height < 0) || (_point.y > (int) Globals::height))
        return false;
    return true;
}

void Object::reset(float X, float Y)
{
    x = X;
    y = Y;
    exists = true;
    dead = false;
}

unsigned int Object::getBoundingColor()
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

NAMESPACE_END
