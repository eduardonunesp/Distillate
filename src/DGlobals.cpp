#include "DGlobals.hpp"
#include "DGame.hpp"
#include "DPoint.hpp"
#include "DConsole.hpp"
#include "DKeyboard.hpp"
#include "DMouse.hpp"

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
    DKeyboard *DGlobals::keys = new DKeyboard();

    void DGlobals::log(const std::string& log)
    {
        if(_game && _game->_console)
            _game->_console->log(log);
    }

    bool DGlobals::pause()
    {
        return _pause;
    }

    void DGlobals::setGameData(DGame* Game, unsigned int Width, unsigned int Height, unsigned int Zoom)
    {
        _running = true;
        _game = Game;
        width = Width;
        height = Height;
    }
}
