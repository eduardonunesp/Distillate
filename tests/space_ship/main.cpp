#include <iostream>
#include "Distillate.hpp"

using namespace Distillate;

class State : public DState {
public:
     State() : DState("state") {};
     ~State() {}
};

class Test : public DGame {
public:
     Test() : DGame("Space Ship") {};
};

int main(int argc, char* argv[])
{
     Test test;
     test.setup(640, 480, 16);

     State state;
     test.add(&state, true);

     return test.run();
}
