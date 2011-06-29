#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <sstream>
#include "Object.hpp"

namespace Distillate
{
    namespace Backend {

        class Event : public Object 
        {
        public:
            static const std::string ENTER_APP;
            static const std::string RUNNING_APP;

            Event(const std::string &type, bool bubbles = false, bool cancelable = false):
                type(type), bubbles(bubbles), cancelable(cancelable) {}
            virtual ~Event() {}

            std::string type;
            bool bubbles;
            bool cancelable;

            virtual std::string toString(const std::string &more = "Event") const
            {
                std::stringstream ss;
                ss << more << "\n";
                ss << "\ttype:" << type << "\n";
                ss << "\tbubbles:" << (bubbles ? "true" : "false") << "\n";
                ss << "\tcancelable:" << (cancelable ? "true" : "false") << "\n";
                return ss.str();
            }
        };

    }
}

typedef void (*eventFunctionPtr)(const Distillate::Backend::Event &);


#endif // EVENT_HPP
