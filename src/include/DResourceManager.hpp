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
#endif

#include <map>
#include <string>

namespace Distillate {
    struct Resource {
        Resource(const std::string &filenameValue, const std::string &resourceidValue) :
            filename(filenameValue), resourceid(resourceidValue) {}
        virtual ~Resource() {}
        const std::string &filename;        
        const std::string &resourceid;
        unsigned int ref;
    };

    struct TextureResource : public Resource {
        TextureResource(const std::string &filenameValue, const std::string &resourceidValue) : 
            Resource(filenameValue, resourceidValue) {}
        ~TextureResource() {
#if defined(SDL_RENDER)            
            SDL_FreeSurface(data);
#elif defined(GL_RENDER)
            if(glIsTexture(texID))
                glDeleteTextures(1,&texID);
#endif
            data = NULL;
        }
            
#if defined(SDL_RENDER)            
        SDL_Surface *data;
#elif defined(GL_RENDER) && defined(GL_VBO)
         typedef struct {
             GLfloat x,y,z; 
             GLfloat t0,t1;
         } DVBO;

         GLuint vboID; 
         DVBO   VBOarr[4];
#elif defined(GL_RENDER)
        GLubyte *data;
        GLuint texID;
#endif
        unsigned int h;
        unsigned int w;
    };

    typedef std::map<const std::string, Resource*> ResourceMap;

    class ResourceManager {
        friend class TextureLoader;
    public:
        void loadConfigurationXML(const std::string &filename);
        bool loadTexture(const std::string &filename);
        bool loadAudio(const std::string &filename);

    private:
        ResourceMap _resources;
    };

    class Implementation;
    class TextureLoader {
        Implementation *impl;
        bool load(Resource *r);
    };

    class Implementation {
        virtual void operator() (Resource *r) = 0;
    };

    class PNGTextureImplementation : public Implementation {
        virtual void operator() (Resource *r);
    };
}

#endif /* __RESOURCEMANAGER_HPP__ */
