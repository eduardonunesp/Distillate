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

#ifndef __DISTILLATE_HPP__
#define __DISTILLATE_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#endif

#define DISTILLATE_VERSION_NAME   "Vodka"
#define DISTILLATE_MAJOR_VERSION  0
#define DISTILLATE_MINOR_VERSION  0
#define DISTILLATE_PATCH_VERSION  0

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "Defs.hpp"
#include "Anim.hpp"
#include "Emitter.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Group.hpp"
#include "Keyboard.hpp"
#include "List.hpp"
#include "Loader.hpp"
#include "Mouse.hpp"
#include "Object.hpp"
#include "Particle.hpp"
#include "Point.hpp"
#include "QuadTree.hpp"
#include "Rect.hpp"
#include "Resource.hpp"
#include "ResourceManager.hpp"
#include "Sound.hpp"
#include "SoundResource.hpp"
#include "Sprite.hpp"
#include "State.hpp"
#include "Text.hpp"
#include "TextureResource.hpp"
#include "TTFResource.hpp"
#include "Utils.hpp"

#endif /* __DISTILLATE_HPP__ */
