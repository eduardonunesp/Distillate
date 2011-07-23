#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__

#include <string>
#include "Defs.hpp"

NAMESPACE_BEGIN

class Resource {
public:    
    Resource(const std::string &filenameValue, const std::string &resourceidValue) :
        filename(filenameValue), resourceid(resourceidValue) {}
    virtual ~Resource() {}

    const std::string filename;        
    const std::string resourceid;
    unsigned int count;
};

NAMESPACE_END
#endif /* __RESOURCE_HPP__ */
