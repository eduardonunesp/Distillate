#ifndef __DGROUP_HPP__
#define __DGROUP_HPP__

#include <vector>
#include <string>
#include "DObject.hpp"

namespace Distillate
{
    /* Forward */
    class DPoint;

    /**
     * This is an organizational class that can update and render a bunch of <code>DObject</code>s.
     * NOTE: Although <code>DGroup</code> extends <code>DObject</code>, it will not automatically
     * add itself to the global collisions quad tree, it will only add its members.
     */
    class DGroup : public DObject
    {
        friend class DState;

    public:
        typedef enum {
            ASCENDING = -1,
            DESCENDING = 1
        } SortType;

    protected:
        /**
         * Array of all the <code>DObject</code>s that exist in this layer.
         */
        std::vector<DObject*> members;

        /**
         * Helpers for moving/updating group members.
         */
        DPoint* _last;
        bool _first;

        /**
         * Helpers for sorting members
         */
        std::string _sortIndex;
        int _sortOrder;
            
        /**
         * Internal function, helps with the moving/updating of group members.
         */
        void saveOldPosition();

        /**
         * Internal function that actually goes through and updates all the group members.
         * Depends on <code>saveOldPosition()</code> to set up the correct values in <code>_last</code> in order to work properly.
         */
        void updateMembers();

        /**
         * Internal function that calls kill on all members.
         */
        void killMembers();

        /**
         * Internal function that actually loops through and destroys each member.
         */
        void destroyMembers();

    public:
        DGroup();
        virtual ~DGroup();

        /**
         * Adds a new <code>DObject</code> subclass (DSprite, DBlock, etc) to the list of children
         *
         * @param   Object          The object you want to add
         * @param   ShareScroll     Whether or not this DCore should sync up with this layer's scrollFactor
         *
         * @return  The same <code>DCore</code> object that was passed in.
         */
        DObject* add(DObject *Object, bool ShareScroll=false);

        /**
         * Replaces an existing <code>DObject</code> with a new one.
         *
         * @param   OldObject   The object you want to replace.
         * @param   NewObject   The new object you want to use instead.
         *
         * @return  The new object.
         */
        DObject* replace(DObject *OldObject, DObject *NewObject);

        /**
         * Removes an object from the group.
         *
         * @param   Object  The <code>DObject</code> you want to remove.
         * @param   Splice  Whether the object should be cut from the 
         * array entirely or not.
         *
         * @return  The removed object.
         */
        DObject* remove(DObject *Object, bool Splice=false);

        /** 
         * Call this function to sort the group according to a particular 
         * value and order.     
         * For example, to sort game objects for Zelda-style overlaps you might call
         * <code>myGroup.sort("y",ASCENDING)</code> at the bottom of your
         * <code>DState.update()</code> override.  
         * To sort all existing objects after
         * a big explosion or bomb attack, you might call 
         * <code>myGroup.sort("exists",DESCENDING)</code>.
         * 
         * @param   Index   The <code>String</code> name of the member 
         * variable you want to sort on.  Default value is "y".
         * @param   Order   A <code>DGroup</code> constant that defines 
         * the sort order.  Possible values are <code>ASCENDING</code> 
         * and <code>DESCENDING</code>.  Default value is <code>ASCENDING</code>.  
         */
        void sort(SortType Order=ASCENDING);

        /**
         * Call this function to retrieve the first object with exists == false in the group.
         * This is handy for recycling in general, e.g. respawning enemies.
         *
         * @return  A <code>DObject</code> currently flagged as not existing.
         */
        DObject* getFirstAvail();

        /**
         * Call this function to retrieve the first index set to 'null'.
         * Returns -1 if no index stores a null object.
         *
         * @return  An <code>int</code> indicating the first null slot in the group.
         */
        int getFirstNull();

        /**
         * Finds the first object with exists == false and calls reset on it.
         *
         * @param   X   The new X position of this object.
         * @param   Y   The new Y position of this object.
         *
         * @return  Whether a suitable <code>DObject</code> was found and reset.
         */
        bool resetFirstAvail(float X=0, float Y=0);

        /**
         * Call this function to retrieve the first object with exists == true in the group.
         * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
         *
         * @return  A <code>DObject</code> currently flagged as existing.
         */
        DObject* getFirstExtant();

        /**
         * Call this function to retrieve the first object with dead == false in the group.
         * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
         *
         * @return  A <code>DObject</code> currently flagged as not dead.
         */
        DObject* getFirstAlive();

        /**
         * Call this function to retrieve the first object with dead == true in the group.
         * This is handy for checking if everything's wiped out, or choosing a squad leader, etc.
         *
         * @return  A <code>DObject</code> currently flagged as dead.
         */
        DObject* getFirstDead();

        /**
         * Call this function to find out how many members of the group are not dead.
         *
         * @return  The number of <code>DObject</code>s flagged as not dead.  Returns -1 if group is empty.
         */
        int countLiving();

        /**
         * Call this function to find out how many members of the group are dead.
         *
         * @return  The number of <code>DObject</code>s flagged as dead.  Returns -1 if group is empty.
         */
        int countDead();

        /**
         * Returns a count of how many objects in this group are on-screen right now.
         *
         * @return  The number of <code>DObject</code>s that are on screen.  Returns -1 if group is empty.
         */
        int countOnScreen();

        /**
         * Returns a member at random from the group.
         *
         * @return  A <code>DObject</code> from the members list.
         */
        DObject* getRandom();

        /**
         * Automatically goes through and calls update on everything you added,
         * override this function to handle custom input and perform collisions.
         */
        void update();

        /**
         * Internal function that actually loops through and renders all the group members.
         */
        void renderMembers();

        /**
         * Automatically goes through and calls render on everything you added,
         * override this loop to control render order manually.
         */
        void render();

        /**
         * Calls kill on the group and all its members.
         */
        void kill();

        /**
         * Override this function to handle any deleting or "shutdown" type operations you might need,
         * such as removing traditional Flash children like Sprite objects.
         */
        void destroy();

        /**
         * If the group's position is reset, we want to reset all its members too.
         *
         * @param   X   The new X position of this object.
         * @param   Y   The new Y position of this object.
         */
        void reset(float X, float Y);

        int sortHandler(DObject* Obj1, DObject* Obj2);
    };
}
#endif // DGROUP_HPP
