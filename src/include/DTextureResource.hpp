#ifndef __DTEXTURERESOURCE_HPP__
#define __DTEXTURERESOURCE_HPP__

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

#include <string>
#include "DUtils.hpp"
#include "DResource.hpp"

namespace Distillate {
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

    class DImplementation {
    public:
        virtual void process(DResource *r) = 0;
    };

    class DTextureLoader {
    public:
        typedef enum {
            PNG_TEXTURE,
            NONE
        } TextureType;

        DTextureLoader() : impl(NULL) {}
        ~DTextureLoader() { delete impl; }

        DImplementation *impl;
        static TextureType checkTexture(DResource* r);
    };

    class DPNGTextureImplementation : public DImplementation {
    public:
        virtual void process(DResource *r);
    };
}

#endif /* __DTEXTURERESOURCE_HPP__ */
