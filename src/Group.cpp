#include "include/Group.hpp"
#include "include/Point.hpp"
#include "include/Utils.hpp"
#include <algorithm>

NAMESPACE_BEGIN

Group::Group():
    _first(true)
{
    _group = true;
    _solid = false;
}

Group::~Group()
{
}

Object* Group::add(Object* Obj, bool ShareScroll)
{
    pointer_values_equal<Object*> eq = { Obj };
    if(std::find_if(members.begin(), members.end(), eq) == members.end())
        members.push_back(Obj);
    return Obj;
}

Object* Group::replace(Object* OldObject, Object* NewObject)
{
    pointer_values_equal<Object*> eq = { OldObject };
    std::vector<Object*>::const_iterator it;
    it = std::find_if(members.begin(), members.end(), eq);
    if(it == members.end())
        return NULL;
    std::replace(members.begin(), members.end(), OldObject, NewObject);
    return NewObject;
}

Object* Group::remove(Object* Obj, bool Splice)
{
    pointer_values_equal<Object*> eq = { Obj };
    std::vector<Object*>::iterator it;
    it = std::find_if(members.begin(), members.end(), eq);
    if(it == members.end())
        return NULL;
    members.erase(it);
    return Obj;
}

void Group::sort(SortType Order)
{
    //TODO: Must doing a sort here
}

Object* Group::getFirstAvail()
{
    unsigned int i=0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o && o->exists)
            return o;
    }
    return NULL;
}

int Group::getFirstNull()
{
    unsigned int i=0;
    while(i < members.size()) {
        if(!members[i])
            return i;
        else
            i++;
    }
    return -1;
}

bool Group::resetFirstAvail(float X, float Y)
{
    Object* o = getFirstAvail();
    if(!o)
        return false;
    o->reset(X,Y);
    return true;
}

Object* Group::getFirstExtant()
{
    Object* o = NULL;
    unsigned int i=0;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o && o->exists)
            return o;
    }
    return NULL;
}

Object* Group::getFirstAlive()
{
    Object* o = NULL;
    unsigned int i=0;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o && o->exists && !o->dead)
            return o;
    }
    return NULL;
}

Object* Group::getFirstDead()
{
    Object* o = NULL;
    unsigned int i=0;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o && o->dead)
            return o;
    }
    return NULL;
}

int Group::countLiving()
{
    int count =-1;
    unsigned int i =0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o) {
            if(count < 0)
                count = 0;
            if(o->exists && !o->dead)
                count++;
        }
    }
    return count;
}

int Group::countDead()
{
    int count =-1;
    unsigned int i =0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o) {
            if(count < 0)
                count = 0;
            if(o->dead)
                count++;
        }
    }
    return count;
}

int Group::countOnScreen()
{
    int count =-1;
    unsigned int i =0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o) {
            if(count < 0)
                count = 0;
            if(o->onScreen())
                count++;
        }
    }
    return count;
}

Object* Group::getRandom()
{
    unsigned int c=0;
    Object* o = NULL;
    unsigned int i= Utils::random()*members.size();
    while(!o && i < members.size()) {
        o = (Object*) members[++i%members.size()];
        c++;
    }
    return o;
}

void Group::saveOldPosition()
{
    if(_first) {
        _first = false;
        _last.x = 0;
        _last.y = 0;
        return;
    }
    _last.x = x;
    _last.y = y;
}

void Group::updateMembers()
{
    float mx;
    float my;
    bool moved = false;

    if((x != _last.x) || (y != _last.y)) {
        moved = true;
        mx = x - _last.x;
        my = y - _last.y;
    }

    unsigned int i=0;
    Object* o = NULL;
    unsigned int ml = members.size();

    while (i < ml) {
        o = (Object*) members[i++];
        if((o) && o->exists) {
            if(moved) {
                if(o->_group)
                    o->reset(o->x+mx, o->y+my);
                else {
                    o->x += mx;
                    o->y += my;
                }
            }

            if(o->active)
                o->update();

            if(moved && o->_solid) {
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

void Group::update()
{
    saveOldPosition();
    updateMotion();
    updateMembers();
    updateFlickering();
}

void Group::renderMembers()
{
    unsigned int i=0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if((o) && o->exists && o->visible)
            o->render();
    }
}

void Group::render()
{
    renderMembers();
}

void Group::killMembers()
{
    unsigned int i=0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o)
            o->kill();
    }
}

void Group::kill()
{
    killMembers();
    Object::kill();
}

void Group::destroyMembers()
{
    unsigned int i=0;
    Object* o = NULL;
    while(i < members.size()) {
        o = (Object*) members[i++];
        if(o)
            o->destroy();
    }
}

void Group::destroy()
{
    destroyMembers();
    Object::destroy();
}

void Group::reset(float X, float Y)
{
    saveOldPosition();
    Object::reset(X,Y);
    float mx;
    float my;
    bool moved = false;

    if((x != _last.x) || (y != _last.y)) {
        moved = true;
        mx = x - _last.x;
        my = y - _last.y;
    }

    unsigned int i=0;
    Object* o = NULL;
    unsigned int ml = members.size();

    while (i < ml) {
        o = (Object*) members[i++];
        if((o) && o->exists) {
            if(moved) {
                if(o->_group)
                    o->reset(o->x+mx, o->y+my);
                else {
                    o->x += mx;
                    o->y += my;
                    if(_solid) {
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

int Group::sortHandler(Object* Obj1, Object* Obj2)
{
    //TODO: I'm lazy, one day
    return 0;
}

NAMESPACE_END
