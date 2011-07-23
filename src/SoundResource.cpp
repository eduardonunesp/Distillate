#include "include/SoundResource.hpp"

NAMESPACE_BEGIN

void SoundImplementation::process(Resource* r) {
    if(!r) {
        fprintf(stderr, "Null Resource detected\n");
        return;
    }

    SoundResource *soundRes = static_cast<SoundResource*>(r);
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

NAMESPACE_END
