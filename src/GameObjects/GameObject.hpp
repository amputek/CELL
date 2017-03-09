//
//  Highest Superclass for all game objects
//  Has a global location in gameworld, and local location on screen
//  Has a depth and a radius
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <iostream>
#include "Globals.hpp"

using namespace globals;
using namespace ci;

class GameObject{
public:
    GameObject(vec2 startWorldPosition, float entityDepth, float entitySize )
    {
        mPosition = startWorldPosition;
        mDepth = entityDepth;
        mRadius = entitySize;
    }
    
    float getSize() const { return mRadius; }
    float getDepth() const { return mDepth; }
    vec2 getPosition() const { return mPosition; }
    
protected:
    float mRadius;
    float mDepth;
    vec2 mPosition;
    
    

};


#endif
