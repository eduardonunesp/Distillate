#ifndef __DISTILLATE_HPP__
#define __DISTILLATE_HPP__

#if defined(SDL_ENGINE)
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfx.h>
#endif

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "Anim.hpp"
#include "Emitter.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Group.hpp"
#include "istillate.hpp"
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

#define LIBRARY_NAME             "Distillate"
#define LIBRARY_MAJOR_VERSION    0
#define LIBRARY_MINOR_VERSION    0
#define LIBRARY_REVISION_VERSION 0

#endif /* __DISTILLATE_HPP__ */
