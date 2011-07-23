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

#ifndef __RESOURCEMANAGER_HPP__
#define __RESOURCEMANAGER_HPP__

#if defined(GL_ENGINE) && defined(__linux__)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#elif defined(SDL_ENGINE)
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif

#include <map>
#include <string>
#include "Defs.hpp"

NAMESPACE_BEGIN

/* Forwards */
class Resource;
class TextureResource;
class TextureLoader;
class SoundResource;
class SoundLoader;
class TTFResource;
class TTFLoader;

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();            

    /* Loaders */
    typedef std::map<const std::string, Resource*> ResourceMap;

    /* Texture loaders */
    bool loadConfigurationXML(const std::string &filename) { return true; };
    bool loadTexture(const std::string &filename);
    bool loadTexture(const std::string &filename, const std::string &resourceid);
    void attachTexture(TextureResource *resource);

    /* Texture create */
    bool createTexture(const std::string &resourceid, unsigned int width, unsigned int height, unsigned int color);

    /* TTF loaders */
    bool loadTTF(const std::string &filename, unsigned int width, unsigned int size, unsigned int color);
    bool loadTTF(const std::string &filename, const std::string &resourceid, unsigned int width, unsigned int size, unsigned int color);

    /* Sound loaders */
    bool loadSound(const std::string &filename);
    bool loadSound(const std::string &filename, const std::string &resourceid);

    /* TTF unloaders */
    void unloadTTF(const std::string &resourceid);

    /* Getters */
    TextureResource *texture(const std::string& resourceid);
    TTFResource *ttf(const std::string& resourceid);

private:
    ResourceMap _resources;
    static TextureLoader *textureLoader;
    static TTFLoader *ttfLoader;
    static SoundLoader *soundLoader;
};

NAMESPACE_END
#endif /* __RESOURCEMANAGER_HPP__ */
