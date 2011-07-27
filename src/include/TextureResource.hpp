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

#ifndef __TEXTURERESOURCE_HPP__
#define __TEXTURERESOURCE_HPP__

#if defined(SDL_VIDEO)
#include <SDL/SDL.h>
#endif

#if defined(HW_RENDER)
#include <GL/gl.h>
#include <GL/glu.h>
#if defined(X11_VIDEO)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>        
#endif
#endif

#include <string>
#include <cstdio>

#include "Defs.hpp"
#include "Utils.hpp"
#include "Resource.hpp"
#include "Loader.hpp"

NAMESPACE_BEGIN

class TextureResource : public Resource {
public:    
    TextureResource(const std::string &filenameValue, const std::string &resourceidValue) : 
        Resource(filenameValue, resourceidValue),
        data(0),
        h(0), w(0) {}
    ~TextureResource() {
#ifdef DEBUG
        fprintf(stdout, "Deleting texture %s", filename.c_str());
#endif

#if defined(SDL_VIDEO) && defined(SW_RENDER)           
        SDL_FreeSurface(data);
        data = NULL;
#elif defined(HW_RENDER)
        if(glIsTexture(data))
            glDeleteTextures(1,&data);
#endif
    }

#if defined(SDL_VIDEO) && defined(SW_RENDER)           
    SDL_Surface *data;
#elif defined(HW_RENDER) && defined(VERTEX_BUFFER)
    typedef struct {
        GLfloat x,y,z; 
        GLfloat t0,t1;
    } DVBO;

    GLuint vboID; 
    DVBO   VBOarr[4];
#elif defined(HW_RENDER)
    GLuint data;
#endif
    unsigned int h;
    unsigned int w;
    unsigned int color;
};

class TextureLoader : public Loader {
public:
    typedef enum {
        PNG_TEXTURE,
        NONE
    } TextureType;

    TextureLoader() : Loader() {}
    ~TextureLoader() {}
    static TextureType checkTexture(Resource* r);
};

class PNGTextureImplementation : public Implementation {
public:
    virtual void process(Resource *r);
};

class AutoTextureImplementation : public Implementation {
public:
    virtual void process(Resource *r);
};

NAMESPACE_END
#endif /* __TEXTURERESOURCE_HPP__ */
