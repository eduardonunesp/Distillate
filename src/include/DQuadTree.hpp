#ifndef DQUADTREE_HPP
#define DQUADTREE_HPP

#include "DRect.hpp"
#include "DUtils.hpp"

namespace Distillate
{
    class DObject;
    class DList;

    /**
     * A fairly generic quad tree structure for rapid overlap checks.
     * FlxQuadTree is also configured for single or dual list operation.
     * You can add items either to its A list or its B list.
     * When you do an overlap check, you can compare the A list to itself,
     * or the A list against the B list.  Handy for different things!
     */
    class DQuadTree : public DRect
    {
    public:
        /**
         * Set this to null to force it to refresh on the next collide.
         */
        static DQuadTree *quadTree;

        /**
         * This variable stores the dimensions of the root of the quad tree.
         * This is the eligible game collision space.
         */
        static DRect *bounds;

        /** 
         * Controls the granularity of the quad tree.  Default is 3 (decent performance on large and small worlds).
         */  
        static unsigned int divisions;

        /**
         * Flag for specifying that you want to add an object to the A list.
         */
        static const unsigned int A_LIST = 0;

        /**
         * Flag for specifying that you want to add an object to the B list.
         */
        static const unsigned int B_LIST = 1;

    protected:
        /**
         * Whether this branch of the tree can be subdivided or not.
         */
        bool _canSubdivide;

        /**
         * These variables refer to the internal A and B linked lists,
         * which are used to store objects in the leaves.
         */
        DList* _headA;
        DList* _tailA;
        DList* _headB;
        DList* _tailB;

        /**
         * These variables refer to the potential child quadrants for this node.
         */
        static unsigned int _min;
        DQuadTree* _nw;
        DQuadTree* _ne;
        DQuadTree* _se;
        DQuadTree* _sw;
        int _l;
        int _r;
        int _t;
        int _b;
        int _hw;
        int _hh;
        int _mx;
        int _my;

        /**
         * These objects are used to reduce recursive parameters internally.
         */
        static DObject* _o;
        static int _ol;
        static int _ot;
        static int _or;
        static int _ob;
        static unsigned int _oa;
        static DUtils::callbackFunctionQuadTree* _oc;

    public:
        /**
         * Instantiate a new Quad Tree node.
         *
         * @param	X			The X-coordinate of the point in space.
         * @param	Y			The Y-coordinate of the point in space.
         * @param	Width		Desired width of this node.
         * @param	Height		Desired height of this node.
         * @param	Parent		The parent branch or node.  Pass null to create a root.
         */
        DQuadTree(int X, int Y, int Width, int Height, DQuadTree *Parent = NULL);
        virtual ~DQuadTree();

        /**
         * Call this function to add an object to the root of the tree.
         * This function will recursively add all group members, but
         * not the groups themselves.
         *
         * @param	Object		The <code>FlxObject</code> you want to add.  <code>FlxGroup</code> objects will be recursed and their applicable members added automatically.
         * @param	List		A <code>uint</code> flag indicating the list to which you want to add the objects.  Options are <code>A_LIST</code> and <code>B_LIST</code>.
         */
        void add(DObject *Object, unsigned int List);

    protected:
        /**
         * Internal function for recursively navigating and creating the tree
         * while adding objects to the appropriate nodes.
         */
        void addObject();

        /**
         * Internal function for recursively adding objects to leaf lists.
         */
        void addToList();

    public:
        /**
         * <code>FlxQuadTree</code>'s other main function.  Call this after adding objects
         * using <code>FlxQuadTree.add()</code> to compare the objects that you loaded.
         *
         * @param	BothLists	Whether you are doing an A-B list comparison, or comparing A against itself.
         * @param	Callback	A function with two <code>FlxObject</code> parameters - e.g. <code>myOverlapFunction(Object1:FlxObject,Object2:FlxObject);</code>  If no function is provided, <code>FlxQuadTree</code> will call <code>kill()</code> on both objects.
         *
         * @return	Whether or not any overlaps were found.
         */
        bool overlap(bool BothLists = true, DUtils::callbackFunctionQuadTree *Callback = NULL);

    protected:
        /**
         * An internal function for comparing an object against the contents of a node.
         *
         * @param	Iterator	An optional pointer to a linked list entry (for comparing A against itself).
         *
         * @return	Whether or not any overlaps were found.
         */
        bool overlapNode(DList *Iterator = NULL);
    };
}

#endif
