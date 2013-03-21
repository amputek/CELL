// Follows braitenberg vehicle type movement
// Has two sensors which affect two motors
//
//

#ifndef __Cellv0__Braitenberg__
#define __Cellv0__Braitenberg__

#include <iostream>
#include "GameOBject.h"

using namespace std;

class Braitenberg : public GameObject{
    
public:
    Braitenberg(Vec2f loc, bool sl);
    void moveTo(Vec2f loc);

    float direction;
    float speed;
    
private:
    float lm, rm;
    const float turnLimit = 0.3; //0.5
    const float turnSpeed = 0.05; //0.3
    const float speedLimit = 10.0;
    bool slows;
};

#endif /* defined(__Cellv0__Braitenberg__) */
