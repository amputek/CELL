//
//  Link.h
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#ifndef __OSCSender__Link__
#define __OSCSender__Link__

#include <iostream>
#include "Node.h"
#include "Bubble.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class Link{
public:
    Link();
    
    void update(Vec2f mousePos);
    void draw();
    void highlight();
    
    string state;
    
    int index1;
    int index2;
    int opacity;
    
    Node *node1;
    Node *node2;
    
    Vec2f edge1;
    Vec2f edge2;
    
    list<Spark> sparks;
    
};


#endif /* defined(__OSCSender__Link__) */
