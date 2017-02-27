#include "Dust.hpp"

Dust :: Dust(vec2 aloc, vec2 vel, float damp, float d):GameObject(aloc, d){
    velocity = vel;
    damping = damp;
}

void Dust :: update(){
    
    //reduce velocity by damping amount (friction)
    float velMod = (1.0 - (deltaTime * damping));
    velocity *= velMod;
    
    //velocity affects location
    global += velocity * deltaTime;

    GameObject::update();
}

void Dust :: addVelocity(vec2 v){
    velocity += v;
}
