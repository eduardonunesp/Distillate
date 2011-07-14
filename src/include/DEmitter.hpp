#ifndef __DEMITTER_HPP__
#define __DEMITTER_HPP__

#include <string>
#include "DGroup.hpp"

namespace Distillate
{

class DPoint;

/**
 * <code>DEmitter</code> is a lightweight particle emitter.
 * It can be used for one-time explosions or for
 * continuous fx like rain and fire.  <code>DEmitter</code>
 * is not optimized or anything; all it does is launch
 * <code>DSprite</code> objects out at set intervals
 * by setting their positions and velocities accordingly.
 * It is easy to use and relatively efficient, since it
 * automatically redelays its sprites and/or kills
 * them once they've been launched.
 */
class DEmitter : public DGroup
{
public:
    DEmitter();
    virtual ~DEmitter();

    /** 
     * The minimum possible velocity of a particle.
     * The default value is (-100,-100).
     */
    DPoint *minParticleSpeed;

    /** 
     * The maximum possible velocity of a particle.
     * The default value is (100,100).
     */
    DPoint *maxParticleSpeed;

    /** 
     * The X and Y drag component of particles launched from the emitter.
     */
    DPoint *particleDrag;

    /** 
     * The minimum possible angular velocity of a particle.  The default value is -360.
     * NOTE: rotating particles are more expensive to draw than non-rotating ones!
     */
    float minRotation;

    /** 
     * The maximum possible angular velocity of a particle.  The default value is 360.
     * NOTE: rotating particles are more expensive to draw than non-rotating ones!
     */
    float maxRotation;

    /** 
     * Sets the <code>acceleration.y</code> member of each particle to this value on launch.
     */
    float gravity;

    /** 
     * Determines whether the emitter is currently emitting particles.
     */
    bool on;

    /**
     * This variable has different effects depending on what kind of emission it is.
     * During an explosion, delay controls the lifespan of the particles.
     * During normal emission, delay controls the time between particle launches.
     * NOTE: In older builds, polarity (negative numbers) was used to define emitter behavior.
     * THIS IS NO LONGER THE CASE!  DEmitter.start() controls that now!
     */
    float delay;

    /**
     * The number of particles to launch at a time.
     */

    unsigned int quantity;

protected:
    /**
     * Checks whether you already fired a particle this frame.
     */
    bool justEmitted;

    /**
     * The style of particle emission (all at once, or one at a time).
     */
    bool _explode;

    /**
     * Internal helper for deciding when to launch particles or kill them.
     */
    float _timer;

    /**
     * Internal marker for where we are in <code>_sprites</code>.
     */
    unsigned int _particle;

    /**
     * Internal counter for figuring out how many particles to launch.
     */
    unsigned int _counter;

public:
    /**
     * Creates a new <code>DEmitter</code> object at a specific position.
     * Does not automatically generate or attach particles!
     * 
     * @param   X           The X position of the emitter.
     * @param   Y           The Y position of the emitter.
     */
    DEmitter(float X, float Y);

    /**
     * This function generates a new array of sprites to attach to the emitter.
     * 
     * @param   Graphics        If you opted to not pre-configure an array of DSprite objects, you can simply pass in a particle image or sprite sheet.
     * @param   Quantity        The number of particles to generate when using the "create from image" option.
     * @param   BakedRotations  How many frames of baked rotation to use (boosts performance).  Set to zero to not use baked rotations.
     * @param   Multiple        Whether the image in the Graphics param is a single particle or a bunch of particles (if it's a bunch, they need to be square!).
     * @param   Collide         Whether the particles should be flagged as not 'dead' (non-colliding particles are higher performance).  0 means no collisions, 0-1 controls scale of particle's bounding box.
     * @param   Bounce          Whether the particles should bounce after colliding with things.  0 means no bounce, 1 means full reflection.
     * 
     * @return  This DEmitter instance (nice for chaining stuff together, if you're into that).
     */
    DEmitter * createSprites(const std::string &GraphicFile, unsigned int Quantity=50, unsigned int BakedRotations=16, bool Multiple=true, float Collide=0, float Bounce=0);

    /**
     * A more compact way of setting the width and height of the emitter.
     * 
     * @param   Width   The desired width of the emitter (particles are spawned randomly within these dimensions).
     * @param   Height  The desired height of the emitter.
     */
    void setSize(unsigned int Width, unsigned int Height)
    {
        width = Width;
        height = Height;
    }

    /**
     * A more compact way of setting the X velocity range of the emitter.
     * 
     * @param   Min     The minimum value for this range.
     * @param   Max     The maximum value for this range.
     */
    void setXSpeed(float Min=0, float Max=0)
    {
        minParticleSpeed->x = Min;
        maxParticleSpeed->x = Max;
    }

    /**
     * A more compact way of setting the Y velocity range of the emitter.
     * 
     * @param   Min     The minimum value for this range.
     * @param   Max     The maximum value for this range.
     */
    void setYSpeed(float Min=0, float Max=0)
    {
        minParticleSpeed->y = Min;
        minParticleSpeed->y = Max;
    }

    /**
     * A more compact way of setting the angular velocity constraints of the emitter.
     * 
     * @param   Min     The minimum value for this range.
     * @param   Max     The maximum value for this range.
     */
    void setRotation(float Min=0, float Max=0)
    {
        minRotation = Min;
        maxRotation = Max;
    }

protected:
    /**
     * Internal function that actually performs the emitter update (called by update()).
     */
    void updateEmitter();

    /**
     * Internal function that actually goes through and updates all the group members.
     * Overridden here to remove the position update code normally used by a DGroup.
     */
    void updateMembers();

public:
    /**
     * Called automatically by the game loop, decides when to launch particles and when to "die".
     */
    virtual void update();

    /**
     * Call this function to start emitting particles.
     * 
     * @param   Explode     Whether the particles should all burst out at once.
     * @param   Delay       You can set the delay (or lifespan) here if you want.
     * @param   Quantity    How many particles to launch.  Default value is 0, or "all the particles".
     */
    void start(bool Explode=true, float Delay=0, unsigned int Quantity=0);

    /**
     * This function can be used both internally and externally to emit the next particle.
     */
    void emitParticle();

    /**
     * Call this function to stop the emitter without killing it.
     * 
     * @param   Delay   How long to wait before killing all the particles.  Set to 'zero' to never kill them.
     */
    void stop(float Delay=3);

    /**
     * Change the emitter's position to the origin of a <code>DObject</code>.
     * 
     * @param   Object      The <code>DObject</code> that needs to spew particles.
     */
    void at(DObject* DObject);

    /**
     * Call this function to turn off all the particles and the emitter.
     */
    void kill();
};

}
#endif // DEMITTER_HPP
