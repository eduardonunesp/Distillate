libname = 'Distillate';

IncludePath = ['src','src/sdl','src/data','includes']
LibsPath = []   
Libs = ['SDL','SDL_mixer']

CommonFlags = ' -Wall -pedantic'

debug  = ARGUMENTS.get('debug', 1)
shared = ARGUMENTS.get('shared', 0)
libenv = Environment(CPPFLAGS = CommonFlags)
if int(debug):
	libenv.Append(CPPFLAGS = ' -g')

files = Split("""
src/SDL/SDL.cpp
src/SDL/Surface.cpp
src/SDL/Sprite.cpp
src/SDL/Bitmap.cpp
src/SDL/BitmapData.cpp
src/SDL/Graphics.cpp
src/Data/DMouse.cpp
src/Data/DInput.cpp
src/Data/DKeyboard.cpp
src/DEmitter.cpp
src/DGame.cpp
src/DGlobals.cpp
src/DGroup.cpp
src/DObject.cpp
src/DQuadTree.cpp
src/DState.cpp
src/DSprite.cpp
src/DUtils.cpp
""");

if int(shared):
    lib = libenv.SharedLibrary(target = libname, source = files, CPPPATH = IncludePath, LIBS = Libs, LIBPATH = LibsPath)
    obj = libenv.Install("/usr/lib",lib)
    libenv.Alias('install',"/usr/lib")
else:
    files.append("test/main.cpp")
    libenv.Program(target = 'test/test', source = files, CPPPATH = IncludePath, LIBS = Libs, LIBPATH = LibsPath)


