#ifndef DPOINT_HPP
#define DPOINT_HPP

#include <string>
#include <sstream>

namespace Distillate 
{
    /**
     * Stores a 2D floating point coordinate.
     */
    class DPoint
    {
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
        DPoint(float X=0, float Y=0):x(X),y(Y) {}
        virtual ~DPoint() {}

        /**
         * Convert object to std::string, Usefulo for debugging, save games, etc.
         */
        virtual std::string toString() 
        {
            std::stringstream s;
            s << "DPoint[X=" << x << ",Y=" << y << "]";
            return s.str();
        }
    };
}
#endif // DPOINT_HPP