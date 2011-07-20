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
#include "DUtils.hpp"

namespace Distillate {
    struct DResource {
        DResource(const std::string &filenameValue, const std::string &resourceidValue) :
            filename(filenameValue), resourceid(resourceidValue) {}
        virtual ~DResource() {}
        const std::string filename;        
        const std::string resourceid;
        unsigned int count;
    };

    struct DTextureResource : public DResource {
        DTextureResource(const std::string &filenameValue, const std::string &resourceidValue) : 
            DResource(filenameValue, resourceidValue) {}
        ~DTextureResource() {
#ifdef DEBUG
            fprintf(stdout, "Deleting texture %s", filename.c_str());
#endif

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

    class Implementation {
    public:
        virtual void process(DResource *r) = 0;
    };

    class PNGTextureImplementation : public Implementation {
    public:
        virtual void process(DResource *r);
    };

    class TextureLoader {
    public:
        typedef enum {
            PNG_TEXTURE,
            NONE
        } TextureType;

        TextureLoader() : impl(NULL) {}
        ~TextureLoader() { delete impl; }

        Implementation *impl;
        static TextureType checkTexture(DResource* r);
    };

    typedef std::map<const std::string, DResource*> DResourceMap;

    class DResourceManager {
    public:
        void loadConfigurationXML(const std::string &filename);
        bool loadTexture(const std::string &filename, const std::string &resourceid);

        DTextureResource *texture(const std::string& resourceid) {
            return static_cast<DTextureResource*>(_resources[resourceid]);
        }

        DResourceManager() : 
            textureLoader(new TextureLoader) {} 
        ~DResourceManager() {
            delete_all(_resources);
            delete textureLoader;     
        }
            
    private:
        DResourceMap _resources;
        TextureLoader *textureLoader;
    };
}

#endif /* __RESOURCEMANAGER_HPP__ */
