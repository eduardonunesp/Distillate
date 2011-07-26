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

#include "include/Emitter.hpp"
#include "include/Point.hpp"
#include "include/Globals.hpp"
#include "include/Utils.hpp"
#include "include/Sprite.hpp"
#include "include/Particle.hpp"

NAMESPACE_BEGIN

Emitter::Emitter(float X, float Y):
    Group(),
    minParticleSpeed(-100,-100),
    maxParticleSpeed(100,100),
    minRotation(-360),
    maxRotation(360),
    gravity(400),
    on(false),
    delay(0),
    quantity(0),
    justEmitted(false),
    _explode(0),
    _timer(0),
    _particle(0),
    _counter(0)
{
    x = X;
    y = Y;
    width = 0;
    height = 0;
    exists = false;
}

Emitter::~Emitter()
{
}

Emitter * Emitter::createSprites(const std::string &GraphicFile, unsigned int Quantity, unsigned int BakedRotations, bool Multiple, float Collide, float Bounce)
{
    unsigned int r;
    Sprite *s = NULL;
    unsigned int tf = 1;
    float sw;
    float sh;

    if(Multiple) {
        s = new Sprite();
        s->loadGraphic(GraphicFile, true);
        tf = s->frames;
    }

    unsigned int i = 0;
    while(i < Quantity) {
        if((Collide > 0) && (Bounce > 0))
            s = new Particle(Bounce);
        else
            s = new Sprite();

        if(Multiple) {
            r = Utils::random() * tf;
            if(BakedRotations > 0)
                s->loadGraphic(GraphicFile, true); //TODO: Must rotate !
            else {
                s->loadGraphic(GraphicFile, true);
                s->frame(-r); //WARN
            }
        } else {
            if(BakedRotations > 0)
                s->loadGraphic(GraphicFile, true);
            else
                s->loadGraphic(GraphicFile, true);
        }

        if(Collide) {
            sw = s->width;
            sh = s->height;
            s->width *= Collide;
            s->height *= Collide;
            s->offset.x = (sw-s->width)/2;
            s->offset.y = (sh-s->height)/2;
            s->solid(true);
        } else
            s->solid(false);
        s->exists = false;
        s->scrollFactor = scrollFactor;
        add(s);
        i++;
    }

    return this;
}

void Emitter::updateEmitter()
{
    if(_explode) {
        _timer += Globals::elapsed;
        if((delay > 0) && (_timer > delay)) {
            kill();
            return;
        }

        if(on) {
            on = false;
            unsigned int i = _particle;
            unsigned int l = members.size();
            if(quantity > 0)
                l = quantity;
            l += _particle;
            while(i < l) {
                emitParticle();
                i++;
            }
        }

        return;
    }

    if(!on)
        return;

    _timer += Globals::elapsed;
    while((_timer > delay) && ((quantity <= 0) || (_counter < quantity))) {
        _timer -= delay;
        emitParticle();
    }
}

void Emitter::emitParticle()
{
    _counter++;
    Sprite *s = static_cast<Sprite*>(members[_particle]);
    if(!s) return;

    s->visible = true;
    s->exists  = true;
    s->active  = true;
    s->dead    = false;
    s->x = x - ((int) s->width  >>1) + Utils::random() * width;
    s->y = y - ((int) s->height >>1) + Utils::random() * height;

    s->velocity.x = minParticleSpeed.x;

    if(minParticleSpeed.x != maxParticleSpeed.x)
        s->velocity.x += Utils::random()*(maxParticleSpeed.x - minParticleSpeed.x);

    s->velocity.y = minParticleSpeed.y;

    if(minParticleSpeed.y != maxParticleSpeed.y)
        s->velocity.y += Utils::random()*(maxParticleSpeed.y - minParticleSpeed.y);

    s->acceleration.y = gravity;
    s->angularVelocity = minRotation;

    if(minRotation != maxRotation)
        s->angularVelocity += Utils::random() * (maxRotation - minRotation);

    if(s->angularVelocity != 0)
        s->angle = Utils::random() * 360-180;

    s->drag.x = particleDrag.x;
    s->drag.y = particleDrag.y;

    _particle++;
    if(_particle >= members.size())
        _particle = 0;

    s->onEmit();
    justEmitted = true;
}

void Emitter::update()
{
    justEmitted = false;
    Group::update();
    updateEmitter();
}

void Emitter::start(bool Explode, float Delay, unsigned int Quantity)
{
    if(members.size() <= 0) {
        return;
    }

    _explode = Explode;
    if(!_explode)
        _counter = 0;
    if(!exists)
        _particle = 0;
    exists = true;
    visible = true;
    active = true;
    dead = false;
    on = true;
    _timer = 0;
    if(quantity == 0)
        quantity = Quantity;
    else if(Quantity != 0)
        Quantity = Quantity;
    if(Delay != 0)
        delay = Delay;
    if(delay < 0)
        delay = -delay;
    if(delay == 0) {
        if(Explode)
            delay = 3;
        else
            delay = 0.1f;
    }
}

void Emitter::stop(float Delay)
{
    _explode = true;
    delay = Delay;
    if(delay < 0)
        delay = -Delay;
    on = false;
}

void Emitter::at(Object* Obj)
{
    if(!Obj) return;
    x = Obj->x + Obj->origin.x;
    y = Obj->y + Obj->origin.y;
}

void Emitter::kill()
{
    Group::kill();
    on = false;
}

NAMESPACE_END
