#ifndef __DPARTICLE_HPP__
#define __DPARTICLE_HPP__

#include "DSprite.hpp"

namespace Distillate {
    class DParticle : public DSprite {
    protected:
         float _bounce;

    public:

         DParticle(float Bounce) : DSprite() {
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
                   DSprite::hitBottom(Contact, Velocity);
              }

              velocity.x *= _bounce;
         }
    };
}

#endif
