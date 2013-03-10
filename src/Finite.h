//
//  Finite.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Finite__
#define __Cellv0__Finite__

#include <iostream>
#include "GameObject.h"

class Finite : public GameObject{
public:
    Finite(Vec2f loc, float l);
    Finite(Vec2f loc, float l, float d);
    void update();
    bool alive(){ return time <= life; };
    
    float direction;
    int time;
    int life;
    
};

#endif /* defined(__Cellv0__Finite__) */
