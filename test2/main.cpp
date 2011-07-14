#include "Distillate.hpp"
#include "MenuState.hpp"

using namespace Distillate;

class Mode : public DGame
{
public:    
    Mode() : DGame("Mode", 640, 480, new MenuState()) {};
};


int main(int argc, char* argv[])
{
    Mode* mode = NULL;

    try
    {
        mode = new Mode();
    }
    catch(std::exception &e)
    {
        fprintf(stderr,"Ops: %s\n", e.what());
    }

    delete mode;
    return 0;
}
