#ifndef DLIST_HPP
#define DLIST_HPP

#include "Distillate/DObject.hpp"

namespace Distillate
{

/**
 * The world's smallest linked list class.
 * Useful for optimizing time-critical or highly repetitive tasks!
 * See <code>FlxQuadTree</code> for how to use it, IF YOU DARE.
 */
class DList
{
public:
    /**
     * Stores a reference to a <code>FlxObject</code>.
     */
    DObject* object;
    /**
     * Stores a reference to the next link in the list.
     */
    DList* next;

    /**
     * Creates a new link, and sets <code>object</code> and <code>next</code> to <code>null</null>.
     */
    DList() : object(NULL), next(NULL) {};
    virtual ~DList() {};
};

}

#endif // DLIST_H
