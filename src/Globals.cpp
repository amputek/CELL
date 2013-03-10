
#include "Globals.h"

namespace globals {
    Vec2f offset = Vec2f(0,0);
    
    float rand(float a, float b){
        return Rand::randFloat(a,b);
    }
    
    float rand(float a){
        return Rand::randFloat(a);
    }
    
    //converts a local coordinate set into a global coordinate set
    //based on a given depth value and the current offset coordinate
    Vec2f globalise(Vec2f loc, float d){
        return (loc - (cinder::app::getWindowSize() / 2)) / d + offset;
    }
}