#include "Braitenberg.hpp"

Braitenberg :: Braitenberg(vec2 loc, bool sl) : GameObject(loc, 1){
    speed = 0.022;
    direction = 0.0;
    slows = sl;
}


void Braitenberg :: moveTo(vec2 target){
    
    //make sure target is above ground
    if(target.y > 0){ target.y = 0; }
    
    //initialise speed modifier
    float d = 0;
    
    //either braitenberg eases towards target or maintains speed (useful for NPCs)
    //slows = does not ease
    if(slows == true){
        d = ( length(global-target) * speed);
    } else {
        d = speed * 200.0;
    }
    
    //maximum limit on speed
    if(d > speedLimit){
        d = speedLimit;
    }
    
    
    //position of the left and right sensors
    vec2 right = vec2(global.x + sin(direction-0.5) * 10, global.y + cos(direction-0.5) * 10);
    vec2 left = vec2(global.x + sin(direction+0.5) * 10, global.y + cos(direction+0.5) * 10);
    
    //distance between target and sensors
    float li = length(left-target);
    float ri = length(right-target);
    
    
    //turn depending on which sensor is higher
    if (li > ri) {
        rm-=turnSpeed;
        lm+=turnSpeed;
    } else {
        lm-=turnSpeed;
        rm+=turnSpeed;
    }
    
    //make sure speeds are not above limit
    if(lm < -turnLimit){
        lm = -turnLimit;
    } else if(lm > turnLimit){
        lm = turnLimit;
    }
    
    if(rm < -turnLimit){
        rm = -turnLimit;
    } else if(rm > turnLimit){
        rm = turnLimit;
    }
    
    //turn accordingly
    direction += (rm-lm)/5;
    
    //increase position depending on direction (with speed)
    global.x += sin(direction) * (d);
    global.y += cos(direction) * (d);
}