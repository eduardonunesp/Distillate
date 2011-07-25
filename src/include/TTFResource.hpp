/**
 * Copyright (c) 2010 - 2011 Distillate Project
 *
 *  ______ ________________________            _____________________
 *  |     \  |  |______   |     |  |     |     |_____|   |   |______
 *  |_____/__|________|   |   __|__|_____|_____|     |   |   |______
 *
 *
 * License: BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Wintermoon nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
#if defined(SDL_ENGINE)        
        data(NULL), 
#endif        
        size(8), color(0xffffffff), w(0), h(0) {}
    ~TTFResource() {
#ifdef DEBUG
        fprintf(stdout, "Deleting TTF %s", filename.c_str());
#endif

#if defined(SDL_ENGINE)
        TTF_CloseFont(data);
#endif
    }

#if defined(SDL_ENGINE)            
    TTF_Font *data;
#else
    void* data;
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
