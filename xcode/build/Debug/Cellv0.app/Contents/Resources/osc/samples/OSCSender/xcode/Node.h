//
//  Node.h
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#ifndef __OSCSender__Node__
#define __OSCSender__Node__

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "GameObject.h"
#include "cinder/ImageIo.h"
#include <iostream>
#include "cinder/Rand.h"
#include "Bubble.h"
#include "cinder/Utilities.h"
#include "cinder/Sphere.h"

using namespace ci;
using namespace ci::app;
using namespace std;




class Node : public GameObject {
public:
    Node(Vec2f location, string t);
    Vec2f velocity;
    Vec2f offset;
    Vec2f center;
    
    string type;
    
   // gl::Texture image;

    void resize(Vec2f mouseLoc);
    bool trigger(float rippleLoc);
    void update(Vec2f mouseLoc);
    void draw();
    void collide(Vec2f aLoc);
    void collide(Node an);
    void addVelocity(Vec2f add);
    void orbit(Vec2f home);
    
    list<Spark> sparks;
    
};


#endif /* defined(__OSCSender__Node__) */
