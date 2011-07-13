#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include <map>
#include <string>
#include <list>
#include "Event.hpp"

class EventDispatcher
{
private:
    std::map<const std::string, std::map<int, std::list > > eventHandlerList;;

public:
    void addEventListener(const std::string &type, eventFunctionPtr listener, 
                          bool useCapture = false, int priority = 0, bool useWeakReference = false)
    {
        eventHandlerList[type][priority].push_back(listener);
    }

    bool hasEventListener(const std::string &type)
    {
        return (eventHandlerList.find(type) != eventHandlerList.end());
    }

    void dispatchEvent(const Event &event)
    {
        if(!hasEventListener(event.type)) return;
        std::map<int, std::list > &allFunctions = eventHandlerList[event.type];

        for(std::map<int, std::list >::reverse_iterator i=allFunctions.rbegin(); i!=allFunctions.rend(); ++i) 
        {
            std::list &funcList = i->second;
            for(std::list::iterator f=funcList.begin(); f!=funcList.end(); ++f)
                (*f)(event);
        }
    }

    void removeEventListener(const std::string &type, eventFunctionPtr listener, bool useCapture = false)
    {
        if(!hasEventListener(type)) return;
        std::map<int, std::list > &allFunctions = eventHandlerList[type];
        
        for(std::map<int, std::list >::iterator i=allFunctions.begin(); i!=allFunctions.end(); ++i) 
        {
            i->second.remove(listener);
            if(i->second.empty())
                allFunctions.erase(i);
        }

        if(allFunctions.empty())
            eventHandlerList.erase(type);
    }
};

#endif // EventDispatcher
