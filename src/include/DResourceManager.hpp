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

    class DResourceManager {
    public:
        DResourceManager();
        ~DResourceManager();            

        /* Loaders */
        typedef std::map<const std::string, DResource*> DResourceMap;
        bool loadConfigurationXML(const std::string &filename) { return true; };
        bool loadTexture(const std::string &filename);
        bool loadTexture(const std::string &filename, const std::string &resourceid);
        bool createTexture(unsigned int width, unsigned int height, unsigned int color, const std::string &resourceid);

        /* Getters */
        DTextureResource *texture(const std::string& resourceid);

    private:
        DResourceMap _resources;
        static DTextureLoader *textureLoader;
    };
}

#endif /* __RESOURCEMANAGER_HPP__ */
