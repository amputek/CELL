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
    GameObject(vec2 startWorldPosition, float entityDepth);
    void update();
    bool onScreen();
    vec2 global;
    vec2 local;
    float depth;
    int radius;
    
    int onScreenSize = 200;

   // bool operator<( const GameObject& c ) { return depth > c.depth; }

    void deleteObject(){ }
};


#endif
