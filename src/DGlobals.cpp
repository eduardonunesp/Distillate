#include "Distillate/DGlobals.hpp"
#include "Distillate/DGame.hpp"
#include "Distillate/DPoint.hpp"

namespace Distillate
{
    float  DGlobals::elapsed;
    const  std::string DGlobals::LIBRARY_NAME = "Distillate";
    DPoint *DGlobals::scroll = new DPoint();
    unsigned int DGlobals::width  = 0;
    unsigned int DGlobals::height = 0;
    bool DGlobals::_pause = false;

    DGlobals::DGlobals()
    {
        //ctor
    }

    DGlobals::~DGlobals()
    {
        //dtor
    }

    bool DGlobals::pause()
    {
        return _pause;
    }

    void DGlobals::pause(bool Pause)
    {
        bool op = _pause;
        _pause = Pause;
        if(_pause != op)
        {
            if(_pause)
            {
                //_game->pauseGame();
                //pauseSounds();
            }
            else
            {
                //_game->unpauseGame();
                //playSounds();
            }
        }
    }

    SDL::BitmapData* DGlobals::addBitmap(void* Graphic, bool Reverse, bool Unique, const std::string& Key)
    {
        bool needReverse = false;
        std::string key = "";
            return NULL;
    }
}
