#ifndef __DCLASS_HPP__
#define __DCLASS_HPP__

namespace Distillate
{
     /**
     * All classes comes from here
     */
    class DClass
    {
    protected:
        DClass() : id(++_id) {};
        unsigned long id;
    private:
        static unsigned long _id;
    };
}

#endif
