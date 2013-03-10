#include "Braitenberg.h"

Braitenberg :: Braitenberg(Vec2f loc, bool sl) : GameObject(loc, 1){
    speed = 0.022;
   // turnLimit = 0.3;
   // turnSpeed = 0.05;
    direction = 0.0;
    slows = sl;
}


void Braitenberg :: moveTo(Vec2f target){
    
    //make sure target is above ground
    if(target.y > 500){ target.y = 500; }
    
    
    //either braitenberg eases towards target or maintains speed (useful for NPCs)
    float d = 0;
    
    //'slows' means braitenberg does not ease
    if(slows == true){
        d = ((global-target).length() * speed);
    } else {
        d = speed*200.0;
    }
    
    //maximum limit on speed
    if(d > speedLimit){
        d = speedLimit;
    }
    
    
    //position of the left and right sensors
    Vec2f right = Vec2f(global.x + sin(direction-0.5) * 10, global.y + cos(direction-0.5) * 10);
    Vec2f left = Vec2f(global.x + sin(direction+0.5) * 10, global.y + cos(direction+0.5) * 10);
    
    //distance between target and sensors
    float li = (left-target).length();
    float ri = (right-target).length();
    
    
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