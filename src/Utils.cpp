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

#include "include/Utils.hpp"
#include "include/Globals.hpp"
#include "include/Object.hpp"
#include "include/Point.hpp"
#include "include/QuadTree.hpp"
#include "include/Rect.hpp"
#include "include/TextureResource.hpp"
#include "include/ResourceManager.hpp"

#include <cmath>
#include <cstdlib>

NAMESPACE_BEGIN

QuadTree    Utils::quadTree;
Rect        Utils::quadTreeBounds;
int         Utils::_seed          = 0;
const float Utils::roundingError  = 0.0000001f;

float Utils::random(bool UseGlobalSeed)
{
    if(UseGlobalSeed && _seed) {
        return 0.0f;
    }

    return float(rand()) / RAND_MAX;
}

float Utils::computeVelocity(float Velocity, float Acceleration, float Drag, float Max)
{
    if(Acceleration != 0) {
        Velocity += Acceleration*Globals::elapsed;
    } else if(Drag != 0) {
        float d = Drag*Globals::elapsed;
        if(Velocity - d > 0)
            Velocity -= d;
        else if(Velocity + d < 0)
            Velocity += d;
        else
            Velocity = 0;
    }
    if((Velocity != 0) && (Max != 10000)) {
        if(Velocity > Max)
            Velocity = Max;
        else if(Velocity < -Max)
            Velocity = -Max;
    }

    return Velocity;
}

void Utils::setWorldBounds(float X, float Y, float Width, float Height, unsigned int Divisions)
{
    if(!QuadTree::bounds)
        QuadTree::bounds = new Rect();
    QuadTree::bounds->x = X;
    QuadTree::bounds->y = Y;
    if(Width > 0)
        QuadTree::bounds->width = Width;
    if(Height > 0)
        QuadTree::bounds->height = Height;
    if(Divisions > 0)
        QuadTree::divisions = Divisions;
}

bool Utils::overlap(Object *Object1, Object *Object2, callbackFunctionQuadTree *Callback)
{
    return false;
    if((!Object1) || !Object1->exists ||
            (!Object2) || !Object2->exists )
        return false;
    quadTree = QuadTree(QuadTree::bounds->x,QuadTree::bounds->y,QuadTree::bounds->width,QuadTree::bounds->height);
    quadTree.add(Object1, QuadTree::A_LIST);
    if(Object1 == Object2)
        return quadTree.overlap(false,Callback);
    quadTree.add(Object2,QuadTree::B_LIST);
    return quadTree.overlap(true,Callback);
}

void Utils::rotatePoint(float X, float Y, float PivotX, float PivotY, float  Angle, Point *P)
{
    float radians = -Angle / 180 * M_PI;
    float dx = X-PivotX;
    float dy = PivotY-Y;
    P->x = PivotX + cos(radians)*dx - sin(radians)*dy;
    P->y = PivotY - (sin(radians)*dx + cos(radians)*dy);
}

float Utils::getAngle(float X, float Y)
{
    float c1 = 3.14159265 / 4;
    float c2 = 3 * c1;
    float ay = (Y < 0)?-Y:Y;
    float angle = 0;
    if (X >= 0)
        angle = c1 - c1 * ((X - ay) / (X + ay));
    else
        angle = c2 - c1 * ((X + ay) / (ay - X));
    return ((Y < 0)?-angle:angle)*57.2957796;
}

bool Utils::collide(Object *Object1, Object *Object2)
{
    return false;
    if( (Object1 == NULL) || !Object1->exists ||
            (Object2 == NULL) || !Object2->exists )
        return false;
    quadTree = QuadTree(quadTreeBounds.x,quadTreeBounds.y,quadTreeBounds.width,quadTreeBounds.height);
    quadTree.add(Object1,QuadTree::A_LIST);

    bool match = (Object1 == Object2);
    if(!match) quadTree.add(Object2,QuadTree::B_LIST);
    bool cx = quadTree.overlap(!match,&solveXCollision);
    bool cy = quadTree.overlap(!match,&solveYCollision);
    return cx || cy;
}

bool Utils::solveXCollision(Object* Object1, Object* Object2)
{
    int o1 = Object1->colVector.x;
    int o2 = Object2->colVector.x;
    if(o1 == o2)
        return false;

    Object1->preCollide(Object2);
    Object2->preCollide(Object1);

    int  overlap;
    bool hit = false;
    bool p1hn2;

    bool obj1Stopped = (o1 == 0);
    bool obj1MoveNeg = (o1 < 0);
    bool obj1MovePos = (o1 > 0);
    bool obj2Stopped = (o2 == 0);
    bool obj2MoveNeg = (o2 < 0);
    bool obj2MovePos = (o2 > 0);


    unsigned int i1;
    unsigned int i2;
    Rect obj1Hull = Object1->colHullX;
    Rect obj2Hull = Object2->colHullX;
    std::vector<Point*> co1 = Object1->colOffsets;
    std::vector<Point*> co2 = Object2->colOffsets;
    unsigned int l1 = co1.size();
    unsigned int l2 = co2.size();
    int ox1;
    int oy1;
    int ox2;
    int oy2;
    int r1;
    int r2;
    int sv1;
    int sv2;

    p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) ||
            (obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) ||
            (obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) );

    if(p1hn2?(!Object1->collideRight || !Object2->collideLeft):(!Object1->collideLeft || !Object2->collideRight))
        return false;

    for(i1 = 0; i1 < l1; i1++) {
        ox1 = co1[i1]->x;
        oy1 = co1[i1]->y;
        obj1Hull.x += ox1;
        obj1Hull.y += oy1;
        for(i2 = 0; i2 < l2; i2++) {
            ox2 = co2[i2]->x;
            oy2 = co2[i2]->y;
            obj2Hull.x += ox2;
            obj2Hull.y += oy2;

            if( (obj1Hull.x + obj1Hull.width  < obj2Hull.x + roundingError) ||
                    (obj1Hull.x + roundingError > obj2Hull.x + obj2Hull.width) ||
                    (obj1Hull.y + obj1Hull.height < obj2Hull.y + roundingError) ||
                    (obj1Hull.y + roundingError > obj2Hull.y + obj2Hull.height) ) {
                obj2Hull.x -= ox2;
                obj2Hull.y -= oy2;
                continue;
            }

            if(p1hn2) {
                if(obj1MoveNeg)
                    r1 = obj1Hull.x + Object1->colHullY.width;
                else
                    r1 = obj1Hull.x + obj1Hull.width;
                if(obj2MoveNeg)
                    r2 = obj2Hull.x;
                else
                    r2 = obj2Hull.x + obj2Hull.width - Object2->colHullY.width;
            } else {
                if(obj2MoveNeg)
                    r1 = -obj2Hull.x - Object2->colHullY.width;
                else
                    r1 = -obj2Hull.x - obj2Hull.width;
                if(obj1MoveNeg)
                    r2 = -obj1Hull.x;
                else
                    r2 = -obj1Hull.x - obj1Hull.width + Object1->colHullY.width;
            }
            overlap = r1 - r2;

            if( (overlap == 0) ||
                    ((!Object1->fixed() && ((overlap>0)?overlap:-overlap) > obj1Hull.width*0.8)) ||
                    ((!Object2->fixed() && ((overlap>0)?overlap:-overlap) > obj2Hull.width*0.8)) ) {
                obj2Hull.x -= ox2;
                obj2Hull.y -= oy2;
                continue;
            }
            hit = true;

            sv1 = Object2->velocity.x;
            sv2 = Object1->velocity.x;
            if(!Object1->fixed() && Object2->fixed()) {
                if(Object1->_group)
                    Object1->reset(Object1->x - overlap,Object1->y);
                else
                    Object1->x -= overlap;
            } else if(Object1->fixed() && !Object2->fixed()) {
                if(Object2->_group)
                    Object2->reset(Object2->x + overlap,Object2->y);
                else
                    Object2->x += overlap;
            } else if(!Object1->fixed() && !Object2->fixed()) {
                overlap /= 2;
                if(Object1->_group)
                    Object1->reset(Object1->x - overlap,Object1->y);
                else
                    Object1->x -= overlap;
                if(Object2->_group)
                    Object2->reset(Object2->x + overlap,Object2->y);
                else
                    Object2->x += overlap;
                sv1 /= 2;
                sv2 /= 2;
            }
            if(p1hn2) {
                Object1->hitRight(Object2,sv1);
                Object2->hitLeft(Object1,sv2);
            } else {
                Object1->hitLeft(Object2,sv1);
                Object2->hitRight(Object1,sv2);
            }

            if(!Object1->fixed() && (overlap != 0)) {
                if(p1hn2)
                    obj1Hull.width -= overlap;
                else {
                    obj1Hull.x -= overlap;
                    obj1Hull.width += overlap;
                }
                Object1->colHullY.x -= overlap;
            }
            if(!Object2->fixed() && (overlap != 0)) {
                if(p1hn2) {
                    obj2Hull.x += overlap;
                    obj2Hull.width -= overlap;
                } else
                    obj2Hull.width += overlap;
                Object2->colHullY.x += overlap;
            }

            obj2Hull.x -= ox2;
            obj2Hull.y -= oy2;
        }

        obj1Hull.x -= ox1;
        obj1Hull.y -= oy1;
    }

    return hit;
}

bool Utils::solveYCollision(Object* Object1, Object* Object2)
{
    int o1 = Object1->colVector.y;
    int o2 = Object2->colVector.y;
    if(o1 == o2)
        return false;

    Object1->preCollide(Object2);
    Object2->preCollide(Object1);

    int overlap;
    bool hit = false;
    bool p1hn2;

    bool obj1Stopped = (o1 == 0);
    bool obj1MoveNeg = (o1 < 0);
    bool obj1MovePos = (o1 > 0);
    bool obj2Stopped = (o2 == 0);
    bool obj2MoveNeg = (o2 < 0);
    bool obj2MovePos = (o2 > 0);

    unsigned int i1;
    unsigned int i2;
    Rect obj1Hull = Object1->colHullY;
    Rect obj2Hull = Object2->colHullY;
    std::vector<Point*> co1 = Object1->colOffsets;
    std::vector<Point*> co2 = Object2->colOffsets;
    unsigned int l1 = co1.size();
    unsigned int l2 = co2.size();
    int ox1;
    int oy1;
    int ox2;
    int oy2;
    int r1 ;
    int r2 ;
    int sv1;
    int sv2;

    p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) ||
            (obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) ||
            (obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) );

    if(p1hn2?(!Object1->collideBottom || !Object2->collideTop):(!Object1->collideTop || !Object2->collideBottom))
        return false;

    for(i1 = 0; i1 < l1; i1++) {
        ox1 = co1[i1]->x;
        oy1 = co1[i1]->y;
        obj1Hull.x += ox1;
        obj1Hull.y += oy1;
        for(i2 = 0; i2 < l2; i2++) {
            ox2 = co2[i2]->x;
            oy2 = co2[i2]->y;
            obj2Hull.x += ox2;
            obj2Hull.y += oy2;

            if( (obj1Hull.x + obj1Hull.width  < obj2Hull.x + roundingError) ||
                    (obj1Hull.x + roundingError > obj2Hull.x + obj2Hull.width) ||
                    (obj1Hull.y + obj1Hull.height < obj2Hull.y + roundingError) ||
                    (obj1Hull.y + roundingError > obj2Hull.y + obj2Hull.height) ) {
                obj2Hull.x -= ox2;
                obj2Hull.y -= oy2;
                continue;
            }

            if(p1hn2) {
                if(obj1MoveNeg)
                    r1 = obj1Hull.y + Object1->colHullX.height;
                else
                    r1 = obj1Hull.y + obj1Hull.height;
                if(obj2MoveNeg)
                    r2 = obj2Hull.y;
                else
                    r2 = obj2Hull.y + obj2Hull.height - Object2->colHullX.height;
            } else {
                if(obj2MoveNeg)
                    r1 = -obj2Hull.y - Object2->colHullX.height;
                else
                    r1 = -obj2Hull.y - obj2Hull.height;
                if(obj1MoveNeg)
                    r2 = -obj1Hull.y;
                else
                    r2 = -obj1Hull.y - obj1Hull.height + Object1->colHullX.height;
            }
            overlap = r1 - r2;

            if( (overlap == 0) ||
                    ((!Object1->fixed() && ((overlap>0)?overlap:-overlap) > obj1Hull.height*0.8)) ||
                    ((!Object2->fixed() && ((overlap>0)?overlap:-overlap) > obj2Hull.height*0.8)) ) {
                obj2Hull.x -= ox2;
                obj2Hull.y -= oy2;
                continue;
            }
            hit = true;

            sv1 = Object2->velocity.y;
            sv2 = Object1->velocity.y;
            if(!Object1->fixed() && Object2->fixed()) {
                if(Object1->_group)
                    Object1->reset(Object1->x, Object1->y - overlap);
                else
                    Object1->y -= overlap;
            } else if(Object1->fixed() && !Object2->fixed()) {
                if(Object2->_group)
                    Object2->reset(Object2->x, Object2->y + overlap);
                else
                    Object2->y += overlap;
            } else if(!Object1->fixed() && !Object2->fixed()) {
                overlap /= 2;
                if(Object1->_group)
                    Object1->reset(Object1->x, Object1->y - overlap);
                else
                    Object1->y -= overlap;
                if(Object2->_group)
                    Object2->reset(Object2->x, Object2->y + overlap);
                else
                    Object2->y += overlap;
                sv1 /= 2;
                sv2 /= 2;
            }
            if(p1hn2) {
                Object1->hitBottom(Object2,sv1);
                Object2->hitTop(Object1,sv2);
            } else {
                Object1->hitTop(Object2,sv1);
                Object2->hitBottom(Object1,sv2);
            }

            if(!Object1->fixed() && (overlap != 0)) {
                if(p1hn2) {
                    obj1Hull.y -= overlap;

                    if(Object2->fixed() && Object2->moves) {
                        sv1 = Object2->colVector.x;
                        Object1->x += sv1;
                        obj1Hull.x += sv1;
                        Object1->colHullX.x += sv1;
                    }
                } else {
                    obj1Hull.y -= overlap;
                    obj1Hull.height += overlap;
                }
            }
            if(!Object2->fixed() && (overlap != 0)) {
                if(p1hn2) {
                    obj2Hull.y += overlap;
                    obj2Hull.height -= overlap;
                } else {
                    obj2Hull.height += overlap;

                    if(Object1->fixed() && Object1->moves) {
                        sv2 = Object1->colVector.x;
                        Object2->x += sv2;
                        obj2Hull.x += sv2;
                        Object2->colHullX.x += sv2;
                    }
                }
            }
            obj2Hull.x -= ox2;
            obj2Hull.y -= oy2;
        }
        obj1Hull.x -= ox1;
        obj1Hull.y -= oy1;
    }

    return hit;
}

NAMESPACE_END
