#ifndef __DPOINT_HPP__
#define __DPOINT_HPP__

namespace Distillate {
    /**
     * Stores a 2D floating point coordinate.
     */
    class DPoint {
    public:
         /**
          * @default 0
          */
         float x;

         /**
          * @default 0
          */
         float y;

         /**
          * Instantiate a new point object
          *
          * @param X The X-coordinate of the point in space.
          * @param Y The Y-coordinate of the point in space.
          *
          */
         DPoint(float X=0, float Y=0) : x(X), y(Y) {};
         ~DPoint() {}
    };
}
#endif /* __DPOINT_HPP__ */
