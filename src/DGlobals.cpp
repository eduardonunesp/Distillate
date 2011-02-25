#include "Distillate/DGlobals.hpp"
#include "Distillate/DGame.hpp"
#include "Distillate/DPoint.hpp"
#include "Distillate/Data/DConsole.hpp"

namespace Distillate
{
    const std::string DGlobals::LIBRARY_NAME = "Distillate";
    float DGlobals::elapsed;
    float DGlobals::maxElapsed;
    float DGlobals::timeScale;
    DPoint *DGlobals::scroll = new DPoint();
    unsigned int DGlobals::width  = 0;
    unsigned int DGlobals::height = 0;
    DGame* DGlobals::_game = NULL;
    bool DGlobals::_pause = false;
    bool DGlobals::_running = false;

    void DGlobals::log(const std::string& log)
    {
        if(_game && _game->_console)
            _game->_console->log(log);
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
        std::string key = Key;
        /*
        if(!key.empty())
        {
            key = String(Graphic);
            if(Unique && (_cache[key] != undefined) && (_cache[key] != null))
            {
                //Generate a unique key
                var inc:uint = 0;
                var ukey:String;
                do { ukey = key + inc++;
                } while((_cache[ukey] != undefined) && (_cache[ukey] != null));
                key = ukey;
            }
        }
        //If there is no data for this key, generate the requested graphic
        if(!checkBitmapCache(key))
        {
            _cache[key] = (new Graphic).bitmapData;
            if(Reverse) needReverse = true;
        }
        var pixels:BitmapData = _cache[key];
        if(!needReverse && Reverse && (pixels.width == (new Graphic).bitmapData.width))
            needReverse = true;
        if(needReverse)
        {
            var newPixels:BitmapData = new BitmapData(pixels.width<<1,pixels.height,true,0x00000000);
            newPixels.draw(pixels);
            var mtx:Matrix = new Matrix();
            mtx.scale(-1,1);
            mtx.translate(newPixels.width,0);
            newPixels.draw(pixels,mtx);
            pixels = newPixels;
        }
        return pixels;


        bool needReverse = false;

        std::string key = "";
        */
        return NULL;
    }

     void DGlobals::setGameData(DGame* Game, unsigned int Width, unsigned int Height, unsigned int Zoom)
     {
         _running = true;
         _game = Game;
         width = Width;
         height = Height;
         timeScale = 1.0f;
         maxElapsed = 0.0333333;
     }
}
