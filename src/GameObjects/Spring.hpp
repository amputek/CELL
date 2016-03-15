
//some of the code in this class is based on algorithms found on the Processing website, from an example sketch by Casey Reas and Ben Fry
//Sketch URL: http://processing.org/learning/topics/chain.html
// For the most part, this involves using damping, mass, and stiffness values to calculate acceleration and velocity.
// My extensions of this sketch are: collisions and adding further exeternal forces.

#ifndef Spring_hpp
#define Spring_hpp

#include <iostream>
#include "Dust.hpp"

using namespace std;

class Spring : public Dust{
    
public:
    Spring(vec2 loc, float dep, float dmp, float stf, float ms);
    void draw();
    void update(vec2 target);
    void collide(vec2 heroLoc);
    void addForce(vec2 force);
    
    //getters
    bool contact(){ return springContact; };
    
private:
    float damping;
    float mass;
    float stiffness;
    bool springContact;
    const int collisionDistance = 20;
};

#endif
