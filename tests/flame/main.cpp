#include "Distillate.hpp"
#include "FlameState.hpp"

using namespace Distillate;

class Flame : public DGame
{
public:    
    Flame() : DGame("Flame", 640, 480, new FlameState()) {};
};


int main(int argc, char* argv[])
{
    try
    {
        Flame flame();
    }
    catch(std::exception &e)
    {
        fprintf(stderr,"Ops: %s\n", e.what());
    }

    return 0;
}
