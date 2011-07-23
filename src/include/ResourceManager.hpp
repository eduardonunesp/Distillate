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
