//
//  Bubble.h
//  OSCSender
//
//  Created by Rob on 23/10/2012.
//
//

#ifndef __OSCSender__Bubble__
#define __OSCSender__Bubble__

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

class Spark{
public:
    Spark(Vec2f aloc, float size, float r, float g, float b);
    bool alive();
    void draw();
    float r,g,b;
    float time = 0;
    int life;
    float startSize;
    Vec2f loc;
};

#endif /* defined(__OSCSender__Bubble__) */
