#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

class Object 
{
public:
    virtual std::string toString(const std::string &more = "Object") const
    {
        return more;
    }
};

#endif // OBJECT
