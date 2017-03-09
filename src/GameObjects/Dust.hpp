#ifndef Dust_hpp
#define Dust_hpp

#include <iostream>
#include "GameObject.hpp"

class Dust : public GameObject{

public:

    Dust(vec2 position, float depth, float rad, float damp, vec2 startVelocity ) : GameObject(position, depth, rad){
        mVelocity = startVelocity;
        mDamping = damp;
    }
    
    Dust(vec2 position, float depth, float rad, float damp) : Dust(position, depth, rad, damp, vec2(0,0) ){
    
    }
    
    void update(){
        
        //reduce velocity by damping amount (friction)
        float velMod = (1.0 - (deltaTime * mDamping));
        mVelocity *= velMod;
        
        //velocity affects location
        mPosition += mVelocity * deltaTime;
    }
    
    void addVelocity(vec2 v)
    {
        mVelocity += v;
    }


private:
    vec2 mVelocity;
    float mDamping = 4.0f;
};

#endif
