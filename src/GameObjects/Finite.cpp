#include "Finite.hpp"

Finite :: Finite(vec2 loc, float l):GameObject(loc,1){
    life = l;
    time = 0;
}

//secondary constructor allows for direction to be specified
Finite :: Finite(vec2 loc, float l, float d):GameObject(loc,1){
    life = l;
    time = 0;
    direction = d;
}

void Finite :: update(){
    time++;
    GameObject::update();
}