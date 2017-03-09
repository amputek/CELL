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
        for( vector<Spring*>::iterator p = springs.begin(); p != springs.end(); ++p){
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
    
    void update();
    void collide(const vec2 & heroLoc, float colliderSize);
    void addForce(const vec2 & force);
    bool feelTowards( const vec2 & colliderPos, float minDist, float force );
    
    const vector<vec2> getPoints() const {
        vector<vec2> positions;
        for(int i = 0; i < springs.size(); i++)
            positions.push_back( springs.at(i)->getPosition() );
        return positions;
    };
    
    //getters
    bool inContactWithCollider(){ return mFeelerInContact; };

    float getBaseWidth(){ return baseWidth; }
    float getTipWidth(){ return tipWidth; }
    
private:
   
    bool mFeelerInContact;
  
    
    vector<Spring*> springs;
    
    float baseWidth = 4.0f;
    float tipWidth = 0.2f;


    
};




#endif
