#ifndef __DRESOURCE_HPP__
#define __DRESOURCE_HPP__

#include <string>

namespace Distillate {
    struct DResource {
        DResource(const std::string &filenameValue, const std::string &resourceidValue) :
            filename(filenameValue), resourceid(resourceidValue) {}
        virtual ~DResource() {}
        const std::string filename;        
        const std::string resourceid;
        unsigned int count;
    };
}

#endif /* __DRESOURCE_HPP__ */
