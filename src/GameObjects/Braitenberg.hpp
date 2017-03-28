// Follows braitenberg vehicle type movement
// Has two sensors which affect two motors
//
//

#ifndef Braitenberg_hpp
#define Braitenberg_hpp

#include "GameObject.hpp"

using namespace std;

class Braitenberg : public GameObject, public IDynamic{
    
public:
    Braitenberg(vec2 startPosition, bool canSlow, float radius, float speed )
    : slowsWhenAtTarget(canSlow), mSpeed(speed), GameObject(startPosition, 1, radius ) {
        mTargetDestination = startPosition;
    }

    void update(){

        //Safety Checks! Shouldn't really be needed
        if( isnan( mPosition.x ) ){
            mPosition = mTargetDestination;
            leftMotorSpeed = 0.5f;
            rightMotorSpeed = 0.5f;
            mDirection = 0.0f;
            mSpeed = 0.1f;
            cout << "SAFETY FAIL! NAN" << endl;
        }
        if( isinf( abs(mPosition.x) ) ) mTargetDestination = mPosition + vec2(1,1);
        if( mPosition == mTargetDestination ) mTargetDestination += vec2(1,1);
        
        //initialise speed modifier
        float speedModifier = 0;
        
        //either braitenberg eases towards target or maintains speed (useful for NPCs)
        
     
        if(slowsWhenAtTarget){
            float distanceToTarget = length(mPosition - mTargetDestination);
            speedModifier = distanceToTarget * mSpeed;
        } else {
            speedModifier = fixedDistanceToTarget * mSpeed;
        }
        
        
        
        //position of the left and right sensors
        vec2 right = vec2(mPosition.x + sin(mDirection-0.5) * 10, mPosition.y + cos(mDirection-0.5) * 10);
        vec2 left = vec2(mPosition.x + sin(mDirection+0.5) * 10, mPosition.y + cos(mDirection+0.5) * 10);
        
        //distance between target and sensors
        float leftDist = length(left-mTargetDestination);
        float rightDist = length(right-mTargetDestination);
        
        
        //turn depending on which sensor is higher
        if (leftDist > rightDist) {
            rightMotorSpeed -= turnSpeed * deltaTime * 30.0f;
            leftMotorSpeed += turnSpeed * deltaTime * 30.0f;
        } else {
            leftMotorSpeed -= turnSpeed * deltaTime * 30.0f;
            rightMotorSpeed += turnSpeed * deltaTime * 30.0f;
        }
        
        //make sure speeds are not above limit
        if(leftMotorSpeed < -turnLimit){
            leftMotorSpeed = -turnLimit;
        } else if(leftMotorSpeed > turnLimit){
            leftMotorSpeed = turnLimit;
        }
        
        if(rightMotorSpeed < -turnLimit){
            rightMotorSpeed = -turnLimit;
        } else if(rightMotorSpeed > turnLimit){
            rightMotorSpeed = turnLimit;
        }
    
        //turn accordingly
        mDirection += ( rightMotorSpeed - leftMotorSpeed ) * deltaTime * 10.0f;
        
        //increase position depending on direction (with speed)
        mPosition.x += sin(mDirection) * speedModifier * deltaTime;
        mPosition.y += cos(mDirection) * speedModifier * deltaTime;
    }

    void setDestination(vec2 dest){
        mTargetDestination = dest;
    };
    
private:
    float leftMotorSpeed = 0.0f, rightMotorSpeed = 0.0f;
    const float turnLimit = 0.3;
    const float turnSpeed = 0.05;
    bool slowsWhenAtTarget;
    
    const float fixedDistanceToTarget = 200.0f;
  

    
protected:
    
    vec2 mTargetDestination;
    float mDirection = 0.0f;
    float mSpeed = 0.1f;

    void debugDraw( CellRenderer & renderer )
    {
        if( DEBUG_MODE ) renderer.debugDestination(mPosition, mTargetDestination);
    }
    

};

#endif
