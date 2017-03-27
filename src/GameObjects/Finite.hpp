#ifndef Finite_hpp
#define Finite_hpp

#include <iostream>
#include "GameObject.hpp"

class Finite : public GameObject, public IDynamic{
public:

    
    Finite(vec2 loc, float lifeLength, float r):GameObject(loc,1,r){
        maxLife = lifeLength;
        currentLife = 0.0f;
    }
    
    //secondary constructor allows for direction to be specified
    Finite(vec2 loc, float lifeLength, float r, float d):GameObject(loc,d,r){
        maxLife = lifeLength;
        currentLife = 0.0f;
        direction = d;
    }
    
    void update(){
        currentLife += deltaTime * 60.0f;
        if( currentLife > maxLife ) mDeleteMe = true;
    }

    float lifeRatio()
    {
        return (currentLife / maxLife);
    }
    
protected:
    float direction;
    float currentLife = 0.0f;
    float maxLife = 10.0f;
};

#endif
