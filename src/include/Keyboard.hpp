/**
 * Copyright (c) 2010 - 2011 Distillate Project
 *
 *  ______ ________________________            _____________________
 *  |     \  |  |______   |     |  |     |     |_____|   |   |______
 *  |_____/__|________|   |   __|__|_____|_____|     |   |   |______
 *
 *
 * License: BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Wintermoon nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

            UP     = XK_Up,
            DOWN   = XK_Down,
            RIGHT  = XK_Right,
            LEFT   = XK_Left,
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

    typedef std::map<int, unsigned int> KeyMap ;

    Keyboard();
    ~Keyboard();

    inline void setPressed(unsigned int k) { setKeyState(Key::State::PRESSED, k); }
    inline void setReleased(unsigned int k) { setKeyState(Key::State::RELEASED, k); }
    inline bool checkReleased(unsigned int k) { return checkKeyState(Key::State::RELEASED, k); }
    inline bool checkPressed(unsigned int k) { return checkKeyState(Key::State::PRESSED, k); }

    /* Shortcut for keypressed */
    inline bool operator()(int k) { return checkKeyState(Key::State::PRESSED, k); }

    void setKeyState(int state, unsigned int k);
    bool checkKeyState(int state, unsigned int k);

private:
     KeyMap _keys;
};

NAMESPACE_END
#endif /* __DKEYBOARD_HPP__ */
