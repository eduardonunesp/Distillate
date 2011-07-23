#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

#include <map>
#include "Defs.hpp"

NAMESPACE_BEGIN

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

NAMESPACE_END    
#endif /* __MOUSE_HPP__ */
