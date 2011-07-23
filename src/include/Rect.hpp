#ifndef __RECT_HPP__
#define __RECT_HPP__

#include "Defs.hpp"
#include "Point.hpp"

NAMESPACE_BEGIN

/**
 * Stores a rectangle
 */
class Rect : public Point {
public:
    /**
     * @default 0
     */
    float width;

    /**
     * @default 0
     */
    float height;

    /**
     * Instantiate a new rectangle.
     *
     * @param   X       The X-coordinate of the point in space.
     * @param   Y       The Y-coordinate of the point in space.
     * @param   Width   Desired width of the rectangle.
     * @param   Height  Desired height of the rectangle.
     */
    Rect(float X=0, float Y=0, float Width=0, float Height=0)
        : Point(X,Y), width(Width), height(Height) {}
    ~Rect() {}

    /**
     * The X coordinate of the left side of the rectangle.  Read-only.
     */
    unsigned int getLeft() {
        return x;
    }

    /**
     * The X coordinate of the right side of the rectangle.  Read-only.
     */
    unsigned int getRight() {
        return x + width;
    }

    /**
     * The Y coordinate of the top of the rectangle.  Read-only.
     */
    unsigned int getTop() {
        return y;
    }

    /**
     * The Y coordinate of the bottom of the rectangle.  Read-only.
     */
    unsigned int getBottom() {
        return y + height;
    }
};

NAMESPACE_END
#endif /* __RECT_HPP */
