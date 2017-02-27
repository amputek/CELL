#include "Bubble.hpp"

Bubble :: Bubble( const vec2 & loc, const vec2 & vel, float d, gl::TextureRef * tex) : Dust( loc, vel, 3.0f, d ) {
    img = tex;
    opacity = depth * 0.6;
    radius = rand(3,9) * depth * depth;
    if(radius < 2.0f) radius = 2.0f;

}

bool Bubble :: alive(){
    //off screen or on floor or above surface
    return !(local.y < -100 || local.x < -100 || local.x > cinder::app::getWindowWidth() + 100);
}

void Bubble :: draw(){
    
    if( !onScreen() ) return;
    entityDrawCount++;
        
    gl::color(ColorA( 1.0, 1.0, 1.0, opacity ));
    gl::ScopedModelMatrix modelScope;
    gl::translate( local );
    gl::draw( *img, Rectf(-radius,-radius,radius,radius) );
    
}
