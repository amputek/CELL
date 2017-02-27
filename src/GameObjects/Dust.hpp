#ifndef Dust_hpp
#define Dust_hpp

#include <iostream>
#include "GameObject.hpp"

class Dust : public GameObject{
public:
    Dust(vec2 aloc, vec2 vel, float damp, float d);
    vec2 velocity;
    void update();
    void addVelocity(vec2 v);


private:
    int opacity;
    float damping = 4.0f;
    
};

#endif
