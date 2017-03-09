#ifndef Mask_hpp
#define Mask_hpp

#include <iostream>
#include "cinder/gl/Fbo.h"
#include "Renderer.hpp"

using namespace ci;

class Mask{
    
public:
    Mask(){
        
    }
    void update( const vec2 & pos, float d )
    {
        local = pos;
        depthMod = d;
    }
    void draw( CellRenderer & renderer )
    {
        renderer.drawMask( local, mFbo, depthMod );
    }
    gl::FboRef mFbo;
    
private:
    vec2 local;
    float depthMod;
    
};

#endif
