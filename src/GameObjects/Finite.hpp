#ifndef Finite_hpp
#define Finite_hpp

#include <iostream>
#include "GameObject.hpp"

class Finite : public GameObject{
public:
    Finite(vec2 loc, float l);
    Finite(vec2 loc, float l, float d);
    void update();
    bool alive(){ return time <= life; };
    
    float direction;
    int time;
    int life;
    
};

#endif
