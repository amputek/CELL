#include "Dust.hpp"

Dust :: Dust(vec2 aloc, vec2 vel, float d):GameObject(aloc, d){
    velocity = vel;
    radius = rand(4,15) * depth;
    drawRect = Rectf( -radius, -radius, radius, radius );
}

void Dust :: update(){
    float v = pow(length(velocity),2);
    //if(velocity.lengthSquared() > 0.05){    //if velocity is not negligable
    if( v > 0.05 ){
        velocity*=0.9;                      //reduce velocity (friction)
        global+=velocity;                   //velocity affects location
    }
    GameObject::update();
}

void Dust :: addVelocity(vec2 v){
    velocity += v;
}
