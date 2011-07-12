#ifndef __DKEYBOARD_HPP__
#define __DKEYBOARD_HPP__

#include <map>

namespace Distillate 
{
    class DKeyboard
    {
    public:
        DKeyboard();
        ~DKeyboard();

        typedef std::map<int, int> KeyMap ;

        void setKeyState(int state, int k);
        int checkKeyState(int state, int k);

    private:
        KeyMap _keys;
    };
}
#endif
