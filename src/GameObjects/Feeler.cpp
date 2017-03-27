#include "Feeler.hpp"

Feeler :: Feeler(vec2 loc, float depth, int jointCount, float base, float tip, float stiff ) {
    
    baseWidth = base;
    tipWidth = tip;
    
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



bool Feeler :: collide(const vec2 & entityPosition, float colliderSize ){
    bool feelerInContact = false;
    for(int i = 1; i < springs.size(); i++){
        if( springs.at(i)->collide(entityPosition, colliderSize) )
        {
            feelerInContact = true;
        }
    }
    return feelerInContact;
}

bool Feeler :: feelTowards( const vec2 & colliderPos, float minDist, float force ){
    
    bool activated = false;
    
    for(int i = 1; i < springs.size(); i++){
    
        float alongRatio = (float)i / springs.size();
        
        SpringyObject * spring = springs.at(i);
        if( glm::distance( spring->getPosition(), colliderPos ) < minDist )
        {
            activated = true;
        }
        if( activated )
        {
            spring->addForce( (colliderPos - spring->getPosition()) * force * alongRatio );
        }
    }
    return activated;
}


//add force - for environmental affects (gravity, flow etc)
void Feeler :: addForce(const vec2 & force){
    for(int i = 1; i < springs.size(); i++){
        springs.at(i)->addForce(force);
    }
}


void Feeler :: update(){
    
    positions.clear();
    
    //all other springs gets updated against the prior spring
    for(int i = 0; i < springs.size(); i++){
        if( i > 0) springs.at(i)->moveTowards(springs.at(i-1)->getPosition());
        springs.at(i)->update();
        positions.push_back( springs.at(i)->getPosition() );
    }
    
}


