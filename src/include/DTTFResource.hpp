#ifndef __DTTFRESOURCE_HPP__
#define __DTTFRESOURCE_HPP__

#if defined(SDL_RENDER)
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#endif

#include <string>
#include <cstdio>
#include "DResource.hpp"
#include "DLoader.hpp"

namespace Distillate {
    struct DTTFResource : public DResource {
        DTTFResource(const std::string &filenameValue, const std::string &resourceidValue) : 
            DResource(filenameValue, resourceidValue), 
            data(NULL), size(8), color(0xffffffff), w(0), h(0) {}
        ~DTTFResource() {
#ifdef DEBUG
            fprintf(stdout, "Deleting TTF %s", filename.c_str());
#endif

#if defined(SDL_RENDER)
            TTF_CloseFont(data);
#endif
        }

#if defined(SDL_RENDER)            
        TTF_Font *data;
#endif
        unsigned int size;
        unsigned int color;
        unsigned int w;
        unsigned int h;
    };

    class DTTFLoader : public DLoader {
    public:
        DTTFLoader() : DLoader() {}
        ~DTTFLoader() {}
    };

    class DTTFImplementation : public DImplementation {
    public:
        virtual void process(DResource *r);
    };
}

#endif
