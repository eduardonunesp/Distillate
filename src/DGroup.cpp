#include "include/DGroup.hpp"
#include <algorithm>
#include "include/DPoint.hpp"
#include "include/DUtils.hpp"

namespace Distillate
{

DGroup::DGroup():
    _first(true)
{
    _group = true;
    _solid = false;
}

DGroup::~DGroup()
{
}

DObject* DGroup::add(DObject* Object, bool ShareScroll)
{
    pointer_values_equal<DObject*> eq = { Object };
    if(std::find_if(members.begin(), members.end(), eq) == members.end())
        members.push_back(Object);
    return Object;
}

DObject* DGroup::replace(DObject* OldObject, DObject* NewObject)
{
    pointer_values_equal<DObject*> eq = { OldObject };
    std::vector<DObject*>::const_iterator it;
    it = std::find_if(members.begin(), members.end(), eq);
    if(it == members.end())
        return NULL;
    std::replace(members.begin(), members.end(), OldObject, NewObject);
    return NewObject;
}

DObject* DGroup::remove(DObject* Object, bool Splice)
{
    pointer_values_equal<DObject*> eq = { Object };
    std::vector<DObject*>::iterator it;
    it = std::find_if(members.begin(), members.end(), eq);
    if(it == members.end())
        return NULL;
    members.erase(it);
    return Object;
}

void DGroup::sort(SortType Order)
{
    //TODO: Must doing a sort here
}

DObject* DGroup::getFirstAvail()
{
    unsigned int i=0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o && o->exists)
            return o;
    }
    return NULL;
}

int DGroup::getFirstNull()
{
    unsigned int i=0;
    while(i < members.size())
    {
        if(!members[i])
            return i;
        else
            i++;
    }
    return -1;
}

bool DGroup::resetFirstAvail(float X, float Y)
{
    DObject* o = getFirstAvail();
    if(!o)
        return false;
    o->reset(X,Y);
    return true;
}

DObject* DGroup::getFirstExtant()
{
    DObject* o = NULL;
    unsigned int i=0;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o && o->exists)
            return o;
    }
    return NULL;
}

DObject* DGroup::getFirstAlive()
{
    DObject* o = NULL;
    unsigned int i=0;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o && o->exists && !o->dead)
            return o;
    }
    return NULL;
}

DObject* DGroup::getFirstDead()
{
    DObject* o = NULL;
    unsigned int i=0;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o && o->dead)
            return o;
    }
    return NULL;
}

int DGroup::countLiving()
{
    int count =-1;
    unsigned int i =0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o)
        {
            if(count < 0)
                count = 0;
            if(o->exists && !o->dead)
                count++;
        }
    }
    return count;
}

int DGroup::countDead()
{
    int count =-1;
    unsigned int i =0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o)
        {
            if(count < 0)
                count = 0;
            if(o->dead)
                count++;
        }
    }
    return count;
}

int DGroup::countOnScreen()
{
    int count =-1;
    unsigned int i =0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o)
        {
            if(count < 0)
                count = 0;
            if(o->onScreen())
                count++;
        }
    }
    return count;
}

DObject* DGroup::getRandom()
{
    unsigned int c=0;
    DObject* o = NULL;
    unsigned int i= DUtils::random()*members.size();
    while(!o && i < members.size())
    {
        o = (DObject*) members[++i%members.size()];
        c++;
    }
    return o;
}

void DGroup::saveOldPosition()
{
    if(_first)
    {
        _first = false;
        _last.x = 0;
        _last.y = 0;
        return;
    }
    _last.x = x;
    _last.y = y;
}

void DGroup::updateMembers()
{
    float mx;
    float my;
    bool moved = false;

    if((x != _last.x) || (y != _last.y))
    {
        moved = true;
        mx = x - _last.x;
        my = y - _last.y;
    }

    unsigned int i=0;
    DObject* o = NULL;
    unsigned int ml = members.size();

    while (i < ml)
    {
        o = (DObject*) members[i++];
        if((o) && o->exists)
        {
            if(moved)
            {
                if(o->_group)
                    o->reset(o->x+mx, o->y+my);
                else
                {
                    o->x += mx;
                    o->y += my;
                }
            }

            if(o->active)
                o->update();

            if(moved && o->_solid)
            {
                o->colHullX.width += ((mx>0) ? mx : -mx);
                if(mx < 0)
                    o->colHullX.x += mx;
                o->colHullY.x = x;
                o->colHullY.height += ((my>0) ? my : -my);
                if(my < 0)
                    o->colHullY.y += mx;
                o->colVector.x += mx;
                o->colVector.y += my;
            }
        }
    }
}

void DGroup::update()
{
    saveOldPosition();
    updateMotion();
    updateMembers();
    updateFlickering();
}

void DGroup::renderMembers()
{
    unsigned int i=0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if((o) && o->exists && o->visible)
            o->render();
    }
}

void DGroup::render()
{
    renderMembers();
}

void DGroup::killMembers()
{
    unsigned int i=0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o)
            o->kill();
    }
}

void DGroup::kill()
{
    killMembers();
    DObject::kill();
}

void DGroup::destroyMembers()
{
    unsigned int i=0;
    DObject* o = NULL;
    while(i < members.size())
    {
        o = (DObject*) members[i++];
        if(o)
            o->destroy();
    }
}

void DGroup::destroy()
{
    destroyMembers();
    DObject::destroy();
}

void DGroup::reset(float X, float Y)
{
    saveOldPosition();
    DObject::reset(X,Y);
    float mx;
    float my;
    bool moved = false;

    if((x != _last.x) || (y != _last.y))
    {
        moved = true;
        mx = x - _last.x;
        my = y - _last.y;
    }

    unsigned int i=0;
    DObject* o = NULL;
    unsigned int ml = members.size();

    while (i < ml)
    {
        o = (DObject*) members[i++];
        if((o) && o->exists)
        {
            if(moved)
            {
                if(o->_group)
                    o->reset(o->x+mx, o->y+my);
                else
                {
                    o->x += mx;
                    o->y += my;
                    if(_solid)
                    {
                        o->colHullX.width += ((mx>0) ? mx : -mx);
                        if(mx < 0)
                            o->colHullX.x += mx;
                        o->colHullY.x = x;
                        o->colHullY.height += ((my>0) ? my : -my);
                        if(my < 0)
                            o->colHullY.y += mx;
                        o->colVector.x += mx;
                        o->colVector.y += my;
                    }
                }
            }
        }
    }
}

int DGroup::sortHandler(DObject* Obj1, DObject* Obj2)
{
    //TODO: I'm lazy, one day
    return 0;
}

}
