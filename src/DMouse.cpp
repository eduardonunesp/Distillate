#include "DMouse.hpp"
#include "DSprite.hpp"

namespace Distillate
{
    DMouse::DMouse():
        x(0),
        y(0),
        screenX(0),
        screenY(0),
        cursor(false),
        _current(0),
        _last(0),
        _out(false)
    {}

    void DMouse::show(void *Graphic, int XOffset, int YOffset)
    {
        _out = true;
        if(Graphic)
            load(Graphic, XOffset, YOffset);
        else if(cursor)
            cursor->visible = true;
        else
            load(NULL);
    }


    void DMouse::load(void* Graphic, int XOffset, int YOffset)
    {
        /* TODO: MUST SET DEFAULT
        if(!Graphic)
            Graphic = ImgDefaultCursor;
        */
        cursor = new DSprite(screenX, screenY, Graphic);
        cursor->solid(false);
        cursor->offset->x = XOffset;
        cursor->offset->y = YOffset;
    }

    void DMouse::update(int X, int Y, float XScroll, float YScroll)
    {
        screenX = X;
        screenY = Y;
        x = screenX-DUtils::floorValue(XScroll);
        y = screenY-DUtils::floorValue(YScroll);

        if(cursor)
        {
            cursor->x = x;
            cursor->y = y;
        }

        if((_last == -1) && (_current == -1))
            _current = 0;
        else if((_last == 2) && (_current == 2))
            _current = 1;
        _last = _current;
    }

    void DMouse::unload()
    {
        if(cursor)
        {
            if(cursor->visible)
                load(NULL);
            else
                cursor = NULL;
        }
    }

    void DMouse::hide()
    {
        if(cursor)
        {
            cursor->visible = false;
            _out = false;
        }
    }
}
