#ifndef __RESOURCEMANAGER_HPP__
#define __RESOURCEMANAGER_HPP__

#if defined(GL_RENDER)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#elif defined(SDL_RENDER)
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif

#include <map>
#include <string>

namespace Distillate {
    /* Forwards */
    class DResource;
    class DTextureResource;
    class DTextureLoader;
    class DTTFResource;
    class DTTFLoader;

    class DResourceManager {
    public:
        DResourceManager();
        ~DResourceManager();            

        /* Loaders */
        typedef std::map<const std::string, DResource*> DResourceMap;

        /* Texture loaders */
        bool loadConfigurationXML(const std::string &filename) { return true; };
        bool loadTexture(const std::string &filename);
        bool loadTexture(const std::string &filename, const std::string &resourceid);
        
        /* Texture create */
        bool createTexture(const std::string &resourceid, unsigned int width, unsigned int height, unsigned int color);

        /* TTF loaders */
        bool loadTTF(const std::string &filename, unsigned int width, unsigned int size, unsigned int color);
        bool loadTTF(const std::string &filename, const std::string &resourceid, unsigned int width, unsigned int size, unsigned int color);

        /* TTF unloaders */
        void unloadTTF(const std::string &resourceid);

        /* Getters */
        DTextureResource *texture(const std::string& resourceid);
        DTTFResource *ttf(const std::string& resourceid);

    private:
        DResourceMap _resources;
        static DTextureLoader *textureLoader;
        static DTTFLoader *ttfLoader;
    };
}

#endif /* __RESOURCEMANAGER_HPP__ */
