#ifndef __DSOUND_HPP__
#define __DSOUND_HPP__

#include <string>
#include <vector>

#if defined(SDL_RENDER)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

#include "DObject.hpp"
#include "DUtils.hpp"

namespace Distillate {
    /* Forward */
    class DPoint;
    class DSoundResource;

    /**
     * The main "game object" class, handles basic physics and animation.
     */
    class DSound : public DObject {
        friend class DState;
    protected:        

        /**
         * All sound stuff
         */
        DSoundResource *_sound;
    public:

        /**
         * Whether the sound is currently playing or not.
         */
        bool playing;

        /**
         * The DSound constructor gets all the variables initialized, but NOT ready to play a sound yet.
         */
        DSound();
        ~DSound();

        /**
         * One of two main setup functions for sounds, this function loads a sound from a URL.
         * 
         * @param   EmbeddedSound   A string representing the URL of the MP3 file you want to play.
         * @param   Looped          Whether or not this sound should loop endlessly.
         * 
         * @return  This <code>DSound</code> instance (nice for chaining stuff together, if you're into that).
         */
        DSound *loadSound(const std::string *SoundFile, bool Looped = false);

        /**
         * Call this function if you want this sound's volume to change
         * based on distance from a particular DCore object.
         * 
         * @param   X       The X position of the sound.
         * @param   Y       The Y position of the sound.
         * @param   Core    The object you want to track.
         * @param   Radius  The maximum distance this sound can travel.
         * 
         * @return  This DSound instance (nice for chaining stuff together, if you're into that).
         */
        DSound *proximity(float X, float Y, DObject *Core, float Radius, bool Pan=true); 

        /**
         * Call this function to play the sound.
         */
        void play();

        /**
         * Call this function to pause this sound.
         */
        void pause();

        /**
         * Call this function to stop this sound.
         */
        void stop();

        /**
         * Call this function to make this sound fade out over a certain time interval.
         * 
         * @param   Seconds         The amount of time the fade out operation should take.
         * @param   PauseInstead    Tells the sound to pause on fadeout, instead of stopping.
         */
        void fadeOut(float Seconds, bool PauseInstead);

        /**
         * Call this function to make a sound fade in over a certain
         * time interval (calls <code>play()</code> automatically).
         * 
         * @param   Seconds     The amount of time the fade-in operation should take.
         */
        void fadeIn(float Seconds);

        /**
         * Set <code>volume</code> to a value between 0 and 1 to change how this sound is.
         */
        float getVolume();

        /**
         * @private
         */
        void setVolume();

        /**
         * Internal function that performs the actual logical updates to the sound object.
         * Doesn't do much except optional proximity and fade calculations.
         */
        void updateSound();

        /**
         * The basic game loop update function.  Just calls <code>updateSound()</code>.
         */
        void update();

        /**
         * An internal function used to help organize and change the volume of the sound.
         */
        void updateTransform();
    };
}

#endif /* __DSOUND_HPP__ */
