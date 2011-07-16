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
    Flame* flame = NULL;

    try
    {
        flame = new Flame();
    }
    catch(std::exception &e)
    {
        fprintf(stderr,"Ops: %s\n", e.what());
    }

    delete flame;
    return 0;
}
