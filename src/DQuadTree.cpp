#include "DQuadTree.hpp"
#include "DList.hpp"
#include "DRect.hpp"
#include <vector>

namespace Distillate
{

DQuadTree *DQuadTree::quadTree = NULL;
DRect     *DQuadTree::bounds = NULL;
unsigned int DQuadTree::divisions = 3;
unsigned int DQuadTree::_min = 0;
DObject *DQuadTree::_o = NULL;
int DQuadTree::_ol = 0;
int DQuadTree::_ot = 0;
int DQuadTree::_or = 0;
int DQuadTree::_ob = 0;
unsigned int DQuadTree::_oa = 0;
DUtils::callbackFunctionQuadTree *DQuadTree::_oc;

DQuadTree::DQuadTree(int X, int Y, int Width, int Height, DQuadTree *Parent):
    DRect(X,Y,Width,Height),
    _headA(new DList()),
    _tailA(new DList()),
    _headB(new DList()),
    _tailB(new DList()),
    _nw(NULL),
    _ne(NULL),
    _se(NULL),
    _sw(NULL)
{
    if(Parent != NULL)
    {
        DList *itr = NULL;
        DList *ot  = NULL;
        if(Parent->_headA->object != NULL)
        {
            itr = Parent->_headA;
            while(itr != NULL)
            {
                if(_tailA->object != NULL)
                {
                    ot = _tailA;
                    _tailA = new DList();
                    ot->next = _tailA;
                }
                _tailA->object = itr->object;
                itr = itr->next;
            }
        }
        if(Parent->_headB->object != NULL)
        {
            itr = Parent->_headB;
            while(itr != NULL)
            {
                if(_tailB->object != NULL)
                {
                    ot = _tailB;
                    _tailB = new DList();
                    ot->next = _tailB;
                }
                _tailB->object = itr->object;
                itr = itr->next;
            }
        }
    }
    else
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

DQuadTree::~DQuadTree()
{
    delete _headA;
    delete _tailA;
    delete _headB;
    delete _tailB;
    delete _nw;
    delete _ne;
    delete _se;
    delete _sw;
}

void DQuadTree::add(DObject *Object, unsigned int List)
{
    _oa = List;
    if(Object->_group)
    {
        DObject *m;
        std::vector<DObject*> members;
        unsigned int l = members.size();
        for(unsigned int i = 0; i < l; i++)
        {
            m = members[i];
            if((m != NULL) && m->exists)
            {
                if(m->_group)
                    add(m,List);
                else if(m->solid())
                {
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
    if(Object->solid())
    {
        _o = Object;
        _ol = _o->x;
        _ot = _o->y;
        _or = _o->x + _o->width;
        _ob = _o->y + _o->height;
        addObject();
    }
}

void DQuadTree::addObject()
{
    //If this quad (not its children) lies entirely inside this object, add it here
    if(!_canSubdivide || ((_l >= _ol) && (_r <= _or) && (_t >= _ot) && (_b <= _ob)))
    {
        addToList();
        return;
    }

    if((_ol > _l) && (_or < _mx))
    {
        if((_ot > _t) && (_ob < _my))
        {
            if(_nw == NULL)
                _nw = new DQuadTree(_l,_t,_hw,_hh,this);
            _nw->addObject();
            return;
        }
        if((_ot > _my) && (_ob < _b))
        {
            if(_sw == NULL)
                _sw = new DQuadTree(_l,_my,_hw,_hh,this);
            _sw->addObject();
            return;
        }
    }

    if((_ol > _mx) && (_or < _r))
    {
        if((_ot > _t) && (_ob < _my))
        {
            if(_ne == NULL)
                _ne = new DQuadTree(_mx,_t,_hw,_hh,this);
            _ne->addObject();
            return;
        }
        if((_ot > _my) && (_ob < _b))
        {
            if(_se == NULL)
                _se = new DQuadTree(_mx,_my,_hw,_hh,this);
            _se->addObject();
            return;
        }
    }

    if((_or > _l) && (_ol < _mx) && (_ob > _t) && (_ot < _my))
    {
        if(_nw == NULL)
            _nw = new DQuadTree(_l,_t,_hw,_hh,this);
        _nw->addObject();
    }
    if((_or > _mx) && (_ol < _r) && (_ob > _t) && (_ot < _my))
    {
        if(_ne == NULL)
            _ne = new DQuadTree(_mx,_t,_hw,_hh,this);
        _ne->addObject();
    }
    if((_or > _mx) && (_ol < _r) && (_ob > _my) && (_ot < _b))
    {
        if(_se == NULL)
            _se = new DQuadTree(_mx,_my,_hw,_hh,this);
        _se->addObject();
    }
    if((_or > _l) && (_ol < _mx) && (_ob > _my) && (_ot < _b))
    {
        if(_sw == NULL)
            _sw = new DQuadTree(_l,_my,_hw,_hh,this);
        _sw->addObject();
    }
}

void DQuadTree::addToList()
{
    DList* ot = NULL;
    if(_oa == A_LIST)
    {
        if(_tailA->object != NULL)
        {
            ot = _tailA;
            _tailA = new DList();
            ot->next = _tailA;
        }
        _tailA->object = _o;
    }
    else
    {
        if(_tailB->object != NULL)
        {
            ot = _tailB;
            _tailB = new DList();
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

bool DQuadTree::overlap(bool BothLists, DUtils::callbackFunctionQuadTree *Callback)
{
    _oc = Callback;
    bool c = false;
    DList* itr;
    if(BothLists)
    {
        _oa = B_LIST;
        if(_headA->object != NULL)
        {
            itr = _headA;
            while(itr != NULL)
            {
                _o = itr->object;
                if(_o->exists && _o->solid() && overlapNode())
                    c = true;
                itr = itr->next;
            }
        }
        _oa = A_LIST;
        if(_headB->object != NULL)
        {
            itr = _headB;
            while(itr != NULL)
            {
                _o = itr->object;
                if(_o->exists && _o->solid())
                {
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
    }
    else
    {
        if(_headA->object != NULL)
        {
            itr = _headA;
            while(itr != NULL)
            {
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

bool DQuadTree::overlapNode(DList *Iterator)
{
    bool c = false;
    DObject* co;
    DList* itr = Iterator;
    if(itr == NULL)
    {
        if(_oa == A_LIST)
            itr = _headA;
        else
            itr = _headB;
    }

    if(itr->object != NULL)
    {
        while(itr != NULL)
        {
            co = itr->object;
            if( (_o == co) || !co->exists || !_o->exists || !co->solid() || !_o->solid() ||
                    (_o->x + _o->width  < co->x + DUtils::roundingError) ||
                    (_o->x + DUtils::roundingError > co->x + co->width) ||
                    (_o->y + _o->height < co->y + DUtils::roundingError) ||
                    (_o->y + DUtils::roundingError > co->y + co->height) )
            {
                itr = itr->next;
                continue;
            }
            if(_oc == NULL)
            {
                _o->kill();
                co->kill();
                c = true;
            }
            else if(_oc(_o,co))
                c = true;
            itr = itr->next;
        }
    }

    return c;
}

}
