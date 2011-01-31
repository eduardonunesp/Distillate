#ifndef DUTILS_HPP
#define DUTILS_HPP

#include <math.h>
#include <algorithm>
#include <functional>
#include <utility>
#include <string>

namespace Distillate
{
    /**
     * Clean functions, usefull to clean containers
     */
    template<typename T>
    struct dtor : public std::unary_function <T, void>
    {
        void operator()(T &ptr)
        {
            delete ptr;
        }
    };

    template<typename T1, typename T2>
    struct dtor<std::pair<T1, T2> > :
    public std::unary_function <std::pair<T1, T2>, void>
    {
        void operator()(std::pair<T1, T2> &pair)
        {
            delete pair.second;
        }
    };

    template<class Cont>
    inline dtor<typename Cont::value_type> make_dtor(Cont const&)
    {
        return dtor<typename Cont::value_type>();
    }

    template<typename Container>
    inline void delete_all(Container &c)
    {
        std::for_each(c.begin(), c.end(), make_dtor(c));
    }

    /* Functor to compare if exists */
    template <typename T>
    struct pointer_values_equal
    {
        T to_find;
        bool operator()(T other) const
        {
            return to_find == other;
        }
    };

    /* Forward */
    class DObject;
    class DPoint;
    class DQuadTree;
    class DRect;

    /**
     * Common utilities
     */
    class DUtils
    {
        protected:
            /**
             * Internal random number calculation helpers.
             */
            static int _seed;
            /**
             * Internal random number calculation helpers.
             */
            static int _originalSeed;

        public:
            /**
             * Callback function for DUtils
             */
            typedef bool (callbackFunctionUtils)(void);

            /**
             * Helps to eliminate false collisions and/or rendering glitches caused by rounding errors
             */
            static const float roundingError;

            /**
             * The global quad tree (stored here since it is used primarily by DU functions).
             * Set this to null to force it to refresh on the next collide.
             */

            static DQuadTree* quadTree;
            /**
             * This variable stores the dimensions of the root of the quad tree.
             * This is the eligible game collision space.
             */
            static DRect* quadTreeBounds;

            /**
             * Controls the granularity of the quad tree.  Default is 3 (decent performance on large and small worlds).
             */
            static const unsigned int quadTreeDivisions = 3;

            /**
             * Opens a web page in a new tab or window.
             *
             * @param   URL     The address of the web page.
             */
            static void openURL(const std::string &URL);

            static float abs(float N)
            {
                return (N>0)?N:-N;
            }

            static float floorValue(float N)
            {
                float n = int(N);
                return (N>0)?(n):((n!=N)?(n-1):(n));
            }

            static float ceilValue(float N)
            {
                return ceil(N);
            }

            /**
             * Generate a pseudo-random number.
             *
             * @param   UseGlobalSeed       Whether or not to use the stored DG.seed value to calculate it.
             *
             * @return  A pseudo-random Number object.
             */
            static float random(bool UseGlobalSeed=true);

            /**
             * Generate a pseudo-random number.
             *
             * @param   Seed        The number to use to generate a new random value.
             *
             * @return  A pseudo-random Number object.
             */
            static float randomize(int Seed)
            {
                return ((69621 * int(Seed * 0x7FFFFFFF)) % 0x7FFFFFFF) / 0x7FFFFFFF;
            }

            /**
             * Mutates a seed or other number, useful when combined with <code>randomize()</code>.
             *
             * @param   Seed        The number to mutate.
             * @param   Mutator     The value to use in the mutation.
             *
             * @return  A predictably-altered version of the Seed.
             */
            static int mutate(int Seed,int Mutator);

            /**
             * Set <code>seed</code> to a number between 0 and 1 if you want
             * <code>DG.random()</code> to generate a predictable series of numbers.
             * NOTE: reading the seed will return the original value passed in,
             * not the current mutation.
             */
            static int getSeed()
            {
                return _originalSeed;
            }

            /**
             * @private
             */
            static void setSeed(int Seed)
            {
                _seed = Seed;
                _originalSeed = _seed;
            }

            /**
             * Useful for finding out how long it takes to execute specific blocks of code.
             *
             * @return  A <code>uint</code> to be passed to <code>DU.endProfile()</code>.
             */
            static unsigned int startProfile();

            /**
             * Useful for finding out how long it takes to execute specific blocks of code.
             *
             * @param   Start   A <code>uint</code> created by <code>DU.startProfile()</code>.
             * @param   Name    Optional tag (for debug console display).  Default value is "Profiler".
             * @param   Log     Whether or not to log this elapsed time in the debug console.
             *
             * @return  A <code>uint</code> to be passed to <code>DU.endProfile()</code>.
             */
            static unsigned int endProfile(unsigned int Start, const std::string &Name="Profiler",bool Log=true);

            /**
             * Rotates a point in 2D space around another point by the given angle.
             *
             * @param   X       The X coordinate of the point you want to rotate.
             * @param   Y       The Y coordinate of the point you want to rotate.
             * @param   PivotX  The X coordinate of the point you want to rotate around.
             * @param   PivotY  The Y coordinate of the point you want to rotate around.
             * @param   Angle   Rotate the point by this many degrees.
             * @param   P       Optional <code>DPoint</code> to store the results in.
             *
             * @return  A <code>DPoint</code> containing the coordinates of the rotated point.
             */
            static DPoint* rotatePoint(float X, float Y, float PivotX, float PivotY, float  Angle, DPoint *P=NULL);

            /**
             * Calculates the angle between a point and the origin (0,0).
             *
             * @param   X       The X coordinate of the point.
             * @param   Y       The Y coordinate of the point.
             *
             * @return  The angle in degrees.
             */
            static float getAngle(float X, float Y);

            /**
             * Get the <code>String</code> name of any <code>Object</code>.
             *
             * @param   Obj     The <code>Object</code> object in question.
             * @param   Simple  Returns only the class name, not the package or packages.
             *
             * @return  The name of the <code>Class</code> as a <code>String</code> object.
             */
            static std::string getClassName(void *Obj,bool Simple=false);

            /**
             * Look up a <code>Class</code> object by its string name.
             *
             * @param   Name    The <code>String</code> name of the <code>Class</code> you are interested in.
             *
             * @return  A <code>Class</code> object.
             */
            static void* getClass(const std::string &Name);

            /**
             * A tween-like function that takes a starting velocity
             * and some other factors and returns an altered velocity.
             *
             * @param   Velocity        Any component of velocity (e.g. 20).
             * @param   Acceleration    Rate at which the velocity is changing.
             * @param   Drag            Really kind of a deceleration, this is how much the velocity changes if Acceleration is not set.
             * @param   Max             An absolute value cap for the velocity.
             *
             * @return  The altered Velocity value.
             */
            static float computeVelocity(float Velocity, float Acceleration=0, float Drag=0, float Max=10000);

            /**
             * Call this function to specify a more efficient boundary for your game world.
             * This boundary is used by <code>overlap()</code> and <code>collide()</code>, so it
             * can't hurt to have it be the right size!  Flixel will invent a size for you, but
             * it's pretty huge - 256x the size of the screen, whatever that may be.
             * Leave width and height empty if you want to just update the game world's position.
             *
             * @param   X           The X-coordinate of the left side of the game world.
             * @param   Y           The Y-coordinate of the top of the game world.
             * @param   Width       Desired width of the game world.
             * @param   Height      Desired height of the game world.
             * @param   Divisions   Pass a non-zero value to set <code>quadTreeDivisions</code>.  Default value is 3.
             */
            static void setWorldBounds(float X=0, float Y=0, float Width=0, float Height=0, unsigned int Divisions=3);

            /**
             * Call this function to see if one <code>DObject</code> overlaps another.
             * Can be called with one object and one group, or two groups, or two objects,
             * whatever floats your boat!  It will put everything into a quad tree and then
             * check for overlaps.  For maximum performance try bundling a lot of objects
             * together using a <code>DGroup</code> (even bundling groups together!)
             * NOTE: does NOT take objects' scrollfactor into account.
             *
             * @param   Object1     The first object or group you want to check.
             * @param   Object2     The second object or group you want to check.  If it is the same as the first, flixel knows to just do a comparison within that group.
             * @param   Callback    A function with two <code>DObject</code> parameters - e.g. <code>myOverlapFunction(Object1:DObject,Object2:DObject);</code>  If no function is provided, <code>DQuadTree</code> will call <code>kill()</code> on both objects.
             */
            static bool overlap(DObject *Object1,DObject *Object2,void* callback=NULL);

            /**
             * Call this function to see if one <code>DObject</code> collides with another.
             * Can be called with one object and one group, or two groups, or two objects,
             * whatever floats your boat!  It will put everything into a quad tree and then
             * check for collisions.  For maximum performance try bundling a lot of objects
             * together using a <code>DGroup</code> (even bundling groups together!)
             * NOTE: does NOT take objects' scrollfactor into account.
             *
             * @param   Object1     The first object or group you want to check.
             * @param   Object2     The second object or group you want to check.  If it is the same as the first, flixel knows to just do a comparison within that group.
             */
            static bool collide(DObject *Object1, DObject *Object2);

            /**
             * This quad tree callback function can be used externally as well.
             * Takes two objects and separates them along their X axis (if possible/reasonable).
             *
             * @param	Object1		The first object or group you want to check.
             * @param	Object2		The second object or group you want to check.
             */
            static bool solveXCollision(DObject* Object1, DObject* Object2);

            /**
             * This quad tree callback function can be used externally as well.
             * Takes two objects and separates them along their Y axis (if possible/reasonable).
             *
             * @param	Object1		The first object or group you want to check.
             * @param	Object2		The second object or group you want to check.
             */
            static bool solveYCollision(DObject* Object1, DObject* Object2);
    };
}
#endif // DUTILS_HPP
