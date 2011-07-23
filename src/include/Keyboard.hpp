#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#endif

#if defined(__linux__) && defined(GL_ENGINE)
#define XK_MISCELLANY
#define XK_LATIN1
#define XK_TECHNICAL
#include <X11/keysymdef.h>
#endif

#include <map>
#include "Defs.hpp"

NAMESPACE_BEGIN

class Keyboard {
public:
    struct Key {
        typedef enum {
            __FIRST__ = 0,
#if defined(SDL_ENGINE)
            ESCAPE = SDLK_ESCAPE,
            RETURN = SDLK_RETURN,
            SPACE  = SDLK_SPACE,

            A      = SDLK_a,
            B      = SDLK_b,
            C      = SDLK_c,
            D      = SDLK_d,
            E      = SDLK_e,
            F      = SDLK_f,
            G      = SDLK_g,
            H      = SDLK_h,
            I      = SDLK_i,
            J      = SDLK_j,
            K      = SDLK_k,
            L      = SDLK_l,
            M      = SDLK_m,
            N      = SDLK_n,
            O      = SDLK_o,
            P      = SDLK_p,
            Q      = SDLK_q,
            R      = SDLK_r,
            S      = SDLK_s,
            T      = SDLK_t,
            U      = SDLK_u,
            V      = SDLK_v,
            W      = SDLK_w,
            X      = SDLK_x,
            Y      = SDLK_y,
            Z      = SDLK_z,

            UP     = SDLK_UP,
            DOWN   = SDLK_DOWN,
            RIGHT  = SDLK_RIGHT,
            LEFT   = SDLK_LEFT,
#elif defined(GL_ENGINE) && defined(__linux__)
            ESCAPE = XK_Escape,
            RETURN = XK_Return,
            SPACE  = XK_space,

            A      = (XK_A|XK_a),
            B      = (XK_B|XK_b),
            C      = (XK_C|XK_c),
            D      = (XK_D|XK_d),
            E      = (XK_E|XK_e),
            F      = (XK_F|XK_f),
            G      = (XK_G|XK_g),
            H      = (XK_H|XK_h),
            I      = (XK_I|XK_i),
            J      = (XK_J|XK_j),
            L      = (XK_L|XK_l),
            M      = (XK_M|XK_m),
            N      = (XK_N|XK_n),
            O      = (XK_O|XK_o),
            P      = (XK_P|XK_p),
            Q      = (XK_Q|XK_q),
            R      = (XK_R|XK_r),
            S      = (XK_S|XK_s),
            T      = (XK_T|XK_t),
            U      = (XK_U|XK_u),
            V      = (XK_V|XK_v),
            X      = (XK_X|XK_x),
            Y      = (XK_Y|XK_y),
            Z      = (XK_Z|XK_z),

            UP     = XK_uparrow,
            DOWN   = XK_downarrow,
            RIGHT  = XK_rightarrow,
            LEFT   = XK_leftarrow,
#endif
            __LAST__   
        } Keys;

        struct State {
            typedef enum {
                RELEASED,
                PRESSED,
                CLEARED
            } States;
        };
    };

    typedef std::map<int, int> KeyMap ;

    Keyboard();
    ~Keyboard();

    inline void setPressed(int k) { setKeyState(Key::State::PRESSED, k); }
    inline void setReleased(int k) { setKeyState(Key::State::RELEASED, k); }
    inline bool checkReleased(int k) { return checkKeyState(Key::State::RELEASED, k); }
    inline bool checkPressed(int k) { return checkKeyState(Key::State::PRESSED, k); }

    /* Shortcut for keypressed */
    inline bool operator()(int k) { return checkKeyState(Key::State::PRESSED, k); }

    void setKeyState(int state, int k);
    bool checkKeyState(int state, int k);

private:
     KeyMap _keys;
};

NAMESPACE_END
#endif /* __DKEYBOARD_HPP__ */
