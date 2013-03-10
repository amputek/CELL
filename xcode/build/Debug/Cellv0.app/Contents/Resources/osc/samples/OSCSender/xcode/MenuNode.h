//
//  MenuNode.h
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#ifndef __OSCSender__MenuNode__
#define __OSCSender__MenuNode__

#include <iostream>
#include "GameObject.h"

class MenuNode:public GameObject{
public:
    
    
    MenuNode(Vec2f loc, int radius, string t);
    
    void update(Vec2f mouseLoc);
    void draw();
    void unclick();
    
    Vec2f originalLoc;
    string type;
    
};

#endif /* defined(__OSCSender__MenuNode__) */
