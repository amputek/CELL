#ifndef Feeler_hpp
#define Feeler_hpp

#include <iostream>
#include "Spring.hpp"

using namespace ci;

class Feeler {
    
public:
    Feeler();
    Feeler(vec2 loc, float depth, int joints, float baseWidth, float tipWidth, float stiff );
    Feeler(vec2 loc, float depth, int joints, float baseWidth, float tipWidth )
        : Feeler( loc, depth, joints, baseWidth, tipWidth, 2.0f ){ }
    Feeler(vec2 loc, float depth, int joints )
        : Feeler( loc, depth, joints, 4.0f, 0.2f, 2.0f ){ }

    ~Feeler(){
        for( vector<SpringyObject*>::iterator p = springs.begin(); p != springs.end(); ++p){
            delete *p;
        }
    }
    
    vec2 getPosition() const
    {
        return springs.at(0)->getPosition();
    }
    
    void setHomePosition( vec2 homePosition )
    {
        if(springs.size() > 0){
            springs.at(0)->setPosition( homePosition );
        }
    }
    
    FeelerStruct getDrawStruct()
    {
        return FeelerStruct( positions, tipWidth, baseWidth);
    }
    
    void update();
    bool collide(const vec2 & entityPosition, float colliderSize);
    void addForce(const vec2 & force);
    bool feelTowards( const vec2 & colliderPos, float minDist, float force );
    
private:

    
  
    vector<SpringyObject*> springs;
    vector<vec2> positions;
    
    float baseWidth = 4.0f;
    float tipWidth = 0.2f;


    
};




#endif
