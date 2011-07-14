libname = 'Distillate';

IncludePath = ['src','src/include','backend','backend/include']
LibsPath = []   
Libs = ['SDL','SDL_mixer', 'SDL_image', 'SDL_ttf']

CommonFlags = ' -Wall -pedantic'

debug  = ARGUMENTS.get('debug', 1)
shared = ARGUMENTS.get('shared', 0)
libenv = Environment(CPPFLAGS = CommonFlags)
if int(debug):
	libenv.Append(CPPFLAGS = ' -g')

files = Split("""
src/DClass.cpp
src/DMouse.cpp
src/DInput.cpp
src/DKeyboard.cpp
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
    files.append("test2/main.cpp")
    files.append("test2/MenuState.cpp")
    libenv.Program(target = 'test2/mode', source = files, CPPPATH = IncludePath, LIBS = Libs, LIBPATH = LibsPath)


