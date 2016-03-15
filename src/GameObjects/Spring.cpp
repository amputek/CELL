#include "Spring.hpp"

Spring :: Spring(vec2 loc, float dep, float dmp, float stf, float ms) : Dust(loc, vec2(0,0), dep){
    damping = dmp;
    stiffness = stf;
    mass = ms;
    springContact = false;
}


//Collide with an outside object (generally the player's location)
void Spring :: collide(vec2 heroLoc){
    
    if(dist(heroLoc, global) < collisionDistance){
        vec2 d = heroLoc - global;
        float angle = atan2(d.y, d.x);
        vec2 target = vec2(global.x + cos(angle) * 20, global.y + sin(angle) * 20);
        vec2 accel = (target - heroLoc) * (stiffness * 3);
        
        velocity -= accel;         //increment velocity vector
        springContact = true;      //allows parent to see if spring has been contacted
    } else {
        springContact = false;
    }
}


//Calculating acceleration and velocity from mass, stiffness, and damping.

void Spring :: update(vec2 target){
    vec2 force = (target-global) * stiffness;
    vec2 accel = force / mass;
    velocity = (velocity + accel) * damping;
    Dust::update();
}

void Spring :: addForce(vec2 force){
    vec2 accel = force / mass;
    velocity = (velocity + accel) * damping;
}