#ifndef DSPRITE_HPP
#define DSPRITE_HPP

#include <string>
#include <vector>
#include "DObject.hpp"
#include "DUtils.hpp"
#include "SDL/BitmapData.hpp"
#include "SDL/Sprite.hpp"
#include "SDL/Graphics.hpp"

namespace Distillate
{
    /* Forward */
    class DPoint;
    class DAnim;

    /**
     * The main "game object" class, handles basic physics and animation.
     */
    class DSprite : public DObject
    {
    public:
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
        DPoint* offset;

        /**
         * Change the size of your sprite's graphic.
         * NOTE: Scale doesn't currently affect collisions automatically,
         * you will need to adjust the width, height and offset manually.
         * WARNING: scaling sprites decreases rendering performance for this sprite by a factor of 10x!
         */
        DPoint* scale;

        /**
         * Blending modes, just like Photoshop
         * Eg. "multiply", "screen", etc;
         * @default NULL
         */
        std::string blend;

        /**
         * Blending modes, just like Photoshop!
         * E.g. "multiply", "screen", etc.
         * @default null
         */
        bool antialiasing;

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

        // Animation helpers
        std::vector<DAnim*> _animations;
        unsigned int _flipped;
        DAnim* _curAnim;
        unsigned int _curFrame;
        unsigned int _caf;
        int _frameTimer;
        callbackFunctionSprite *_callback;
        unsigned int _facing;
        int _bakedRotation;

        // Various rendering helpers
        DPoint* _flashPointZero;
        SDL::BitmapData* _pixels;
        SDL::BitmapData* _framePixels;
        int _alpha;
        unsigned int _color;
        SDL::BitmapData* _bbb;
        bool _boundsVisible;

        static SDL::Sprite* _gfxSprite;
        static SDL::Graphics* _gfx;
        /*
_flashRect:Rectangle;
_flashRect2:Rectangle;
_flashPointZero:Point;
_pixels:BitmapData;
_framePixels:BitmapData;
_ct:ColorTransform;
_mtx:Matrix;
_bbb:BitmapData;
*/

    public:

        /**
         * Creates a white 8x8 square <code>FlxSprite</code> at the specified position.
         * Optionally can load a simple, one-frame graphic instead.
         *
         * @param   X               The initial X position of the sprite.
         * @param   Y               The initial Y position of the sprite.
         * @param   SimpleGraphic   The graphic you want to display (OPTIONAL - for simple stuff only, do NOT use for animated images!).
         */
        DSprite(unsigned int X = 0, unsigned int Y = 0, void* SimpleGraphics = NULL);
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
        DSprite *loadGraphic(void *Graphic, bool Animated = false, bool Reverse = false, unsigned int Width = 0, unsigned int Height = 0, bool Unique = false);

        /**
         * Create a pre-rotated sprite sheet from a simple sprite.
         * This can make a huge difference in graphical performance!
         *
         * @param   Graphic         The image you want to rotate & stamp.
         * @param   Frames          The number of frames you want to use (more == smoother rotations).
         * @param   Offset          Use this to select a specific frame to draw from the graphic.
         * @param   AntiAliasing    Whether to use high quality rotations when creating the graphic.
         * @param   AutoBuffer      Whether to automatically increase the image size to accomodate rotated corners.
         *
         * @return  This FlxSprite instance (nice for chaining stuff together, if you're into that).
         */
        DSprite *loadRotatedGraphic(void *Graphic, unsigned int Rotations = 16, int Frame = -1, bool AntiAliasing = false, bool AutoBuffer = false);

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
        DSprite *createGraphic(unsigned int Width, unsigned int Height, unsigned int Color = 0xffffffff, bool Unique = false, const std::string &Key = "");

        /**
         * Set <code>pixels</code> to any <code>BitmapData</code> object.
         * Automatically adjust graphic size and render helpers.
         */
        /*
           BitmapData *pixels()
           {
           return _pixels;
           }
           */

        /**
         * @private
         */
        /*
           void pixels(BitmapData *Pixels)
           {
           _pixels = Pixels;
           width = frameWidth = _pixels.width;
           height = frameHeight = _pixels.height;
           resetHelpers();
           }
           */

    protected:
        /**
         * Resets some important variables for sprite optimization and rendering.
         */
        void resetHelpers()
        {
            /*
               _flashRect.x = 0;
               _flashRect.y = 0;
               _flashRect.width = frameWidth;
               _flashRect.height = frameHeight;
               _flashRect2.x = 0;
               _flashRect2.y = 0;
               _flashRect2.width = _pixels.width;
               _flashRect2.height = _pixels.height;
               if((_framePixels == null) || (_framePixels.width != width) || (_framePixels.height != height))
               _framePixels = new BitmapData(width,height);
               if((_bbb == null) || (_bbb.width != width) || (_bbb.height != height))
               _bbb = new BitmapData(width,height);
               origin.x = frameWidth/2;
               origin.y = frameHeight/2;
               _framePixels.copyPixels(_pixels,_flashRect,_flashPointZero);
               if(FlxG.showBounds)
               drawBounds();
               _caf = 0;
               refreshHulls();
               */
        }
    };

}
#endif
