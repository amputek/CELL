// Follows braitenberg vehicle type movement
// Has two sensors which affect two motors
//
//

#ifndef Braitenberg_hpp
#define Braitenberg_hpp

#include "GameObject.hpp"

using namespace std;

class Braitenberg : public GameObject{
    
public:
    Braitenberg(vec2 startPosition, bool canSlow, float raduis, float speed );
    void moveTo( const vec2 & loc);


    vec2 targetDestination;
    void setDestination(vec2 dest){
        targetDestination = dest;
    };
    
private:
    float leftMotorSpeed, rightMotorSpeed;
    const float turnLimit = 0.3;
    const float turnSpeed = 0.05;
    bool slowsWhenAtTarget;
    
    const float fixedDistanceToTarget = 200.0f;
  
protected:
    float mDirection;
    float mSpeed;
    

};

#endif
