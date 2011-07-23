#include "include/TTFResource.hpp"

NAMESPACE_BEGIN

void TTFImplementation::process(Resource* r) {
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
        fprintf(stderr, "Null Resource detected\n");
        return;
    }

    TTFResource *ttfRes = static_cast<TTFResource*>(r);
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

NAMESPACE_END
