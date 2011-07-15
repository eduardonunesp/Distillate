libname = 'Distillate';

IncludePath = ['src','src/include','backend','backend/include']
LibsPath = []   
Libs = ['SDL','SDL_mixer', 'SDL_image', 'SDL_ttf']

CommonFlags = ' -Wall -pedantic'

debug    = ARGUMENTS.get('debug', 0)
shared   = ARGUMENTS.get('shared', 0)
glrender = ARGUMENTS.get('glrender', 0)
libenv = Environment(CPPFLAGS = CommonFlags)

if int(debug):
	libenv.Append(CPPFLAGS = ' -g')

if int(glrender):
    libenv.Append(CPPDEFINES = 'GL_RENDER')

files = Split("""
src/DEmitter.cpp
src/DGame.cpp
src/DGlobals.cpp
src/DGroup.cpp
src/DKeyboard.cpp
src/DMouse.cpp
src/DObject.cpp
src/DQuadTree.cpp
src/DSprite.cpp
src/DState.cpp
src/DText.cpp
src/DUtils.cpp
""");

if int(shared):
    lib = libenv.SharedLibrary(target = libname, source = files, CPPPATH = IncludePath, LIBS = Libs, LIBPATH = LibsPath)
    obj = libenv.Install("/usr/lib",lib)
    libenv.Alias('install',"/usr/lib")
else:
    files.append("tests/flame/main.cpp")
    files.append("tests/flame/FlameState.cpp")
    libenv.Program(target = 'tests/flame/flame', source = files, CPPPATH = IncludePath, LIBS = Libs, LIBPATH = LibsPath)


