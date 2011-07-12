#include "EventDispatcher.hpp"
#include "Event.hpp"

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::addEventListener(const std::string &type, eventFunctionPtr listener, int priority)
{
    eventHandlerList[type][priority].push_back(listener);
}

bool EventDispatcher::hasEventListener(const std::string &type)
{
    return (eventHandlerList.find(type) != eventHandlerList.end());
}

void EventDispatcher::dispatchEvent(const Event *event)
{
    if(!event) { ; return; }
    if(!hasEventListener(event->type)) { ; return; }
    std::map<int, std::list<eventFunctionPtr> > &allFunctions = eventHandlerList[event->type];

    for(std::map<int, std::list<eventFunctionPtr > >::reverse_iterator i=allFunctions.rbegin(); i!=allFunctions.rend(); ++i) 
    {
        std::list<eventFunctionPtr> &funcList = i->second;
        for(std::list<eventFunctionPtr>::iterator f=funcList.begin(); f!=funcList.end(); ++f)
            (*f)(event);
    }

    delete event;
}

void EventDispatcher::removeEventListener(const std::string &type, eventFunctionPtr listener)
{
    if(!hasEventListener(type)) { ; return; }
    std::map<int, std::list<eventFunctionPtr> > &allFunctions = eventHandlerList[type];

    for(std::map<int, std::list<eventFunctionPtr> >::iterator i=allFunctions.begin(); i!=allFunctions.end(); ++i) 
    {
        i->second.remove(listener);
        if(i->second.empty())
            allFunctions.erase(i);
    }

    if(allFunctions.empty())
        eventHandlerList.erase(type);
}
