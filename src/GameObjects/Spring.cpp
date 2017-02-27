#include "Spring.hpp"

Spring :: Spring(vec2 loc, float stf, float ms, float damp, float depth ) : Dust(loc, vec2(0,0), damp, depth){
    stiffness = stf;
    mass = ms;
    springContact = false;
}


//Collide with an outside object (generally the player's location)
void Spring :: collide(const vec2 & heroLoc, float collideDist ){
 
    if(dist(heroLoc, global) < collideDist){
        vec2 d = heroLoc - global;
        float angle = atan2(d.y, d.x);
        vec2 target = vec2(heroLoc.x - cos(angle) * collideDist, heroLoc.y - sin(angle) * collideDist);
        vec2 force = (target - global) * stiffness * 3.0f;
        addForce( force );
        
        global = target;
        
        springContact = true;      //allows parent to see if spring has been contacted
    } else {
        springContact = false;
    }
}



//Calculating acceleration and velocity from mass, stiffness, and damping.

void Spring :: moveTowards(vec2 targetToMoveTowards ){
    vec2 force = (targetToMoveTowards - global) * stiffness;
    addForce( force );
    Dust::update();
}


void Spring :: addForce(vec2 force){
    vec2 accel = force / mass;
    velocity += accel * deltaTime * 60.0f;
    
}
