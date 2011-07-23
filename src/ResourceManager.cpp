#include "include/ResourceManager.hpp"
#include "include/TextureResource.hpp"
#include "include/TTFResource.hpp"
#include "include/SoundResource.hpp"
#include "include/Resource.hpp"
#include "include/DUtils.hpp"

NAMESPACE_BEGIN

DTextureLoader *ResourceManager::textureLoader = new DTextureLoader();
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

    switch(DTextureLoader::checkTexture(texRes)) {
        case DTextureLoader::PNG_TEXTURE:
            textureLoader->impl = new DPNGTextureImplementation();
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

    textureLoader->impl = new DAutoTextureImplementation();
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
