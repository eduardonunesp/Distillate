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

#include "include/Globals.hpp"
#include "include/Game.hpp"
#include "include/Point.hpp"
#include "include/Keyboard.hpp"
#include "include/Mouse.hpp"
#include "include/Utils.hpp"
#include "include/State.hpp"
#include "include/ResourceManager.hpp"
#include "include/TextureResource.hpp"

#if defined(SDL_VIDEO)
#include <SDL/SDL_image.h>
#endif

NAMESPACE_BEGIN

std::string Globals::gameTitle = "Distillate Game";
float Globals::elapsed;
float Globals::maxElapsed = 0.333333f;
float Globals::timeScale = 1.0f;
Point Globals::scroll;
unsigned int Globals::width  = 0;
unsigned int Globals::height = 0;
unsigned int Globals::bpp    = 32;
unsigned int Globals::FPS = 0;
Game* Globals::_game = NULL;
ResourceManager Globals::resourceManager;
bool Globals::_pause = false;
bool Globals::_running = false;
Keyboard Globals::keys;
Mouse Globals::mouse;

#if defined(SDL_VIDEO)
SDL_Surface *Globals::_buffer = NULL;
#endif

bool Globals::pause()
{
    return _pause;
}

void Globals::setGameData(Game* Game, unsigned int Width, unsigned int Height, unsigned int BPP)
{
    _running = true;
    _game = Game;
    width = Width;
    height = Height;
    bpp = BPP;

#if defined(SDL_VIDEO)
    _buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,Width,Height,Globals::bpp,0,0,0,0);
    if(!_buffer) {
        fprintf(stderr, "%s", "Cannot create buffer \n");
        Globals::quit();
    }
#endif

    Utils::setWorldBounds(0,0,Width, Height);
}

void Globals::setState(const std::string &Name)
{
    _game->switchState(Name);
}

NAMESPACE_END    
