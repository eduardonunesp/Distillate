#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP

#include <string>
#include <map>
#include <list>

#include "Object.hpp"
#include "Event.hpp"

namespace Distillate
{
    namespace Backend
    {
        class EventDispatcher : public Object
        {
        private:
            std::map<const std::string, std::map<int, std::list<eventFunctionPtr > > > eventHandlerList;

        public:
            void addEventListener(const std::string &type, eventFunctionPtr listener, 
                    bool useCapture = false, int priority = 0, bool useWeakReference = false);
            bool hasEventListener(const std::string &type);
            void dispatchEvent(const Event &event);
            void removeEventListener(const std::string &type, eventFunctionPtr listener, bool useCapture = false);
        };

    }
}

#endif // EVENTDISPATCHER_HPP
