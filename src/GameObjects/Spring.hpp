
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
    Spring(vec2 startLocation, float depth, float stiffness, float mass, float damping );
    void draw();
    void moveTowards(vec2 target);
    void setPosition(vec2 target){
        mPosition = target;
    }
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
