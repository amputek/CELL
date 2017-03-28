
#include "Globals.hpp"

namespace globals {
    
    bool DEBUG_MODE = false;
    
    vec2 offset = vec2(-400,-2000);
    
    float deltaTime = 0.0166f;

    int entityDrawCount = 0;
    
    //random vector
    vec2 vrand(float x){
        return vec2( randFloat(-x,x), randFloat(-x,x) );
    }
    
    vec2 vrand(float min, float max){
        return vec2( randPosNegFloat(min,max), randPosNegFloat(min,max) );
    }
    
    //random integer
    int irand(int a, int b){
        return Rand::randInt(a,b);
    }
    
    //converts a local coordinate set into a global coordinate set
    //based on a given depth value and the current offset coordinate
    vec2 globalise(vec2 loc, float d){
        vec2 a = cinder::app::getWindowSize() / 2;
        vec2 b = (loc - a) / d + offset;
//        return ( loc - ( cinder::app::getWindowSize() / 2 ) ) / d + offset;
        return b;
    }

    
    //distance between two vectors, for convenience
    float dist(vec2 locA, vec2 locB){
        return length(locA - locB);
    }
}
