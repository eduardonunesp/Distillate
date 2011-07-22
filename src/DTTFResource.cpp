#include "DTTFResource.hpp"

namespace Distillate {
    void DTTFImplementation::process(DResource* r) {
#if defined(SDL_RENDER)        
        /* Permits early loaders */
        if(TTF_WasInit() == 0) {
#ifdef DEBUG
            fprintf(stdout, "Early initialize for SDL_TTF\n");
#endif
            if(TTF_Init() < 0) {
                fprintf(stderr, "Cannot initialize TTF system\n");
                return;
            }
        }
#endif

        if(!r) {
            fprintf(stderr, "Null DResource detected\n");
            return;
        }

        DTTFResource *ttfRes = static_cast<DTTFResource*>(r);
        ttfRes->data = TTF_OpenFont(ttfRes->filename.c_str(), ttfRes->size);

        ttfRes->h = TTF_FontHeight(ttfRes->data);

        if(!ttfRes->data) {
            fprintf(stderr, "Null pointer after open ttf: %s\n", TTF_GetError());
            return;
        }

        if(ttfRes)
            ttfRes->count++;

        if(!ttfRes->data)
            fprintf(stderr, "Error cannot load ttf\n");
    }
}
