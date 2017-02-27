#ifndef Feeler_hpp
#define Feeler_hpp

#include <iostream>
#include "Spring.hpp"

using namespace ci;

class Feeler : public GameObject{
    
public:
    Feeler();
    Feeler(vec2 loc, int joints, float baseWidth, float tipWidth, float depth);
    Feeler(vec2 loc, int joints, float baseWidth, float tipWidth) : Feeler(loc, joints, baseWidth, tipWidth, 1.0f){ };
    
    ~Feeler(){
        for( vector<Spring*>::iterator p = springs.begin(); p != springs.end(); ++p){
            delete *p;
        }
    }
    
    void update();
    void collide(const vec2 & heroLoc, float colliderSize);
    void addForce(vec2 force);
    bool feelTowards( const vec2 & colliderPos, float minDist, float force );
    
    vector<vec2> getPoints(){
        vector<vec2> poop;
        for(int i = 0; i < springs.size(); i++)
            poop.push_back( springs.at(i)->local );
        return poop;
    };
    
    //getters
    bool inContactWithCollider(){ return feelerContact; };

    vector<vec2> drawPositions;
    
private:
   
    void updateDrawPositions();
   
    bool feelerContact;
  
    vector<Spring*> springs;

    
    float baseWidth = 4.0f;
    float tipWidth = 0.2f;

    
};

#endif
