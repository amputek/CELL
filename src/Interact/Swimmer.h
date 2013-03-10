//
//  Swimmer.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Swimmer__
#define __Cellv0__Swimmer__

#include <iostream>
#include "Braitenberg.h"

using namespace std;

class Swimmer : public Braitenberg{
    
public:
    Swimmer(Vec2f loc);
    void update();
    void updateTarget(vector<GameObject*> movers);
    void setDestination(Vec2f dest);
    //bool inSpace();
    
    bool inSpace;
    
private:
    bool checkInSpace(Vec2f loc, vector<GameObject*> movers);
    Vec2f targetLocation;
    Vec2f targetDestination;
};



#endif /* defined(__Cellv0__Swimmer__) */
