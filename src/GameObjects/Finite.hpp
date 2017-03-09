#ifndef Finite_hpp
#define Finite_hpp

#include <iostream>
#include "GameObject.hpp"

class Finite : public GameObject{
public:
    Finite(vec2 loc, float l, float r);
    Finite(vec2 loc, float l, float r, float d);
    void update();
    bool alive(){ return currentLife <= maxLife; };
    
    
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
