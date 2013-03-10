//
//  Splash.h
//  Nodes2Multi
//
//  Created by Rob on 13/10/2012.
//
//

#ifndef __Nodes2Multi__Splash__
#define __Nodes2Multi__Splash__

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

class Splash{
public:
    Splash(Vec2f aloc, float size);
    Splash(Vec2f aloc, float size, float endSize);
    bool alive();
    void draw();
    
    float time = 0;
    int life;
    float startSize;
    Vec2f loc;
};

#endif /* defined(__Nodes2Multi__Splash__) */
