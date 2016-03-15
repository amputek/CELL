#ifndef Dust_hpp
#define Dust_hpp

#include <iostream>
#include "GameObject.hpp"

class Dust : public GameObject{
public:
    Dust(vec2 aloc, vec2 vel, float d);
    vec2 velocity;
    void draw();
    void update();
    bool alive();
    void addVelocity(vec2 v);

private:
    int opacity;
    
};

#endif
