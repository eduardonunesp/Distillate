#ifndef DRECT_HPP
#define DRECT_HPP

#include "DPoint.hpp"

namespace Distillate
{

/**
* Stores a rectangle
*/
class DRect : public DPoint
{
public:
    unsigned int width;
    unsigned int height;

    /**
     * Instantiate a new rectangle.
     *
     * @param   X       The X-coordinate of the point in space.
     * @param   Y       The Y-coordinate of the point in space.
     * @param   Width   Desired width of the rectangle.
     * @param   Height  Desired height of the rectangle.
     */
    DRect(int X = 0, unsigned int Y = 0, unsigned int Width = 0, int Height = 0)
        : DPoint(X,Y), width(Width), height(Height) {}
    virtual ~DRect() {}

    /**
    * The X coordinate of the left side of the rectangle.  Read-only.
    */
    unsigned int getLeft()
    {
        return x;
    }

    /**
    * The X coordinate of the right side of the rectangle.  Read-only.
    */
    unsigned int getRight()
    {
        return x + width;
    }

    /**
    * The Y coordinate of the top of the rectangle.  Read-only.
    */
    unsigned int getTop()
    {
        return y;
    }

    /**
    * The Y coordinate of the bottom of the rectangle.  Read-only.
    */
    unsigned int getBottom()
    {
        return y + height;
    }
};

}
#endif // DRECT_HPP
