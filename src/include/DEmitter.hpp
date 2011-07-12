#ifndef __DEMITTER_HPP__
#define __DEMITTER_HPP__

#include "DGroup.hpp"

namespace Distillate
{

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
};

}
#endif // DEMITTER_HPP
