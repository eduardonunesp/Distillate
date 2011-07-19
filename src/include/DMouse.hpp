#ifndef __DMOUSE_HPP__
#define __DMOUSE_HPP__

#include <map>

#ifndef NULL
#define NULL 0
#endif

namespace Distillate {
    class DMouse {
    public:
         DMouse();
         ~DMouse();

         typedef std::map<int, int> ButtonMap ;

         void setButtonState(int state, int b);
         bool checkButtonState(int state, int b);
         void setMousePos(int x, int y);
         void showCursor(bool show);


    private:
         int _x;
         int _y;
         ButtonMap _buttons;
    };
}
#endif
