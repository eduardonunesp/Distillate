#ifndef __LIST_HPP__
#define __LIST_HPP__

#include "Defs.hpp"
#include "Object.hpp"

NAMESPACE_BEGIN    
/**
 * Useful for optimizing time-critical or highly repetitive tasks!
 */
class List {
public:
     /**
      * Stores a reference to a <code>Object</code>.
      */
     Object* object;
     /**
      * Stores a reference to the next link in the list.
      */
     List* next;

     /**
      * Creates a new link, and sets <code>object</code> and <code>next</code> to <code>null</null>.
      */
     List() : object(NULL), next(NULL) {};
     ~List() {
          if(object)
               delete object;
          if(next)
               delete next;
     }
};

NAMESPACE_END
#endif /* __LIST_HPP__ */
