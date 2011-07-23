#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <vector>
#include "Defs.hpp"
#include "Rect.hpp"

NAMESPACE_BEGIN

/* Forwards */
class Point;

/**
 * This is the base class for most of the display objects (<code>Sprite</code>, <code>Text</code>, etc).
 * It includes some basic attributes about game objects, including retro-style flickering,
 * basic state information, sizes, scrolling, and basic physics & motion.
 */
class Object : public Rect
{
    /* Internal */
    friend class QuadTree;
    friend class Utils;
    friend class Group;

public:
    /**
     * Kind of a global on/off switch for any objects 
     * descended from <code>Object</code>.
     */
    bool exists;

    /**
     * If an object is not alive, the game loop will not
     * automatically call <code>update()</code> on it.
     */
    bool active;

    /**
     * If an object is not visible, the game loop will not automatically call <code>render()</code> on it.
     */
    bool visible;

protected:
    /**
     * If an object is dead, the functions that automate collisions 
     * will skip it (see <code>Globals.overlapArrays()</code> and <code>
     * Globals.collideArrays()</code>).
     */
    bool _solid;

    /**
     * If an object is 'fixed' in space, it will not budge 
     * when it collides with a not-fixed object.
     * Fixed objects also shortcut out of updateMotion by default.
     */
    bool _fixed;

public:
    /**
     * The basic speed of this object.
     */
    Point velocity;

    /**
     * How fast the speed of this object is changing.
     * Useful for smooth movement and gravity.
     */
    Point acceleration;

    /**
     * This isn't drag exactly, more like deceleration that is only applied
     * when acceleration is not affecting the sprite.
     */
    Point drag;

    /**
     * If you are using <code>acceleration</code>, you can use 
     * <code>maxVelocity</code> with it
     * to cap the speed automatically (very useful!).
     */
    Point maxVelocity;

    /**
     * Set the angle of a sprite to rotate it.
     * WARNING: rotating sprites decreases rendering
     * performance for this sprite by a factor of 10x!
     */
    float angle;

    /**
     * This is how fast you want this sprite to spin.
     */
    float angularVelocity;

    /**
     * How fast the spin speed should change.
     */
    float angularAcceleration;

    /**
     * Like <code>drag</code> but for spinning.
     */
    float angularDrag;

    /**
     * Use in conjunction with <code>angularAcceleration</code> 
     * for fluid spin speed control.
     */
    float maxAngular;

    /**
     * WARNING: The origin of the sprite will default to its center.
     * If you change this, the visuals and the collisions will likely be
     * pretty out-of-sync if you do any rotation.
     */
    Point origin;

    /**
     * If you want to do Asteroids style stuff, check out thrust,
     * instead of directly accessing the object's velocity or acceleration.
     */
    float thrust;

    /**
     * Used to cap <code>thrust</code>, helpful and easy!
     */
    float maxThrust;

    /**
     * A point that can store numbers from 0 to 1 (for X and Y independently)
     * that governs how much this object is affected by the camera subsystem.
     * 0 means it never moves, like a HUD element or far background graphic.
     * 1 means it scrolls along a the same speed as the foreground layer.
     * scrollFactor is initialized as (1,1) by default.
     */
    Point scrollFactor;

protected:
    /**
     * Internal helper used for retro-style flickering.
     */
    bool _flicker;

    /**
     * Internal helper used for retro-style flickering.
     */
    float _flickerTimer;

public:
    /**
     * Handy for storing health percentage or armor points or whatever.
     */
    float health;

    /**
     * Handy for tracking gameplay or animations.
     */
    bool dead;

protected:
    /**
     * This is just a pre-allocated x-y point container to be 
     * used however you like
     */
    Point _point;

    /**
     * This is just a pre-allocated rectangle container to be 
     * used however you like
     */
    Rect _rect;

    /**
     * DEPRECATED
     * This is a pre-allocated Flash Point object, which is
     * useful for certain graphics API calls
     */
    Point _flashPoint;

public:
    /**
     * Set this to false if you want to skip the automatic 
     * motion/movement stuff (see <code>updateMotion()</code>).
     * Object and Sprite default to true.
     * Text, TileBlock, TileMap and Sound default to false.
     */
    bool moves;

    /**
     * These store a couple of useful numbers for speeding up collision resolution.
     */
    Rect colHullX;

    /**
     * These store a couple of useful numbers for speeding up collision resolution.
     */
    Rect colHullY;

    /**
     * These store a couple of useful numbers for speeding up collision resolution.
     */
    Point colVector;

    /**
     * An array of <code>Point</code> objects.  
     * By default contains a single offset (0,0).
     */
    std::vector<Point*> colOffsets;

protected:
    /**
     * Dedicated internal flag for whether or not this class is a Group.
     * Internal use only !!!
     */
    bool _group;

public:
    /**
     * Flag that indicates whether or not you just hit the floor.
     * Primarily useful for platformers, this flag is reset during 
     * the <code>updateMotion()</code>
     */
    bool onFloor;

    /**
     * Flag for direction collision resolution.
     */
    bool collideLeft;

    /**
     * Flag for direction collision resolution.
     */
    bool collideRight;

    /**
     * Flag for direction collision resolution.
     */
    bool collideTop;

    /**
     * Flag for direction collision resolution.
     */
    bool collideBottom;

    /**
     * Flag for whether the bounding box visuals need to be refreshed.
     * Internal use only !!!
     */
    static bool _refreshBounds;

    /**
     * Creates a new <code>Object</code>.
     *
     * @param   X       The X-coordinate of the point in space.
     * @param   Y       The Y-coordinate of the point in space.
     * @param   Width   Desired width of the rectangle.
     * @param   Height  Desired height of the rectangle.
     */
    Object(float X=0, float Y=0, float Width=0, float Height=0);
    virtual ~Object();

    /**
     * Called by <code>Group</code>, commonly when game states are changed.
     */
    virtual void destroy() {};

    /**
     * Set <code>solid</code> to true if you want to collide this object.
     */
    void solid(bool Solid)
    {
        _solid = Solid;
    }

    /**
     * @private
     */
    bool solid()
    {
        return _solid;
    }

    /**
     * Set <code>fixed</code> to true if you want the object 
     * to stay in place during collisions.
     * Useful for levels and other environmental objects.
     */
    void fixed(bool Fixed)
    {
        _fixed = Fixed;
    }

    /**
     * @private
     */
    bool fixed()
    {
        return _fixed;
    }

    /**
     * Called by <code>Object.updateMotion()</code> and some constructors to
     * rebuild the basic collision data for this object.
     */
    void refreshHulls();

    /**
     * Internal function for updating the position and speed of this object.
     * Useful for cases when you need to update this but are buried down in too many supers.
     */
    void updateMotion();

    /**
     * Just updates the retro-style flickering.
     * Considered update logic rather than rendering because it toggles visibility.
     */
    void updateFlickering();

    /**
     * Called by the main game loop, handles motion/physics and game logic
     */
    virtual void update();

    /**
     * Override this function to draw graphics (see <code>Sprite</code>).
     */
    virtual void render() {};

    /**
     * Checks to see if some <code>Object</code> object overlaps this <code>Object</code> object.
     *
     * @param   Object  The object being tested.
     *
     * @return  Whether or not the two objects overlap.
     */
    bool overlaps(Object *Object);

    /**
     * Checks to see if a point in 2D space overlaps this <code>Object</code> object.
     *
     * @param   X           The X coordinate of the point.
     * @param   Y           The Y coordinate of the point.
     * @param   PerPixel    Whether or not to use per pixel collision 
     * checking (only available in <code>Sprite</code> subclass).
     *
     * @return  Whether or not the point overlaps this object.
     */
    bool overlapsPoint(float X,float Y,bool PerPixel=false);

    /**
     * If you don't want to call <code>Utils.collide()</code> you can use this instead.
     * Just calls <code>Utils.collide(this,Object);</code>.  Will collide against itself
     * if Object==null.
     *
     * @param   Object      The <Object> you want to collide with.
     */
    bool collide(Object *Obj=NULL);

    /**
     * <code>Utils.collide()</code> (and thus <code>Object.collide()</code>) call
     * this function each time two objects are compared to see if they collide.
     * It doesn't necessarily mean these objects WILL collide, however.
     *
     * @param   Object  The <code>Object</code> you're about to run into.
     */
    virtual void preCollide(Object *Obj) {};

    /**
     * Called when this object's left side collides with 
     * another <code>Object</code>'s right.
     *
     * @param   Contact     The <code>Object</code> you just ran into.
     * @param   Velocity    The suggested new velocity for this object.
     */
    void hitLeft(Object *Contact,float Velocity);

    /**
     * Called when this object's right side collides with 
     * another <code>Object</code>'s left.
     *
     * @param   Contact     The <code>Object</code> you just ran into.
     * @param   Velocity    The suggested new velocity for this object.
     */
    void hitRight(Object *Contact,float Velocity);

    /**
     * Called when this object's top collides with the 
     * bottom of another <code>Object</code>.
     *
     * @param   Contact     The <code>Object</code> you just ran into.
     * @param   Velocity    The suggested new velocity for this object.
     */
    void hitTop(Object *Contact,float Velocity);

    /**
     * Called when this object's bottom edge collides with
     * the top of another <code>Object</code>.
     *
     * @param   Contact     The <code>Object</code> you just ran into.
     * @param   Velocity    The suggested new velocity for this object.
     */
    void hitBottom(Object *Contact,float Velocity);

    /**
     * Call this function to "damage" (or give health bonus) to this sprite.
     *
     * @param   Damage      How much health to take
     * away (use a negative number to give a health bonus).
     */
    virtual void hurt(int Damage);

    /**
     * Call this function to "kill" a sprite so that it no longer 'exists'.
     */
    void kill();

    /**
     * Tells this object to flicker, retro-style.
     *
     * @param   Duration    How many seconds to flicker for.
     */
    void flicker(int Duration=1);

    /**
     * Check to see if the object is still flickering.
     *
     * @return  Whether the object is flickering or not.
     */
    bool flickering();

    /**
     * Call this function to figure out the on-screen position of the object.
     *
     * @param   P   Takes a <code>Point</code> object and assigns the
     * post-scrolled X and Y values of this object to it.
     *
     * @return  The <code>Point</code> you passed in, or a new <code>Point</code> 
     * if you didn't pass one, containing the screen X and Y position of this object.
     */
    Point* getScreenXY(Point &P);

    /**
     * Check and see if this object is currently on screen.
     *
     * @return  Whether the object is on screen or not.
     */
    bool onScreen();

    /**
     * Handy function for reviving game objects.
     * Resets their existence flags and position, including LAST position.
     *
     * @param   X   The new X position of this object.
     * @param   Y   The new Y position of this object.
     */
    void reset(float X,float Y);

    /**
     * Returns the appropriate color for the bounding box depending on object state.
     */
    unsigned int getBoundingColor();
};

NAMESPACE_END
#endif /* __OBJECT_HPP__ */
