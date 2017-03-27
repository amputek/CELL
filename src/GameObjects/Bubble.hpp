#ifndef Bubble_hpp
#define Bubble_hpp

#include <iostream>
#include "PhysicsObject.hpp"

class Bubble : public PhysicsObject, public IDrawable{
public:
    
    Bubble( const vec2 & loc, const vec2 & vel, float depth ) : PhysicsObject( loc, depth, randFloat(3,9) * depth * depth, 4.0f, vel ) {

    }
    
    void draw( CellRenderer & renderer ){
         renderer.drawBubble( mPosition, mRadius, mDepth );
    }

};


#endif
