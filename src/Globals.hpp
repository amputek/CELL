#ifndef Globals_hpp
#define Globals_hpp

#include "cinder/Rand.h"

using namespace ci;

namespace globals {
    extern float deltaTime;
    extern vec2 offset;
    extern float rand(float a, float b);
    extern float rand(float a);
    extern vec2 vrand(float x);
    extern vec2 vrand(float min, float max);
    extern int irand(int a, int b);
    extern vec2 globalise(vec2 loc, float d);
    extern vec2 localise(vec2 loc, float d);
    extern float dist(vec2 locA, vec2 locB);
    
    extern int entityDrawCount;
}


#endif
