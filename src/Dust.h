//
//  Dust.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Dust__
#define __Cellv0__Dust__

#include <iostream>
#include "GameObject.h"

class Dust : public GameObject{
public:
    Dust(Vec2f aloc, Vec2f vel, float d);
    Vec2f velocity;
    void draw();
    void update();
    bool alive();
    void addVelocity(Vec2f v);

private:
    int opacity;
    
};

#endif /* defined(__Cellv0__Dust__) */
