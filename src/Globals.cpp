
#include "Globals.hpp"

namespace globals {
    vec2 offset = vec2(0,0);
    
    float deltaTime = 0.0166f;

    int entityDrawCount = 0;
    
    //random float in range
    float rand(float a, float b){
        return Rand::randFloat(a,b);
    }
    
    //random float
    float rand(float a){
        return Rand::randFloat(a);
    }
    
    //random vector
    vec2 vrand(float x){
        return vec2( rand(-x,x), rand(-x,x) );
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
    
    vec2 localise(vec2 global, float d){
        vec2 local = global - offset;
        local *= d;
        local += (cinder::app::getWindowSize() / 2);
        return local;
    }
    
    //distance between two vectors, for convenience
    float dist(vec2 locA, vec2 locB){
        return length(locA - locB);
    }
}
