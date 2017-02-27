#ifndef Mask_hpp
#define Mask_hpp

#include <iostream>
#include "cinder/gl/Fbo.h"

using namespace ci;

class Mask{
    
public:
    Mask( gl::TextureRef tex );
    void update(const vec2 & heroLocalPosition, float depthMod);
    void draw();
    
private:
    gl::FboRef mFbo;
    gl::TextureRef mTexture;
    float x1,y1,x2,y2;
    Rectf drawRect;
    
};

#endif
