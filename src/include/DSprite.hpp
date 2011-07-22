#ifndef __DSPRITE_HPP__
#define __DSPRITE_HPP__

#include <string>
#include <vector>

#if defined(GL_RENDER)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#elif defined(SDL_RENDER)
#include <SDL/SDL.h>
#endif

#include "DObject.hpp"
#include "DUtils.hpp"

namespace Distillate {
    /* Forward */
    class DPoint;
    class DAnim;
    class DTextureResource;

    /**
     * The main "game object" class, handles basic physics and animation.
     */
    class DSprite : public DObject {
         friend class DState;
    public:
        typedef std::vector<int> AnimationFrames;

         /**
          * Callback function for DSprite
          */
         typedef bool (callbackFunctionSprite)(DAnim*, unsigned int);

         /**
          * Useful for controlling flipped animations and checking player orientation.
          */
         static const unsigned int  LEFT = 0;

         /**
          * Useful for controlling flipped animations and checking player orientation.
          */
         static const unsigned int RIGHT = 1;

         /**
          * Useful for checking player orientation.
          */
         static const unsigned int    UP = 2;

         /**
          * Useful for checking player orientation.
          */
         static const unsigned int  DOWN = 3;

         /**
          * If you changed the size of your sprite object to shrink the bounding box,
          * you might need to offset the new bounding box from the top-left corner of the sprite.
          */
         DPoint offset;

         /**
          * Whether the current animation has finished its first (or only) loop.
          */
         bool finished;

         /**
          * The width of the actual graphic or image being displayed (not necessarily the game object/bounding box).
          * NOTE: Edit at your own risk!!  This is intended to be read-only.
          */
         unsigned int frameWidth;

         /**
          * The height of the actual graphic or image being displayed (not necessarily the game object/bounding box).
          * NOTE: Edit at your own risk!!  This is intended to be read-only.
          */
         unsigned int frameHeight;

         /**
          * The total number of frames in this image (assumes each row is full)
          */
         unsigned int frames;
         
    protected:
         /* Animation helpers */
         std::vector<DAnim*> _animations;
         unsigned int _flipped;
         DAnim* _curAnim;
         unsigned int _curFrame;
         unsigned int _caf;
         float _frameTimer;
         callbackFunctionSprite *_callback;
         unsigned int _facing;
         int _bakedRotation;
         float _alpha;
         unsigned int _color;
         bool _boundsVisible;

         /* Surface stuff */
         DTextureResource *_pixels;

#if defined(SDL_RENDER)
        SDL_Rect _rendering_rect;
#endif

    public:

         /**
          * Creates a white 8x8 square <code>FlxSprite</code> at the specified position.
          * Optionally can load a simple, one-frame graphic instead.
          *
          * @param   X               The initial X position of the sprite.
          * @param   Y               The initial Y position of the sprite.
          * @param   SimpleGraphic   The graphic you want to display (OPTIONAL - for simple stuff only, do NOT use for animated images!).
          */
         DSprite(float X = 0, float Y = 0, const std::string &SimpleGraphic = "");
         virtual ~DSprite();

         /**
          * Load an image from an embedded graphic file.
          *
          * @param   Graphic     The image you want to use.
          * @param   Animated    Whether the Graphic parameter is a single sprite or a row of sprites.
          * @param   Reverse     Whether you need this class to generate horizontally flipped versions of the animation frames.
          * @param   Width       OPTIONAL - Specify the width of your sprite (helps FlxSprite figure out what to do with non-square sprites or sprite sheets).
          * @param   Height      OPTIONAL - Specify the height of your sprite (helps FlxSprite figure out what to do with non-square sprites or sprite sheets).
          * @param   Unique      Whether the graphic should be a unique instance in the graphics cache.
          *
          * @return  This FlxSprite instance (nice for chaining stuff together, if you're into that).
          */
         DSprite *loadGraphic(const std::string &Graphic, bool Animated = false, bool Reverse = false, unsigned int Width = 0, unsigned int Height = 0, bool Unique = false);

         /**
          * This function creates a flat colored square image dynamically.
          *
          * @param   Width       The width of the sprite you want to generate.
          * @param   Height      The height of the sprite you want to generate.
          * @param   Color       Specifies the color of the generated block.
          * @param   Unique      Whether the graphic should be a unique instance in the graphics cache.
          * @param   Key         Optional parameter - specify a string key to identify this graphic in the cache.  Trumps Unique flag.
          *
          * @return  This FlxSprite instance (nice for chaining stuff together, if you're into that).
          */
         DSprite *createGraphic(const std::string &Key, unsigned int Width, unsigned int Height, unsigned int Color = 0xffffffff);

    protected:
         /**
          * Resets some important variables for sprite optimization and rendering.
          */
         void resetHelpers();

    public:
         /**
          * Main game loop update function.  Override this to create your own sprite logic!
          * Just don't forget to call super.update() or any of the helper functions.
          */
         virtual void update();

         /**
          * Called by game loop, updates then blits or renders current frame of animation to the screen
          */
         virtual void render();

         /**
          * Checks to see if a point in 2D space overlaps this FlxCore object.
          *
          * @param   X           The X coordinate of the point.
          * @param   Y           The Y coordinate of the point.
          * @param   PerPixel    Whether or not to use per pixel collision checking.
          *
          * @return  Whether or not the point overlaps this object.
          */
         bool overlapsPoint(unsigned int X, unsigned int Y, bool PerPixel = false);

         /**
          * Triggered whenever this sprite is launched by a <code>FlxEmitter</code>.
          */
         virtual void onEmit() {};

         /**
          * Adds a new animation to the sprite.
          *
          * @param   Name        What this animation should be called (e.g. "run").
          * @param   Frames      An array of numbers indicating what frames to play in what order (e.g. 1, 2, 3).
          * @param   FrameRate   The speed in frames per second that the animation should play at (e.g. 40 fps).
          * @param   Looped      Whether or not the animation is looped or just plays once.
          */
         void addAnimation(const std::string &Name, std::vector<int> &Frames, float FrameRate, bool Looped = true);

         /**
          * Plays an existing animation (e.g. "run").
          * If you call an animation that is already playing it will be ignored.
          *
          * @param   AnimName    The string name of the animation you want to play.
          * @param   Force       Whether to force the animation to restart.
          */
         void play(const std::string &AnimName, bool Boolean = false);

         /**
          * Call this function to figure out the on-screen position of the object.
          *
          * @param   P   Takes a <code>Point</code> object and assigns the post-scrolled X and Y values of this object to it.
          *
          * @return  The <code>Point</code> you passed in, or a new <code>Point</code> if you didn't pass one, containing the screen X and Y position of this object.
          */
         virtual DPoint * getScreenXY(DPoint &Point);

         /**
          * Internal function to update the current animation frame.
          */
         void calcFrame();

         /**
          * Internal function for updating the sprite's animation.
          * Useful for cases when you need to update this but are buried down in too many supers.
          * This function is called automatically by <code>FlxSprite.update()</code>.
          */
         void  updateAnimation();

         void frame(unsigned int Frame) {
              _curAnim = NULL;
              _caf = Frame;
              calcFrame();
         }
    };
}
#endif /* __DSPRITE_HPP__ */
