#ifndef __DLOADER_HPP__
#define __DLOADER_HPP__

namespace Distillate {
    class DImplementation {
    public:
        virtual void process(DResource *r) = 0;
    };

    class DLoader {
    public:
        DLoader() : impl(NULL) {}
        virtual ~DLoader() { delete impl; }
        DImplementation *impl;
    };
}

#endif /* __DLOADER_HPP__ */
