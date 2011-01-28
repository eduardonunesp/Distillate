#include <cmath>
#include <cstdlib>
#include "DUtils.hpp"
#include "DGlobals.hpp"
#include "DObject.hpp"
#include "DPoint.hpp"
#include "DQuadTree.hpp"
#include "DRect.hpp"

namespace Distillate
{

int         DUtils::_seed          = 0;
const float DUtils::roundingError  = 0.0000001f;
DQuadTree*  DUtils::quadTree       = NULL;
DRect*      DUtils::quadTreeBounds = NULL;

float DUtils::random(bool UseGlobalSeed)
{
    if(UseGlobalSeed && _seed)
    {
        //TODO: Fix here
        /*
        int random = rand(_seed);
        _seed = mutate(_seed,random);
        return random;
        */
        return 0.0f;
    }
    else
        return rand();
}

float DUtils::computeVelocity(float Velocity, float Acceleration, float Drag, float Max)
{
    if(Acceleration != 0)
        Velocity += Acceleration*DGlobals::elapsed;
    else if(Drag != 0)
    {
        int d = Drag*DGlobals::elapsed;
        if(Velocity - d > 0)
            Velocity -= d;
        else if(Velocity + d < 0)
            Velocity += d;
        else
            Velocity = 0;
    }
    if((Velocity != 0) && (Max != 10000))
    {
        if(Velocity > Max)
            Velocity = Max;
        else if(Velocity < -Max)
            Velocity = -Max;
    }
    return Velocity;
}

DPoint* DUtils::rotatePoint(float X, float Y, float PivotX, float PivotY, float  Angle, DPoint *P)
{
    if(!P) P = new DPoint();
    float radians = -Angle / 180 * M_PI;
    float dx = X-PivotX;
    float dy = PivotY-Y;
    P->x = PivotX + cos(radians)*dx - sin(radians)*dy;
    P->y = PivotY - (sin(radians)*dx + cos(radians)*dy);
    return P;
}


bool DUtils::collide(DObject *Object1, DObject *Object2)
{
    if( (Object1 == NULL) || !Object1->exists ||
            (Object2 == NULL) || !Object2->exists )
        return false;
    quadTree = new DQuadTree(quadTreeBounds->x,quadTreeBounds->y,quadTreeBounds->width,quadTreeBounds->height);
    quadTree->add(Object1,DQuadTree::A_LIST);

    bool match = (Object1 == Object2);
    if(!match) quadTree->add(Object2,DQuadTree::B_LIST);
    bool cx = quadTree->overlap(!match,&solveXCollision);
    bool cy = quadTree->overlap(!match,&solveYCollision);
    return cx || cy;
}

bool DUtils::solveXCollision(DObject* Object1, DObject* Object2)
{
    //Avoid messed up collisions ahead of time
    int o1 = Object1->colVector->x;
    int o2 = Object2->colVector->x;
    if(o1 == o2)
        return false;

    //Give the objects a heads up that we're about to resolve some collisions
    Object1->preCollide(Object2);
    Object2->preCollide(Object1);

    //Basic resolution variables
    int  overlap;
    bool hit = false;
    bool p1hn2;

    //Directional variables
    bool obj1Stopped = (o1 == 0);
    bool obj1MoveNeg = (o1 < 0);
    bool obj1MovePos = (o1 > 0);
    bool obj2Stopped = (o2 == 0);
    bool obj2MoveNeg = (o2 < 0);
    bool obj2MovePos = (o2 > 0);


    //Offset loop variables
    unsigned int i1;
    unsigned int i2;
    DRect* obj1Hull = Object1->colHullX;
    DRect* obj2Hull = Object2->colHullX;
    std::vector<DPoint*> co1 = Object1->colOffsets;
    std::vector<DPoint*> co2 = Object2->colOffsets;
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

    //Decide based on object's movement patterns if it was a right-side or left-side collision
    p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) || //the obvious cases
             (obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) || //both moving left, obj2 overtakes obj1
             (obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) ); //both moving right, obj1 overtakes obj2

    //Check to see if these objects allow these collisions
    if(p1hn2?(!Object1->collideRight || !Object2->collideLeft):(!Object1->collideLeft || !Object2->collideRight))
        return false;

    //this looks insane, but we're just looping through collision offsets on each object
    for(i1 = 0; i1 < l1; i1++)
    {
        ox1 = co1[i1]->x;
        oy1 = co1[i1]->y;
        obj1Hull->x += ox1;
        obj1Hull->y += oy1;
        for(i2 = 0; i2 < l2; i2++)
        {
            ox2 = co2[i2]->x;
            oy2 = co2[i2]->y;
            obj2Hull->x += ox2;
            obj2Hull->y += oy2;

            //See if it's a actually a valid collision
            if( (obj1Hull->x + obj1Hull->width  < obj2Hull->x + roundingError) ||
                    (obj1Hull->x + roundingError > obj2Hull->x + obj2Hull->width) ||
                    (obj1Hull->y + obj1Hull->height < obj2Hull->y + roundingError) ||
                    (obj1Hull->y + roundingError > obj2Hull->y + obj2Hull->height) )
            {
                obj2Hull->x -= ox2;
                obj2Hull->y -= oy2;
                continue;
            }
            //Calculate the overlap between the objects
            if(p1hn2)
            {
                if(obj1MoveNeg)
                    r1 = obj1Hull->x + Object1->colHullY->width;
                else
                    r1 = obj1Hull->x + obj1Hull->width;
                if(obj2MoveNeg)
                    r2 = obj2Hull->x;
                else
                    r2 = obj2Hull->x + obj2Hull->width - Object2->colHullY->width;
            }
            else
            {
                if(obj2MoveNeg)
                    r1 = -obj2Hull->x - Object2->colHullY->width;
                else
                    r1 = -obj2Hull->x - obj2Hull->width;
                if(obj1MoveNeg)
                    r2 = -obj1Hull->x;
                else
                    r2 = -obj1Hull->x - obj1Hull->width + Object1->colHullY->width;
            }
            overlap = r1 - r2;

            //Last chance to skip out on a bogus collision resolution
            if( (overlap == 0) ||
                    ((!Object1->fixed() && ((overlap>0)?overlap:-overlap) > obj1Hull->width*0.8)) ||
                    ((!Object2->fixed() && ((overlap>0)?overlap:-overlap) > obj2Hull->width*0.8)) )
            {
                obj2Hull->x -= ox2;
                obj2Hull->y -= oy2;
                continue;
            }
            hit = true;

            //Adjust the objects according to their flags and stuff
            sv1 = Object2->velocity->x;
            sv2 = Object1->velocity->x;
            if(!Object1->fixed() && Object2->fixed())
            {
                if(Object1->_group)
                    Object1->reset(Object1->x - overlap,Object1->y);
                else
                    Object1->x -= overlap;
            }
            else if(Object1->fixed() && !Object2->fixed())
            {
                if(Object2->_group)
                    Object2->reset(Object2->x + overlap,Object2->y);
                else
                    Object2->x += overlap;
            }
            else if(!Object1->fixed() && !Object2->fixed())
            {
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
            if(p1hn2)
            {
                Object1->hitRight(Object2,sv1);
                Object2->hitLeft(Object1,sv2);
            }
            else
            {
                Object1->hitLeft(Object2,sv1);
                Object2->hitRight(Object1,sv2);
            }

            //Adjust collision hulls if necessary
            if(!Object1->fixed() && (overlap != 0))
            {
                if(p1hn2)
                    obj1Hull->width -= overlap;
                else
                {
                    obj1Hull->x -= overlap;
                    obj1Hull->width += overlap;
                }
                Object1->colHullY->x -= overlap;
            }
            if(!Object2->fixed() && (overlap != 0))
            {
                if(p1hn2)
                {
                    obj2Hull->x += overlap;
                    obj2Hull->width -= overlap;
                }
                else
                    obj2Hull->width += overlap;
                Object2->colHullY->x += overlap;
            }

            obj2Hull->x -= ox2;
            obj2Hull->y -= oy2;
        }

        obj1Hull->x -= ox1;
        obj1Hull->y -= oy1;
    }

    return hit;
}

bool DUtils::solveYCollision(DObject* Object1, DObject* Object2)
{
    //Avoid messed up collisions ahead of time
    int o1 = Object1->colVector->y;
    int o2 = Object2->colVector->y;
    if(o1 == o2)
        return false;

    //Give the objects a heads up that we're about to resolve some collisions
    Object1->preCollide(Object2);
    Object2->preCollide(Object1);

    //Basic resolution variables
    int overlap;
    bool hit = false;
    bool p1hn2;

    //Directional variables
    bool obj1Stopped = (o1 == 0);
    bool obj1MoveNeg = (o1 < 0);
    bool obj1MovePos = (o1 > 0);
    bool obj2Stopped = (o2 == 0);
    bool obj2MoveNeg = (o2 < 0);
    bool obj2MovePos = (o2 > 0);

    //Offset loop variables
    unsigned int i1;
    unsigned int i2;
    DRect* obj1Hull = Object1->colHullY;
    DRect* obj2Hull = Object2->colHullY;
    std::vector<DPoint*> co1 = Object1->colOffsets;
    std::vector<DPoint*> co2 = Object2->colOffsets;
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

    //Decide based on object's movement patterns if it was a top or bottom collision
    p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) || //the obvious cases
             (obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) || //both moving up, obj2 overtakes obj1
             (obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) ); //both moving down, obj1 overtakes obj2

    //Check to see if these objects allow these collisions
    if(p1hn2?(!Object1->collideBottom || !Object2->collideTop):(!Object1->collideTop || !Object2->collideBottom))
        return false;

    //this looks insane, but we're just looping through collision offsets on each object
    for(i1 = 0; i1 < l1; i1++)
    {
        ox1 = co1[i1]->x;
        oy1 = co1[i1]->y;
        obj1Hull->x += ox1;
        obj1Hull->y += oy1;
        for(i2 = 0; i2 < l2; i2++)
        {
            ox2 = co2[i2]->x;
            oy2 = co2[i2]->y;
            obj2Hull->x += ox2;
            obj2Hull->y += oy2;

            //See if it's a actually a valid collision
            if( (obj1Hull->x + obj1Hull->width  < obj2Hull->x + roundingError) ||
                    (obj1Hull->x + roundingError > obj2Hull->x + obj2Hull->width) ||
                    (obj1Hull->y + obj1Hull->height < obj2Hull->y + roundingError) ||
                    (obj1Hull->y + roundingError > obj2Hull->y + obj2Hull->height) )
            {
                obj2Hull->x -= ox2;
                obj2Hull->y -= oy2;
                continue;
            }

            //Calculate the overlap between the objects
            if(p1hn2)
            {
                if(obj1MoveNeg)
                    r1 = obj1Hull->y + Object1->colHullX->height;
                else
                    r1 = obj1Hull->y + obj1Hull->height;
                if(obj2MoveNeg)
                    r2 = obj2Hull->y;
                else
                    r2 = obj2Hull->y + obj2Hull->height - Object2->colHullX->height;
            }
            else
            {
                if(obj2MoveNeg)
                    r1 = -obj2Hull->y - Object2->colHullX->height;
                else
                    r1 = -obj2Hull->y - obj2Hull->height;
                if(obj1MoveNeg)
                    r2 = -obj1Hull->y;
                else
                    r2 = -obj1Hull->y - obj1Hull->height + Object1->colHullX->height;
            }
            overlap = r1 - r2;

            //Last chance to skip out on a bogus collision resolution
            if( (overlap == 0) ||
                    ((!Object1->fixed() && ((overlap>0)?overlap:-overlap) > obj1Hull->height*0.8)) ||
                    ((!Object2->fixed() && ((overlap>0)?overlap:-overlap) > obj2Hull->height*0.8)) )
            {
                obj2Hull->x -= ox2;
                obj2Hull->y -= oy2;
                continue;
            }
            hit = true;

            //Adjust the objects according to their flags and stuff
            sv1 = Object2->velocity->y;
            sv2 = Object1->velocity->y;
            if(!Object1->fixed() && Object2->fixed())
            {
                if(Object1->_group)
                    Object1->reset(Object1->x, Object1->y - overlap);
                else
                    Object1->y -= overlap;
            }
            else if(Object1->fixed() && !Object2->fixed())
            {
                if(Object2->_group)
                    Object2->reset(Object2->x, Object2->y + overlap);
                else
                    Object2->y += overlap;
            }
            else if(!Object1->fixed() && !Object2->fixed())
            {
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
            if(p1hn2)
            {
                Object1->hitBottom(Object2,sv1);
                Object2->hitTop(Object1,sv2);
            }
            else
            {
                Object1->hitTop(Object2,sv1);
                Object2->hitBottom(Object1,sv2);
            }

            //Adjust collision hulls if necessary
            if(!Object1->fixed() && (overlap != 0))
            {
                if(p1hn2)
                {
                    obj1Hull->y -= overlap;

                    //This code helps stuff ride horizontally moving platforms.
                    if(Object2->fixed() && Object2->moves)
                    {
                        sv1 = Object2->colVector->x;
                        Object1->x += sv1;
                        obj1Hull->x += sv1;
                        Object1->colHullX->x += sv1;
                    }
                }
                else
                {
                    obj1Hull->y -= overlap;
                    obj1Hull->height += overlap;
                }
            }
            if(!Object2->fixed() && (overlap != 0))
            {
                if(p1hn2)
                {
                    obj2Hull->y += overlap;
                    obj2Hull->height -= overlap;
                }
                else
                {
                    obj2Hull->height += overlap;

                    //This code helps stuff ride horizontally moving platforms.
                    if(Object1->fixed() && Object1->moves)
                    {
                        sv2 = Object1->colVector->x;
                        Object2->x += sv2;
                        obj2Hull->x += sv2;
                        Object2->colHullX->x += sv2;
                    }
                }
            }
            obj2Hull->x -= ox2;
            obj2Hull->y -= oy2;
        }
        obj1Hull->x -= ox1;
        obj1Hull->y -= oy1;
    }

    return false;
}


}

