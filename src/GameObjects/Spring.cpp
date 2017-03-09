#include "Spring.hpp"

Spring :: Spring(vec2 loc, float depth, float stf, float ms, float damp ) : Dust(loc, 1, 1, damp){
    stiffness = stf;
    mass = ms;
    springContact = false;
}


//Collide with an outside object (generally the player's location)
void Spring :: collide( const vec2 & colliderPosition, float collideDist ){
 
    if(dist(colliderPosition, mPosition) < collideDist){
        vec2 d = colliderPosition - mPosition;
        float angle = atan2(d.y, d.x);
        vec2 target = vec2(colliderPosition.x - cos(angle) * collideDist, colliderPosition.y - sin(angle) * collideDist);
        vec2 force = (target - mPosition) * stiffness * 3.0f;
        addForce( force );
        
        mPosition = target;
        
        springContact = true;      //allows parent to see if spring has been contacted
    } else {
        springContact = false;
    }
}



//Calculating acceleration and velocity from mass, stiffness, and damping.

void Spring :: moveTowards(vec2 targetToMoveTowards ){
    vec2 force = (targetToMoveTowards - mPosition) * stiffness;
    addForce( force );
}


void Spring :: addForce(vec2 force){
    vec2 accel = force / mass;
    addVelocity( accel * deltaTime * 60.0f );
    
}
