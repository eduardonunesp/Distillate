#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <sstream>
#include "Object.hpp"

class Event : public Object 
{
public:
    Event(const std::string &t, void *d = NULL) : type(t), data(d)  { }
    virtual ~Event() {  }
    virtual std::string toString(const std::string &more = "Event") const
    {
        std::stringstream ss;
        ss << more << "\n";
        ss << "\ttype:" << type;
        return ss.str();
    }

    std::string type;
    void *data;
};

typedef void (*eventFunctionPtr)(const Event*);

#endif // EVENT_HPP
