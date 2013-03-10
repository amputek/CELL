#include "Finite.h"

Finite :: Finite(Vec2f loc, float l):GameObject(loc,1){
    life = l;
    time = 0;
}

Finite :: Finite(Vec2f loc, float l, float d):GameObject(loc,1){
    life = l;
    time = 0;
    direction = d;
}

void Finite :: update(){
    time++;
    GameObject::update();
}