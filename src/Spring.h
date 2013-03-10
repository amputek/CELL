//
//  Spring.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Spring__
#define __Cellv0__Spring__

#include <iostream>
#include "Dust.h"

using namespace std;

class Spring : public Dust{
    
public:
    Spring(Vec2f loc, float dep, float dmp, float stf, float ms);
    void draw();
    void update(Vec2f target);
    void collide(Vec2f heroLoc);
    void addForce(Vec2f force);
    bool contact(){ return springContact; };
    
private:
    float damping;
    float mass;
    float stiffness;
    bool springContact;
};

#endif /* defined(__Cellv0__Spring__) */
