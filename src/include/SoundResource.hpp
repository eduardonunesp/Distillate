#ifndef __SOUNDRESOURCE_HPP__
#define __SOUNDRESOURCE_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

#include <string>
#include <cstdio>
#include "Defs.hpp"
#include "Resource.hpp"
#include "Loader.hpp"

NAMESPACE_BEGIN

class SoundResource : public Resource {
    public:
        SoundResource(const std::string &filenameValue, const std::string &resourceidValue) : 
            Resource(filenameValue, resourceidValue),
            data(NULL) {}
        ~SoundResource() {
#ifdef DEBUG
            fprintf(stdout, "Deleting sound %s", filename.c_str());
#endif

#if defined(SDL_ENGINE)
            Mix_FreeMusic(data);
#endif
        }

#if defined(SDL_ENGINE)            
        Mix_Music *data;
#endif
};

class SoundLoader : public Loader {
    public:
        SoundLoader() : Loader() {}
        ~SoundLoader() {}
};

class SoundImplementation : public Implementation {
    public:
        virtual void process(Resource *r);
};

NAMESPACE_END
#endif /* __SOUNDRESOURCE_HPP__ */
