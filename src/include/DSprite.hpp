#ifndef DSPRITE_HPP
#define DSPRITE_HPP

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include "DObject.hpp"
#include "DUtils.hpp"

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
        friend class DState;
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
        SDL_Surface *_pixels;
        int _alpha;
        unsigned int _color;
        bool _boundsVisible;

    public:

        /**
         * Creates a white 8x8 square <code>FlxSprite</code> at the specified position.
         * Optionally can load a simple, one-frame graphic instead.
         *
         * @param   X               The initial X position of the sprite.
         * @param   Y               The initial Y position of the sprite.
         * @param   SimpleGraphic   The graphic you want to display (OPTIONAL - for simple stuff only, do NOT use for animated images!).
         */
        DSprite(unsigned int X = 0, unsigned int Y = 0, const std::string &SimpleGraphic = "");
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

        virtual void update();

        virtual void render();
    };
}
#endif
