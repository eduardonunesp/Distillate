/**
 * Copyright (c) 2010 - 2011 Distillate Project
 *
 *  ______ ________________________            _____________________
 *  |     \  |  |______   |     |  |     |     |_____|   |   |______
 *  |_____/__|________|   |   __|__|_____|_____|     |   |   |______
 *
 *
 * License: BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Wintermoon nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__

#include "Defs.hpp"
#include "Rect.hpp"
#include "Utils.hpp"

NAMESPACE_BEGIN

class Object;
class List;

/**
 * A fairly generic quad tree structure for rapid overlap checks.
 * QuadTree is also configured for single or dual list operation.
 * You can add items either to its A list or its B list.
 * When you do an overlap check, you can compare the A list to itself,
 * or the A list against the B list.  Handy for different things!
 */
class QuadTree : public Rect {
public:
    /**
     * Set this to null to force it to refresh on the next collide.
     */
    static QuadTree *quadTree;

    /**
     * This variable stores the dimensions of the root of the quad tree.
     * This is the eligible game collision space.
     */
    static Rect *bounds;

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
    List* _headA;
    List* _tailA;
    List* _headB;
    List* _tailB;

    /**
     * These variables refer to the potential child quadrants for this node.
     */
    static unsigned int _min;
    QuadTree* _nw;
    QuadTree* _ne;
    QuadTree* _se;
    QuadTree* _sw;
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
    static Object* _o;
    static int _ol;
    static int _ot;
    static int _or;
    static int _ob;
    static unsigned int _oa;
    static Utils::callbackFunctionQuadTree* _oc;

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
    QuadTree() {};
    QuadTree(int X, int Y, int Width, int Height, QuadTree *Parent = NULL);
    ~QuadTree();

    /**
     * Call this function to add an object to the root of the tree.
     * This function will recursively add all group members, but
     * not the groups themselves.
     *
     * @param	Object		The <code>Object</code> you want to add.  <code>Group</code> objects will be recursed and their applicable members added automatically.
     * @param	List		A <code>uint</code> flag indicating the list to which you want to add the objects.  Options are <code>A_LIST</code> and <code>B_LIST</code>.
     */
    void add(Object *Obj, unsigned int List);

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
     * <code>QuadTree</code>'s other main function.  Call this after adding objects
     * using <code>QuadTree.add()</code> to compare the objects that you loaded.
     *
     * @param	BothLists	Whether you are doing an A-B list comparison, or comparing A against itself.
     * @param	Callback	A function with two <code>Object</code> parameters - e.g. <code>myOverlapFunction(Object1:Object,Object2:Object);</code>  If no function is provided, <code>QuadTree</code> will call <code>kill()</code> on both objects.
     *
     * @return	Whether or not any overlaps were found.
     */
    bool overlap(bool BothLists = true, Utils::callbackFunctionQuadTree *Callback = NULL);

protected:
    /**
     * An internal function for comparing an object against the contents of a node.
     *
     * @param	Iterator	An optional pointer to a linked list entry (for comparing A against itself).
     *
     * @return	Whether or not any overlaps were found.
     */
    bool overlapNode(List *Iterator = NULL);
};

NAMESPACE_END
#endif /* __QUADTREE_HPP__ */
