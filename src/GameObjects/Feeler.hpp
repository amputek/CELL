#ifndef Feeler_hpp
#define Feeler_hpp

#include <iostream>
#include "Spring.hpp"

using namespace ci;

class Feeler {
    
public:
    Feeler();
    Feeler(vec2 loc, float depth, int joints, float baseWidth, float tipWidth );

    ~Feeler(){
        for( vector<Spring*>::iterator p = springs.begin(); p != springs.end(); ++p){
            delete *p;
        }
    }
    
    vec2 getPosition()
    {
        return springs.at(0)->getPosition();
    }
    
    void setHomePosition( vec2 homePosition )
    {
        if(springs.size() > 0){
            springs.at(0)->setPosition( homePosition );
        }
    }
    
    void update();
    void collide(const vec2 & heroLoc, float colliderSize);
    void addForce(vec2 force);
    bool feelTowards( const vec2 & colliderPos, float minDist, float force );
    
    vector<vec2> getPoints(){
        vector<vec2> positions;
        for(int i = 0; i < springs.size(); i++)
            positions.push_back( springs.at(i)->getPosition() );
        return positions;
    };
    
    //getters
    bool inContactWithCollider(){ return feelerContact; };

    vector<vec2> drawPositions;
    
    vector<Spring*> springs;

    
private:
   
    void updateDrawPositions();
   
    bool feelerContact;
  

    
    float baseWidth = 4.0f;
    float tipWidth = 0.2f;

    
};

#endif
