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

#include "include/ResourceManager.hpp"
#include "include/TextureResource.hpp"
#include "include/TTFResource.hpp"
#include "include/SoundResource.hpp"
#include "include/Resource.hpp"
#include "include/Utils.hpp"

NAMESPACE_BEGIN

TextureLoader *ResourceManager::textureLoader = new TextureLoader();
TTFLoader *ResourceManager::ttfLoader = new TTFLoader();
SoundLoader *ResourceManager::soundLoader = new SoundLoader();

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

bool ResourceManager::loadTexture(const std::string &filename)
{
    return loadTexture(filename, filename);
}

bool ResourceManager::loadTexture(const std::string &filename, const std::string &resourceid)
{
    if(_resources[resourceid]) {
#ifdef DEBUG 
        fprintf(stdout,"Loading from buffer %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif
        return true;
    }

#ifdef DEBUG 
    fprintf(stdout,"Trying load texture %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif

    TextureResource *texRes =  new TextureResource(filename, resourceid); 
    if(!texRes) {
        fprintf(stderr, "Cannot alloc texRes\n");
        return false;
    }

    switch(TextureLoader::checkTexture(texRes)) {
        case TextureLoader::PNG_TEXTURE:
            textureLoader->impl = new PNGTextureImplementation();
            textureLoader->impl->process(texRes);
            break;
        default:
            fprintf(stderr, "Unknow type\n");
            return false;
            break;
    }

    _resources[resourceid] = texRes;
    return true;
}

void ResourceManager::attachTexture(TextureResource *resource)
{
    if(!resource) return;
    _resources[resource->resourceid] = resource;
}

bool ResourceManager::loadTTF(const std::string &filename, unsigned int width, unsigned int size, unsigned int color)
{
    return loadTTF(filename, filename, width, size, color);
}

bool ResourceManager::loadTTF(const std::string &filename, const std::string &resourceid, unsigned int width, unsigned int size, unsigned int color) 
{
    if(_resources[resourceid]) {
#ifdef DEBUG 
        fprintf(stdout,"Loading from buffer %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif
        return true;
    }

#ifdef DEBUG 
    fprintf(stdout,"Trying load ttf %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif

    TTFResource *ttfRes = new TTFResource(filename, resourceid);
    if(!ttfRes) 
    {
        fprintf(stderr, "Cannot alloc ttfRes\n");
        return false;
    }

    ttfRes->w = width;
    ttfRes->size = size;
    ttfRes->color = color;

    ttfLoader->impl = new TTFImplementation();
    ttfLoader->impl->process(ttfRes);

    _resources[resourceid] = ttfRes;
    return true;
}

void ResourceManager::unloadTTF(const std::string &resourceid)
{
    if(_resources[resourceid]) {
        delete _resources[resourceid];
        _resources.erase(resourceid);
    }
}

bool ResourceManager::loadSound(const std::string &filename)
{
    return loadSound(filename, filename);
}

bool ResourceManager::loadSound(const std::string &filename, const std::string &resourceid)
{
    if(_resources[resourceid]) {
#ifdef DEBUG 
        fprintf(stdout,"Loading from buffer %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif
        return true;
    }

#ifdef DEBUG 
    fprintf(stdout,"Trying load sound %s, with id %s\n", filename.c_str(), resourceid.c_str()); 
#endif

    SoundResource *soundRes = new SoundResource(filename, resourceid);
    if(!soundRes) 
    {
        fprintf(stderr, "Cannot alloc soundRes\n");
        return false;
    }

    soundLoader->impl = new SoundImplementation();
    soundLoader->impl->process(soundRes);

    _resources[resourceid] = soundRes;
    return true;
}

bool ResourceManager::createTexture(const std::string &resourceid, unsigned int width, unsigned int height, unsigned int color)
{
    if(_resources[resourceid])
    {
#ifdef DEBUG 
        fprintf(stdout,"Loading from buffer %s\n",  resourceid.c_str()); 
#endif
        return true;
    }

#ifdef DEBUG
    fprintf(stdout, "Creating texture\n");
#endif

    TextureResource *texRes = new TextureResource(resourceid, resourceid);
    if(!texRes) 
    {
        fprintf(stderr, "Cannot alloc texRes\n");
        return false;
    }

    texRes->w = width;
    texRes->h = height;
    texRes->color = color;

    textureLoader->impl = new AutoTextureImplementation();
    textureLoader->impl->process(texRes);
    _resources[resourceid] = texRes;
    return true;
}

TextureResource *ResourceManager::texture(const std::string& resourceid)
{
    if(!_resources[resourceid])
        fprintf(stderr, "DANGER! resource not found\n");            
    return static_cast<TextureResource*>(_resources[resourceid]);
}

TTFResource *ResourceManager::ttf(const std::string& resourceid)
{
    if(!_resources[resourceid])
        fprintf(stderr, "DANGER! resource not found\n");            
    return static_cast<TTFResource*>(_resources[resourceid]);
}

NAMESPACE_END    
