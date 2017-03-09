#ifndef Bubble_hpp
#define Bubble_hpp

#include <iostream>
#include "Dust.hpp"
#include "Renderer.hpp"

class Bubble : public Dust{
public:
    
    Bubble( const vec2 & loc, const vec2 & vel, float depth ) : Dust( loc, depth, randFloat(3,9) * depth * depth, 4.0f, vel ) {

    }
    
    void draw( CellRenderer & renderer ){
         renderer.drawBubble( mPosition, mRadius, mDepth );
    }

};


#endif
