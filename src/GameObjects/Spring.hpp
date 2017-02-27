
//some of the code in this class is based on algorithms found on the Processing website, from an example sketch by Casey Reas and Ben Fry
//Sketch URL: http://processing.org/learning/topics/chain.html
// For the most part, this involves using damping, mass, and stiffness values to calculate acceleration and velocity.
// My extensions of this sketch are: collisions and adding further exeternal forces.

#ifndef Spring_hpp
#define Spring_hpp

#include "Dust.hpp"

using namespace std;

class Spring : public Dust{
    
public:
    Spring(vec2 loc, float stf, float ms, float damp, float depth );
    Spring(vec2 loc, float stf, float ms, float damp) : Spring(loc, stf, ms, damp, 1.0f){ };
    void draw();
    void moveTowards(vec2 target);
    void collide(const vec2 & heroLoc, float colDist);
    void addForce(vec2 force);
    
    //getters
    bool contact(){ return springContact; };
    
private:
    float mass;
    float stiffness;
    bool springContact;
};

#endif
