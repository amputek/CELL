
//some of the code in this class is based on algorithms found on the Processing website, from an example sketch by Casey Reas and Ben Fry
//Sketch URL: http://processing.org/learning/topics/chain.html
// For the most part, this involves using damping, mass, and stiffness values to calculate acceleration and velocity.
// My extensions of this sketch are: collisions and adding further exeternal forces.

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
    
    //getters
    bool contact(){ return springContact; };
    
private:
    float damping;
    float mass;
    float stiffness;
    bool springContact;
    const int collisionDistance = 20;
};

#endif /* defined(__Cellv0__Spring__) */
