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

    void hitSide(DObject* Contact, float Velocity) {
        if (!Contact) return;
        velocity.x = -velocity.x * _bounce;

        if(angularVelocity != 0)
            angularVelocity = -angularVelocity * _bounce;
    }

    void hitBottom(DObject* Contact, float Velocity) {
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
