#include "Braitenberg.hpp"

Braitenberg :: Braitenberg(vec2 loc, bool sl) : GameObject(loc, 1){
    direction = 0.0;
    slowsWhenAtTarget = sl;
}


void Braitenberg :: moveTo(vec2 target){
    
    //make sure target is above ground
    if(target.y > 0){ target.y = 0; }
    
    //initialise speed modifier
    float speedModifier = 0;
    
    //either braitenberg eases towards target or maintains speed (useful for NPCs)

    if(slowsWhenAtTarget){
        float distanceToTarget = length(global-target);
        speedModifier = distanceToTarget * speed;
    } else {
        speedModifier = fixedDistanceToTarget * speed;
    }

    
    
    //position of the left and right sensors
    vec2 right = vec2(global.x + sin(direction-0.5) * 10, global.y + cos(direction-0.5) * 10);
    vec2 left = vec2(global.x + sin(direction+0.5) * 10, global.y + cos(direction+0.5) * 10);
    
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
    direction += ( rightMotorSpeed - leftMotorSpeed ) * deltaTime * 10.0f;
    
    //increase position depending on direction (with speed)
    global.x += sin(direction) * speedModifier * deltaTime;
    global.y += cos(direction) * speedModifier * deltaTime;
}
