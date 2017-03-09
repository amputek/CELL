#include "Braitenberg.hpp"

Braitenberg :: Braitenberg(vec2 position, bool slowsAtTarget, float radius, float speed) : GameObject(position, 1, radius){
    mDirection = 0.0;
    mSpeed = speed;
    slowsWhenAtTarget = slowsAtTarget;
}


void Braitenberg :: moveTo(const vec2 & target){
    
    
    //initialise speed modifier
    float speedModifier = 0;
    
    //either braitenberg eases towards target or maintains speed (useful for NPCs)

    if(slowsWhenAtTarget){
        float distanceToTarget = length(mPosition - target);
        speedModifier = distanceToTarget * mSpeed;
    } else {
        speedModifier = fixedDistanceToTarget * mSpeed;
    }

    
    
    //position of the left and right sensors
    vec2 right = vec2(mPosition.x + sin(mDirection-0.5) * 10, mPosition.y + cos(mDirection-0.5) * 10);
    vec2 left = vec2(mPosition.x + sin(mDirection+0.5) * 10, mPosition.y + cos(mDirection+0.5) * 10);
    
    //distance between target and sensors
    float leftDist = length(left-target);
    float rightDist = length(right-target);
    
    
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
