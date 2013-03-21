#include "Dust.h"

Dust :: Dust(Vec2f aloc, Vec2f vel, float d):GameObject(aloc, d){
    velocity = vel;
    radius = rand(2,10) * depth;
}

void Dust :: update(){
    if(velocity.lengthSquared() > 0.05){    //if velocity is not negligable
        velocity*=0.9;                      //reduce velocity
        global+=velocity;                   //velocity affects location
    }
    GameObject::update();
}

void Dust :: addVelocity(Vec2f v){
    velocity += v;
}