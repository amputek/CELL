#ifndef Dust_hpp
#define Dust_hpp

#include <iostream>
#include "GameObject.hpp"

class PhysicsObject : public GameObject, public IDynamic{

public:

    PhysicsObject(vec2 position, float depth, float rad, float damp, vec2 startVelocity ) : GameObject(position, depth, rad){
        mVelocity = startVelocity;
        mDamping = damp;
    }
    
    PhysicsObject(vec2 position, float depth, float rad, float damp) : PhysicsObject(position, depth, rad, damp, vec2(0,0) ){
    
    }
    
    ~PhysicsObject(){}
    
    //Implementing IDynamic update
    void update(){
        
        //reduce velocity by damping amount (friction)
        float velMod = (1.0 - (globals::deltaTime * mDamping));
        mVelocity *= velMod;
        
        //velocity affects location
        mPosition += mVelocity * globals::deltaTime;
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
