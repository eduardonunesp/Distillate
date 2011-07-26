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

#include "include/QuadTree.hpp"
#include "include/List.hpp"
#include "include/Rect.hpp"
#include <vector>
#include <cstdio>

NAMESPACE_BEGIN

QuadTree *QuadTree::quadTree = NULL;
Rect     *QuadTree::bounds = NULL;
unsigned int QuadTree::divisions = 3;
unsigned int QuadTree::_min = 0;
Object *QuadTree::_o = NULL;
int QuadTree::_ol = 0;
int QuadTree::_ot = 0;
int QuadTree::_or = 0;
int QuadTree::_ob = 0;
unsigned int QuadTree::_oa = 0;
Utils::callbackFunctionQuadTree *QuadTree::_oc;

QuadTree::QuadTree(int X, int Y, int Width, int Height, QuadTree *Parent):
    Rect(X,Y,Width,Height),
    _headA(new List()),
    _tailA(NULL),
    _headB(new List()),
    _tailB(NULL),
    _nw(NULL),
    _ne(NULL),
    _se(NULL),
    _sw(NULL)
{
    _tailA = _headA;
    _tailB = _headB;

    if(Parent != NULL) {
        List *itr = NULL;
        List *ot  = NULL;
        if(Parent->_headA->object != NULL) {
            itr = Parent->_headA;
            while(itr != NULL) {
                if(_tailA->object != NULL) {
                    ot = _tailA;
                    _tailA = new List();
                    ot->next = _tailA;
                }
                _tailA->object = itr->object;
                itr = itr->next;
            }
        }
        if(Parent->_headB->object != NULL) {
            itr = Parent->_headB;
            while(itr != NULL) {
                if(_tailB->object != NULL) {
                    ot = _tailB;
                    _tailB = new List();
                    ot->next = _tailB;
                }
                _tailB->object = itr->object;
                itr = itr->next;
            }
        }
    } else
        _min = (width + height)/(2*divisions);
    _canSubdivide = (width > _min) || (height > _min);

    _l = x;
    _r = x+width;
    _hw = width/2;
    _mx = _l+_hw;
    _t = y;
    _b = y+height;
    _hh = height/2;
    _my = _t+_hh;
}

QuadTree::~QuadTree()
{
    delete _headA;
    delete _headB;
    delete _nw;
    delete _ne;
    delete _se;
    delete _sw;
}

void QuadTree::add(Object *Obj, unsigned int List)
{
    _oa = List;
    if(Obj->_group) {
        Object *m;
        std::vector<Object*> members;
        unsigned int l = members.size();
        for(unsigned int i = 0; i < l; i++) {
            m = members[i];
            if((m != NULL) && m->exists) {
                if(m->_group)
                    add(m,List);
                else if(m->solid()) {
                    _o = m;
                    _ol = _o->x;
                    _ot = _o->y;
                    _or = _o->x + _o->width;
                    _ob = _o->y + _o->height;
                    addObject();
                }
            }
        }
    }
    if(Obj->solid()) {
        _o = Obj;
        _ol = _o->x;
        _ot = _o->y;
        _or = _o->x + _o->width;
        _ob = _o->y + _o->height;
        addObject();
    }
}

void QuadTree::addObject()
{
    //If this quad (not its children) lies entirely inside this object, add it here
    if(!_canSubdivide || ((_l >= _ol) && (_r <= _or) && (_t >= _ot) && (_b <= _ob))) {
        addToList();
        return;
    }

    if((_ol > _l) && (_or < _mx)) {
        if((_ot > _t) && (_ob < _my)) {
            if(_nw == NULL)
                _nw = new QuadTree(_l,_t,_hw,_hh,this);
            _nw->addObject();
            return;
        }
        if((_ot > _my) && (_ob < _b)) {
            if(_sw == NULL)
                _sw = new QuadTree(_l,_my,_hw,_hh,this);
            _sw->addObject();
            return;
        }
    }

    if((_ol > _mx) && (_or < _r)) {
        if((_ot > _t) && (_ob < _my)) {
            if(_ne == NULL)
                _ne = new QuadTree(_mx,_t,_hw,_hh,this);
            _ne->addObject();
            return;
        }
        if((_ot > _my) && (_ob < _b)) {
            if(_se == NULL)
                _se = new QuadTree(_mx,_my,_hw,_hh,this);
            _se->addObject();
            return;
        }
    }

    if((_or > _l) && (_ol < _mx) && (_ob > _t) && (_ot < _my)) {
        if(_nw == NULL)
            _nw = new QuadTree(_l,_t,_hw,_hh,this);
        _nw->addObject();
    }
    if((_or > _mx) && (_ol < _r) && (_ob > _t) && (_ot < _my)) {
        if(_ne == NULL)
            _ne = new QuadTree(_mx,_t,_hw,_hh,this);
        _ne->addObject();
    }
    if((_or > _mx) && (_ol < _r) && (_ob > _my) && (_ot < _b)) {
        if(_se == NULL)
            _se = new QuadTree(_mx,_my,_hw,_hh,this);
        _se->addObject();
    }
    if((_or > _l) && (_ol < _mx) && (_ob > _my) && (_ot < _b)) {
        if(_sw == NULL)
            _sw = new QuadTree(_l,_my,_hw,_hh,this);
        _sw->addObject();
    }
}

void QuadTree::addToList()
{
    List* ot = NULL;
    if(_oa == A_LIST) {
        if(_tailA->object != NULL) {
            ot = _tailA;
            _tailA = new List();
            ot->next = _tailA;
        }
        _tailA->object = _o;
    } else {
        if(_tailB->object != NULL) {
            ot = _tailB;
            _tailB = new List();
            ot->next = _tailB;
        }
        _tailB->object = _o;
    }
    if(!_canSubdivide)
        return;
    if(_nw != NULL)
        _nw->addToList();
    if(_ne != NULL)
        _ne->addToList();
    if(_se != NULL)
        _se->addToList();
    if(_sw != NULL)
        _sw->addToList();
}

bool QuadTree::overlap(bool BothLists, Utils::callbackFunctionQuadTree *Callback)
{
    _oc = Callback;
    bool c = false;
    List* itr;
    if(BothLists) {
        _oa = B_LIST;
        if(_headA->object != NULL) {
            itr = _headA;
            while(itr != NULL) {
                _o = itr->object;
                if(_o->exists && _o->solid() && overlapNode())
                    c = true;
                itr = itr->next;
            }
        }
        _oa = A_LIST;
        if(_headB->object != NULL) {
            itr = _headB;
            while(itr != NULL) {
                _o = itr->object;
                if(_o->exists && _o->solid()) {
                    if((_nw != NULL) && _nw->overlapNode())
                        c = true;
                    if((_ne != NULL) && _ne->overlapNode())
                        c = true;
                    if((_se != NULL) && _se->overlapNode())
                        c = true;
                    if((_sw != NULL) && _sw->overlapNode())
                        c = true;
                }
                itr = itr->next;
            }
        }
    } else {
        if(_headA->object != NULL) {
            itr = _headA;
            while(itr != NULL) {
                _o = itr->object;
                if(_o->exists && _o->solid() && overlapNode(itr->next))
                    c = true;
                itr = itr->next;
            }
        }
    }

    if((_nw != NULL) && _nw->overlap(BothLists,_oc))
        c = true;
    if((_ne != NULL) && _ne->overlap(BothLists,_oc))
        c = true;
    if((_se != NULL) && _se->overlap(BothLists,_oc))
        c = true;
    if((_sw != NULL) && _sw->overlap(BothLists,_oc))
        c = true;

    return c;
}

bool QuadTree::overlapNode(List *Iterator)
{
    bool c = false;
    Object* co;
    List* itr = Iterator;
    if(itr == NULL) {
        if(_oa == A_LIST)
            itr = _headA;
        else
            itr = _headB;
    }

    if(itr->object != NULL) {
        while(itr != NULL) {
            co = itr->object;
            if( (_o == co) || !co->exists || !_o->exists || !co->solid() || !_o->solid() ||
                (_o->x + _o->width  < co->x + Utils::roundingError) ||
                (_o->x + Utils::roundingError > co->x + co->width)  ||
                (_o->y + _o->height < co->y + Utils::roundingError) ||
                (_o->y + Utils::roundingError > co->y + co->height) ) {
                itr = itr->next;
                continue;
            }
            if(_oc == NULL) {
                _o->kill();
                co->kill();
                c = true;
            } else if(_oc(_o,co))
                c = true;
            itr = itr->next;
        }
    }

    return c;
}

NAMESPACE_END
