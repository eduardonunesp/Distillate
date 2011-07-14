#include "DEmitter.hpp"
#include "DPoint.hpp"
#include "DGlobals.hpp"
#include "DUtils.hpp"
#include "DSprite.hpp"
#include "DParticle.hpp"

namespace Distillate
{

DEmitter::DEmitter(float X, float Y):
DGroup(),
minParticleSpeed(new DPoint(-100,-100)),
maxParticleSpeed(new DPoint(100,100)),
particleDrag(new DPoint()),
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

DEmitter::~DEmitter()
{
}

DEmitter * DEmitter::createSprites(const std::string &GraphicFile, unsigned int Quantity, unsigned int BakedRotations, bool Multiple, float Collide, float Bounce)
{
    unsigned int r;
    DSprite *s = NULL;
    unsigned int tf = 1;
    float sw;
    float sh;

    if(Multiple)
    {
        s = new DSprite();
        s->loadGraphic(GraphicFile, true);
        tf = s->frames;
    }

    unsigned int i = 0;
    while(i < Quantity)
    {
        if((Collide > 0) && (Bounce > 0))
            s = new DParticle(Bounce);
        else
            s = new DSprite();

        if(Multiple)
        {
            r = DUtils::random() * tf;
            if(BakedRotations > 0)
                s->loadGraphic(GraphicFile, true); //TODO: Must rotate !
            else
            {
                s->loadGraphic(GraphicFile, true);
                s->frame(-r); //WARN
            }
        }
        else
        {
            if(BakedRotations > 0)
                s->loadGraphic(GraphicFile, true);
            else 
                s->loadGraphic(GraphicFile, true);
        }

        if(Collide)
        {
            sw = s->width;
            sh = s->height;
            s->width *= Collide;
            s->height *= Collide;
            s->offset->x = (sw-s->width)/2;
            s->offset->y = (sh-s->height)/2;
            s->solid(true);
        }
        else
            s->solid(false);
        s->exists = false;
        s->scrollFactor = scrollFactor;
        add(s);
        i++;
    }

    return this;
}

void DEmitter::updateEmitter()
{
    if(_explode)
    {
        _timer += DGlobals::elapsed;
        if((delay > 0) && (_timer > delay))
        {
            kill();
            return;
        }

        if(on)
        {
            on = false;
            unsigned int i = _particle;
            unsigned int l = members.size();
            if(quantity > 0)
                l = quantity;
            l += _particle;
            while(i < l)
            {
                emitParticle();
                i++;
            }
        }

        return;
    }

    if(!on)
        return;
        
    _timer += DGlobals::elapsed;
    while((_timer > delay) && ((quantity <= 0) || (_counter < quantity)))
    {
        _timer -= delay;
        emitParticle();
    }
}

void DEmitter::emitParticle()
{
    _counter++;
    DSprite *s = static_cast<DSprite*>(members[_particle]);
    if(!s) return;
    
    s->visible = true;
    s->exists  = true;
    s->active  = true;
    s->x = x - ((int) s->width>>1) + DUtils::random() * width;
    s->y = y - ((int) s->height>>1) + DUtils::random() * height;
    s->velocity->x = minParticleSpeed->x;

    if(minParticleSpeed->x != maxParticleSpeed->x) 
        s->velocity->x += DUtils::random()*(maxParticleSpeed->x - minParticleSpeed->x);
    if(minParticleSpeed->y != maxParticleSpeed->y) 
        s->velocity->y += DUtils::random()*(maxParticleSpeed->y - minParticleSpeed->y);

    s->acceleration->y = gravity;
    s->angularVelocity = minRotation;

    if(minRotation != maxRotation)
        s->angularVelocity += DUtils::random() * (maxRotation - minRotation);

    if(s->angularVelocity != 0) 
        s->angle = DUtils::random() * 360-180;        

    s->drag->x = particleDrag->x;
    s->drag->y = particleDrag->y;

    _particle++;
    if(_particle >= members.size())
        _particle = 0;

    s->onEmit();
    justEmitted = true;
}

void DEmitter::update()
{
    justEmitted = false;
    DGroup::update();
    updateEmitter();
}

void DEmitter::start(bool Explode, float Delay, unsigned int Quantity)
{
    if(members.size() <= 0)
    {
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
    if(delay == 0)
    {
        if(Explode)
            delay = 3;
        else
            delay = 0.1f;
    }
}

void DEmitter::stop(float Delay)
{
    _explode = true;
    delay = Delay;
    if(delay < 0)
        delay = -Delay;
    on = false;
}

void DEmitter::at(DObject* Object)
{
    if(!Object) return;
    x = Object->x + Object->origin->x;
    y = Object->y + Object->origin->y;
}

void DEmitter::kill()
{
    DGroup::kill();
    on = false;
}

}
