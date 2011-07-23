#ifndef __TTFRESOURCE_HPP__
#define __TTFRESOURCE_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#endif

#include <string>
#include <cstdio>
#include "Defs.hpp"
#include "Resource.hpp"
#include "Loader.hpp"

NAMESPACE_BEGIN

struct TTFResource : public Resource {
    TTFResource(const std::string &filenameValue, const std::string &resourceidValue) : 
        Resource(filenameValue, resourceidValue), 
        data(NULL), size(8), color(0xffffffff), w(0), h(0) {}
    ~TTFResource() {
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

class TTFLoader : public Loader {
public:
    TTFLoader() : Loader() {}
    ~TTFLoader() {}
};

class TTFImplementation : public Implementation {
public:
    virtual void process(Resource *r);
};

NAMESPACE_END
#endif
