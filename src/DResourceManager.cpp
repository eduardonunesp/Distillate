#include "DResourceManager.hpp"

#if defined(GL_RENDER)
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <png.h>
#endif

namespace Distillate {
    void ResourceManager::loadConfigurationXML(const std::string &filename)
    {
    }

    bool ResourceManager::loadTexture(const std::string &filename)
    {
        return true;
    }

    bool ResourceManager::loadAudio(const std::string &filename)
    {
        return true;
    }

#if defined(GL_RENDER)
    void PNGTextureImplementation::operator ()(Resource* r)
    {
        std::string filename;
        FILE *f;
        png_structp png;
        png_infop pinfo, einfo;
        png_color_16p bkcolor;
        int alpha;
        int x, y, i;
        double gamma, sgamma;
        png_uint_32 width, height;
        int depth, junk, color_type;
        png_bytep *png_rows;

        f = fopen(filename.c_str(), "rb");
        if (!f) {
            return ;
        }

        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr) NULL, (png_error_ptr) NULL);
        if (!png) {
            fclose(f);
            return ;
        }

        pinfo = png_create_info_struct(png);
        if (!pinfo) {
            fclose(f);
            png_destroy_read_struct(&png, NULL, NULL);
            return;
        }   

        einfo = png_create_info_struct(png);
        if (!einfo) {
            fclose(f);
            png_destroy_read_struct(&png, &pinfo, NULL);
            return;
        }

#if PNG_LIBPNG_VER - 0 < 10400
        if (setjmp(png->jmpbuf)) 
#else
        if (setjmp(png_jmpbuf(png))) 
#endif
        {
                fclose(f);
                png_destroy_read_struct(&png, &pinfo, &einfo);
                return ;
        }

        png_init_io(png, f);
        png_read_info(png, pinfo);
        png_get_IHDR(png, pinfo, &width, &height, &depth, &color_type, &junk, &junk, &junk);

        /* sanity check */
        if (width < 1 || height < 1) {
            fclose(f);
            png_destroy_read_struct(&png, &pinfo, &einfo);
            return ;
        }

        /* check for an alpha channel */
        if (png_get_valid(png, pinfo, PNG_INFO_tRNS))
            alpha = true;
        else
            alpha = (color_type & PNG_COLOR_MASK_ALPHA);

        /* normalize to 8bpp with alpha channel */
        if (color_type == PNG_COLOR_TYPE_PALETTE && depth <= 8)
            png_set_expand(png);

        if (color_type == PNG_COLOR_TYPE_GRAY && depth <= 8)
            png_set_expand(png);

        if (png_get_valid(png, pinfo, PNG_INFO_tRNS))
            png_set_expand(png);

        if (depth == 16)
            png_set_strip_16(png);

        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png);

        if (png_get_gAMA(png, pinfo, &gamma))
            png_set_gamma(png, sgamma, gamma);
        else
            png_set_gamma(png, sgamma, 0.45);

        /* do the transforms */
        png_read_update_info(png, pinfo);

        /* set background color */
        if (png_get_bKGD(png, pinfo, &bkcolor)) {
            /*
            image->background.red = bkcolor->red >> 8;
            image->background.green = bkcolor->green >> 8;
            image->background.blue = bkcolor->blue >> 8;
            */
        }

        png_rows = (png_byte**) calloc(height, sizeof(char *));
        if (!png_rows) {
            fclose(f);
            png_destroy_read_struct(&png, &pinfo, &einfo);
            return ;
        }
        for (y = 0; y < height; y++) {
            png_rows[y] = (png_bytep) malloc(png_get_rowbytes(png, pinfo));
            if (!png_rows[y]) {
                fclose(f);
                png_destroy_read_struct(&png, &pinfo, &einfo);
                while (y-- > 0)
                    if (png_rows[y])
                        free(png_rows[y]);
                free(png_rows);
                return ;
            }
        }

        /* read data */
        png_read_image(png, png_rows);
        png_read_end(png, einfo);
        png_destroy_read_struct(&png, &pinfo, &einfo);
        fclose(f);
    }
#endif
}
