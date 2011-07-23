#ifndef __LOADER_HPP__
#define __LOADER_HPP__

#include "Defs.hpp"

NAMESPACE_BEGIN    

class Implementation {
public:
    virtual void process(Resource *r) = 0;
};

class Loader {
public:
    Loader() : impl(NULL) {}
    virtual ~Loader() { delete impl; }
    Implementation *impl;
};

NAMESPACE_END
#endif /* __LOADER_HPP__ */
