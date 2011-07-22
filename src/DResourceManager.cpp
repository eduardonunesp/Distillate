#include "DResourceManager.hpp"
#include "DTextureResource.hpp"
#include "DTTFResource.hpp"
#include "DResource.hpp"

#if defined(GL_RENDER)
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <png.h>
#endif

#include "DUtils.hpp"

namespace Distillate {
    DTextureLoader *DResourceManager::textureLoader = new DTextureLoader();
    DTTFLoader *DResourceManager::ttfLoader = new DTTFLoader();

    DResourceManager::DResourceManager() {}
    DResourceManager::~DResourceManager() {}

    bool DResourceManager::loadTexture(const std::string &filename)
    {
        return loadTexture(filename, filename);
    }

    bool DResourceManager::loadTexture(const std::string &filename, const std::string &resourceid)
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

        DTextureResource *texRes =  new DTextureResource(filename, resourceid); 
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

    bool DResourceManager::loadTTF(const std::string &filename, unsigned int width, unsigned int size, unsigned int color)
    {
        return loadTTF(filename, filename, width, size, color);
    }

    bool DResourceManager::loadTTF(const std::string &filename, const std::string &resourceid, unsigned int width, unsigned int size, unsigned int color) 
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

        DTTFResource *ttfRes = new DTTFResource(filename, resourceid);
        if(!ttfRes) 
        {
            fprintf(stderr, "Cannot alloc ttfRes\n");
            return false;
        }

        ttfRes->w = width;
        ttfRes->size = size;
        ttfRes->color = color;

        ttfLoader->impl = new DTTFImplementation();
        ttfLoader->impl->process(ttfRes);

        _resources[resourceid] = ttfRes;
        return true;
    }

    void DResourceManager::unloadTTF(const std::string &resourceid)
    {
        if(_resources[resourceid]) {
            delete _resources[resourceid];
            _resources.erase(resourceid);
        }
    }

    bool DResourceManager::createTexture(const std::string &resourceid, unsigned int width, unsigned int height, unsigned int color)
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

        DTextureResource *texRes = new DTextureResource(resourceid, resourceid);
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

    DTextureResource *DResourceManager::texture(const std::string& resourceid)
    {
        if(!_resources[resourceid])
            fprintf(stderr, "DANGER! resource not found\n");            
        return static_cast<DTextureResource*>(_resources[resourceid]);
    }

    DTTFResource *DResourceManager::ttf(const std::string& resourceid)
    {
        if(!_resources[resourceid])
            fprintf(stderr, "DANGER! resource not found\n");            
        return static_cast<DTTFResource*>(_resources[resourceid]);
    }
}
