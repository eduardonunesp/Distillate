#include "DSoundResource.hpp"

namespace Distillate {
    void DSoundImplementation::process(DResource* r) {
        if(!r) {
            fprintf(stderr, "Null DResource detected\n");
            return;
        }

        DSoundResource *soundRes = static_cast<DSoundResource*>(r);
        soundRes->data = Mix_LoadMUS(soundRes->filename.c_str());

        if(!soundRes->data) {
            fprintf(stderr, "Null pointer after open mixer: %s\n", Mix_GetError());
            return;
        }

        if(soundRes)
            soundRes->count++;

        if(!soundRes->data)
            fprintf(stderr, "Error cannot load mixer\n");
    }
}
