#ifndef EVENT_HPP
#define EVENT_HPP

class Event
{
public:
    Event(const std::string &type, bool bubbles = false, bool cancelable = false):
    type(type), bubbles(bubbles), cancelable(cancelable) {}
        
    const std::string type;
    const bool bubbles;
    const bool cancelable;
};

typedef void (*eventFunctionPtr)(const Event &);

#endif // EVENT_HPP
