#ifndef __DSOUNDRESOURCE_HPP__
#define __DSOUNDRESOURCE_HPP__

#if defined(SDL_RENDER)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

#include <string>
#include <cstdio>
#include "DResource.hpp"
#include "DLoader.hpp"

namespace Distillate {
    struct DSoundResource : public DResource {
        DSoundResource(const std::string &filenameValue, const std::string &resourceidValue) : 
            DResource(filenameValue, resourceidValue),
            data(NULL) {}
        ~DSoundResource() {
#ifdef DEBUG
            fprintf(stdout, "Deleting sound %s", filename.c_str());
#endif

#if defined(SDL_RENDER)
        Mix_FreeMusic(data);
#endif
        }

#if defined(SDL_RENDER)            
        Mix_Music *data;
#endif
    };

    class DSoundLoader : public DLoader {
    public:
        DSoundLoader() : DLoader() {}
        ~DSoundLoader() {}
    };

    class DSoundImplementation : public DImplementation {
    public:
        virtual void process(DResource *r);
    };
}

#endif
