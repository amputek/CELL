#include "Finite.hpp"

Finite :: Finite(vec2 loc, float l, float r):GameObject(loc,1){
    maxLife = l;
    currentLife = 0.0f;
    radius = r;
}

//secondary constructor allows for direction to be specified
Finite :: Finite(vec2 loc, float l, float r, float d):GameObject(loc,d){
    maxLife = l;
    currentLife = 0.0f;
    direction = d;
    radius = r;
}

void Finite :: update(){
    currentLife += deltaTime * 60.0f;
    GameObject::update();
}
