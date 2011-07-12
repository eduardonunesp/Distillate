#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP

#include <string>
#include <map>
#include <list>

#include "Object.hpp"
#include "Event.hpp"

class EventDispatcher : public Object
{
private:
    std::map<const std::string, std::map<int, std::list<eventFunctionPtr > > > eventHandlerList;

public:
    EventDispatcher();
    virtual ~EventDispatcher();

    void addEventListener(const std::string &type, eventFunctionPtr listener, int priority = 0);
    bool hasEventListener(const std::string &type);
    void dispatchEvent(const Event *event);
    void removeEventListener(const std::string &type, eventFunctionPtr listener);

    virtual std::string toString(const std::string &more = "EventDispatcher") const
    {
        std::stringstream ss;
        ss << Object::toString(more) << "\n";
        ss << "\teventHandlerList.size: " << eventHandlerList.size();
        return ss.str();
    }
};

#endif // EVENTDISPATCHER_HPP
