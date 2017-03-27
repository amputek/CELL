#ifndef Feeler_hpp
#define Feeler_hpp

#include <iostream>
#include "SpringyObject.hpp"

using namespace ci;

class Feeler : public IDynamic {
    
public:
    Feeler();
    Feeler(vec2 loc, float depth, int jointCount, float baseWidth = 4.0f, float tipWidth = 0.2f, float stiff = 2.0f )
        : baseWidth( baseWidth ), tipWidth( tipWidth )
    {
        
        
        float stiffness = stiff; //2.0f by default
        float mass = 5.0f;
        float damping = 7.0f;
        
        vec2 dir = randVec2();
        
        for(int i = 0; i < jointCount; i++){
            vec2 springPosition = loc + (dir * (float)i);
            float springStiffness = i == 0 ? 10.0f : stiffness;
            springs.push_back( new SpringyObject(springPosition, depth, springStiffness, mass, damping ) );
        }
        update();
        
    };


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

    void addForce(const vec2 & force)
    {
        for(int i = 1; i < springs.size(); i++){
            springs.at(i)->addForce(force);
        }
    }

    
    FeelerStruct getDrawStruct() const
    {
        return FeelerStruct( positions, tipWidth, baseWidth);
    }
    
    void update(){
        
        positions.clear();
        
        //all other springs gets updated against the prior spring
        for(int i = 0; i < springs.size(); i++){
            if( i > 0) springs.at(i)->moveTowards(springs.at(i-1)->getPosition());
            springs.at(i)->update();
            positions.push_back( springs.at(i)->getPosition() );
        }
        
    }

    //Collides the feeler with a given point
    //Returns true if feeler is colliding
    bool collide(const vec2 & entityPosition, float colliderSize ){
        
        bool feelerInContact = false;
        
        for(int i = 1; i < springs.size(); i++){
            if( springs.at(i)->collide(entityPosition, colliderSize) )
            {
                feelerInContact = true;
            }
        }
        
        return feelerInContact;
    }
    
    


    //Makes the feeler 'grab' towards a given position
    //Return true if feeler is close enough to grab
    bool feelTowards( const vec2 & colliderPos, float minDist, float force )
    {
        
        bool feelerActivated = false;
        
        for(int i = 1; i < springs.size(); i++){
            
            float alongRatio = (float)i / springs.size();
            
            SpringyObject * spring = springs.at(i);
            
            if( !feelerActivated )
            {
                if( glm::distance( spring->getPosition(), colliderPos ) < minDist ) feelerActivated = true;
            }
            
            //All feelers above the 'activated' point feel towards the positions
            if( feelerActivated )
            {
                spring->addForce( (colliderPos - spring->getPosition()) * force * alongRatio );
            }
        }
        return feelerActivated;
    }
    
    

    
    
private:

    vector<SpringyObject*> springs;
    vector<vec2> positions;
    
    float baseWidth = 4.0f;
    float tipWidth = 0.2f;
    
};




#endif
