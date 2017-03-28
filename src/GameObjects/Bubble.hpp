#ifndef Bubble_hpp
#define Bubble_hpp

#include <iostream>
#include "PhysicsObject.hpp"

class Bubble : public PhysicsObject, public IDrawable{
public:
    
    
    static int ENTITY_COUNT;
    static const int DESPAWN_OFF_SCREEN_BY = 600;
    static const int SPAWN_OFF_SCREEN_BY = 200;
    
    
    Bubble( const vec2 & loc, const vec2 & vel, float depth ) : PhysicsObject( loc, depth, randFloat(3,9) * depth * depth, 4.0f, vel )
    {
        ENTITY_COUNT++;
        GameObject::mDespawnOffScreenDist = DESPAWN_OFF_SCREEN_BY;
    }
    
    ~Bubble()
    {
        ENTITY_COUNT--;
    }
    
    void update()
    {
        addVelocity( vec2( randFloat(-1.5f,1.5f), randFloat(-150.0f,-100.0f) * deltaTime) );
        PhysicsObject::update();
    }
    
    void draw( CellRenderer & renderer ){
         renderer.drawBubble( mPosition, mRadius, mDepth );
    }

};


#endif
