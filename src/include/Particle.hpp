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

#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "Defs.hpp"
#include "Sprite.hpp"

NAMESPACE_BEGIN

class Particle : public Sprite {
protected:
    float _bounce;

public:
    Particle(float Bounce) : Sprite() {
        _bounce = Bounce;
    }

    void hitSide(Object* Contact, float Velocity) {
        if (!Contact) return;
        velocity.x = -velocity.x * _bounce;

        if(angularVelocity != 0)
            angularVelocity = -angularVelocity * _bounce;
    }

    void hitBottom(Object* Contact, float Velocity) {
        if (!Contact) return;

        onFloor = true;
        if(((velocity.y > 0) ? velocity.y : -velocity.y) > _bounce*100) {
            velocity.y = -velocity.y * _bounce;
            if(angularVelocity != 0)
                angularVelocity *= -_bounce;
        } else {
            angularVelocity = 0;
            Sprite::hitBottom(Contact, Velocity);
        }

        velocity.x *= _bounce;
    }
};

NAMESPACE_END
#endif /* __DPARTICLE_HPP__ */
