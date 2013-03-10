//
//  GameObject.h
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#ifndef __OSCSender__GameObject__
#define __OSCSender__GameObject__

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Font.h"
//#include "Resources.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

class GameObject {
public:
    GameObject(Vec2f location, int radius, string t);
    Vec2f loc;
    
    cinder::gl::Texture textTexture;
    
    int radius;
    int opacity;
    string state;
    
    bool inCircle();
    
    float getDistance();
    void update(Vec2f mouseLoc);
    void draw();
    void click(Vec2f mouseLoc);
    void unclick();
    void highlight();

};



#endif /* defined(__OSCSender__GameObject__) */
