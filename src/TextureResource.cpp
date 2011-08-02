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

#include "include/TextureResource.hpp"
#include "include/Globals.hpp"

#if defined(PNG_IMAGE_LOADER)
#include <png.h>
#elif defined(SDL_VIDEO)
#include <SDL/SDL_image.h>
#endif

NAMESPACE_BEGIN

bool PNGCheck(const std::string &filename) {
#if defined(SDL_VIDEO)
    return true;
#elif defined(X11_VIDEO)
    png_byte header[8];
    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp) return false;
    fread(header, 1, 8, fp);
    int is_png = !png_sig_cmp(header, 0, 8);
    fclose(fp);
    if (is_png) return true;
#ifdef DEBUG
    fprintf(stdout, "Not PNG File\n");
#endif
    return false;
#endif
}

TextureLoader::TextureType TextureLoader::checkTexture(Resource* r) {
    if(r && r->filename.empty()) return NONE;
    if(PNGCheck(r->filename)) return PNG_TEXTURE;
    return NONE;
}

void PNGTextureImplementation::process(Resource* r) {
    if(!r) {
        fprintf(stderr, "Null Resource detected\n");
        return;
    }

    TextureResource *texRes = static_cast<TextureResource*>(r);
#if defined(SDL_VIDEO) && defined(SW_RENDER)
    SDL_Surface *surface = NULL;
    surface = IMG_Load(r->filename.c_str());

    if(!texRes->data) {
        fprintf(stderr, "Null pointer %s\n", IMG_GetError());
    }

    texRes->data = surface;
    texRes->w = surface->w;
    texRes->h = surface->h;
#elif defined(SDL_VIDEO) && defined(HW_RENDER) && !defined(PNG_IMAGE_LOADER)
    SDL_Surface *surface = NULL;   
    GLenum texture_format;
    GLint  nOfColors;

    surface = IMG_Load(r->filename.c_str()); 

#ifdef DEBUG
    fprintf(stdout, "file: %s (%d, %d)\n", r->filename.c_str(), surface->w, surface->h);
#endif

    if (!surface) {
        fprintf(stderr, "Cannot open file %s: \n", SDL_GetError());
    }

    nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4)     
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else if (nOfColors == 3) {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    } else {
#ifdef DEBUG        
        fprintf(stdout, "warning: the image is not truecolor..  this will probably break\n");
#endif        
    }

    if(!texRes->animated) {
#ifdef DEBUG
        fprintf(stdout, "Single texture load\n");
#endif
        GLuint texture;         
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                texture_format, GL_UNSIGNED_BYTE, surface->pixels );
        texRes->data.push_back(texture);
    } else {
#ifdef DEBUG
        fprintf(stdout, "Multiple texture load (%d)\n", texRes->data.size());
#endif
        SDL_Surface *tmp_surface = NULL;
        GLuint texture;
        SDL_Rect src;

        for(unsigned int i=0; i<texRes->data.size();i++) {
            glGenTextures( 1, &texture );
            glBindTexture( GL_TEXTURE_2D, texture );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                    texture_format, GL_UNSIGNED_BYTE, surface->pixels );
            texRes->data.push_back(texture);
        }
    }

    texRes->w = surface->w;
    texRes->h = surface->h;

    if ( surface ) 
        SDL_FreeSurface( surface );

#elif defined(PNG_IMAGE_LOADER)
    FILE *fp = fopen(r->filename.c_str(), "rb");

    png_byte header[8];
    fread(header, 1, 8, fp);

    if (!fp) {
        fprintf(stderr, "Null point detected\n");
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "Null point detected\n");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        fprintf(stderr, "Null point detected\n");
        return;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        fprintf(stderr, "Null point detected\n");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        fprintf(stderr, "Null point detected [setjmp]\n");
        return;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    int bit_depth, color_type;
    png_uint_32 twidth, theight;

    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

    int width = twidth;
    int height = theight;
    png_read_update_info(png_ptr, info_ptr);

    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_byte *image_data = new png_byte[rowbytes * height];

    if (!image_data) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        fprintf(stderr, "Null point detected\n");
        return;
    }

    png_bytep *row_pointers = new png_bytep[height];
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        fclose(fp);
        return ;
    }

    for (int i = 0; i < height; ++i)
        row_pointers[height - 1 - i] = image_data + i * rowbytes;

    png_read_image(png_ptr, row_pointers);

    glGenTextures(1, &texRes->data);
    glBindTexture(GL_TEXTURE_2D, texRes->data);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    texRes->w = width;
    texRes->h = height;

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] image_data;
    delete[] row_pointers;
    fclose(fp);
#endif

    if(texRes)
        texRes->count++;

    if(texRes->data.size() == 0)
        fprintf(stderr, "Error cannot load texture\n");
}

void AutoTextureImplementation::process(Resource* r)
{
    if(!r) {
        fprintf(stderr, "Null Resource detected\n");
        return;
    }

    TextureResource *texRes = static_cast<TextureResource*>(r);
#if defined(SDL_VIDEO) && defined(SW_RENDER)
    unsigned int rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    texRes->data = SDL_CreateRGBSurface(SDL_SWSURFACE, texRes->w, texRes->h,Globals::bpp,rmask, gmask, bmask, amask);

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = texRes->w;
    rect.h = texRes->h;

    SDL_FillRect(texRes->data, &rect, texRes->color);
#elif defined(HW_RENDER)    
#endif

    if(texRes)
        texRes->count++;

    if(texRes->data.size() == 0)
        fprintf(stderr, "Error cannot create texture\n");
}

NAMESPACE_END
