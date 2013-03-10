//
//  Highest Superclass for all game objects
//  Has a global location in gameworld, and local location on screen
//  Has a depth and a radius
//

#ifndef __Cellv0__GameObject__
#define __Cellv0__GameObject__

#include <iostream>
#include "Globals.h"

using namespace globals;

class GameObject{
public:
    GameObject(Vec2f loc, float d);
    void update();
    bool onScreen();
    Vec2f global;
    Vec2f local;
    float depth;
    int radius;
    
   // bool operator<( const GameObject& c ) { return depth > c.depth; }

};


#endif /* defined(__Cellv0__GameObject__) */
