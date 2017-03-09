#include "Finite.hpp"

Finite :: Finite(vec2 loc, float lifeLength, float r):GameObject(loc,1,r){
    maxLife = lifeLength;
    currentLife = 0.0f;
}

//secondary constructor allows for direction to be specified
Finite :: Finite(vec2 loc, float lifeLength, float r, float d):GameObject(loc,d,r){
    maxLife = lifeLength;
    currentLife = 0.0f;
    direction = d;
}

void Finite :: update(){
    currentLife += deltaTime * 60.0f;
}
