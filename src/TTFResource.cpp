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

#include "include/TTFResource.hpp"

NAMESPACE_BEGIN

void TTFImplementation::process(Resource* r) {
#if defined(SDL_VIDEO)        
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
#if defined(SDL_VIDEO)    
    ttfRes->data = TTF_OpenFont(ttfRes->filename.c_str(), ttfRes->size);

    ttfRes->h = TTF_FontHeight(ttfRes->data);

    if(!ttfRes->data) {
        fprintf(stderr, "Null pointer after open ttf: %s\n", TTF_GetError());
        return;
    }
#endif

    if(ttfRes)
        ttfRes->count++;

    if(!ttfRes->data)
        fprintf(stderr, "Error cannot load ttf\n");
}

NAMESPACE_END
