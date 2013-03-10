//
//  Mask.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Mask__
#define __Cellv0__Mask__

#include <iostream>
#include "cinder/gl/Fbo.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

class Mask{
    
public:
    Mask( gl::Texture tex );
    void update(float xo, float yo, float mod);
    void draw();
    
private:
    gl::Fbo mFbo;
    gl::Texture mTexture;
    float x1,y1,x2,y2;
    
};

#endif /* defined(__Cellv0__Mask__) */
