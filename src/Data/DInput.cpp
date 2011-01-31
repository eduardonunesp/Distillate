#include "Distillate/Data/DInput.hpp"

namespace Distillate 
{
    DInput::DInput():
        _lookup(new Generic::Object),
        _t(256) {}


    void DInput::update()
    {
        unsigned int i =0;
        while(i < _t)
        {
            Generic::Object* o = _map[i++];
            if(!o) continue;
            /*TODO: WHATs goes HERE ?! */
        }
    }

    void DInput::reset()
    {
        unsigned int i =0;
        while(i < _t)
        {
            Generic::Object* o = _map[i++];
            if(!o) continue;
        }
    }
}
