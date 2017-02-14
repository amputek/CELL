#include "Bubble.hpp"

Bubble :: Bubble( const vec2 & loc, const vec2 & vel, float d, gl::TextureRef * tex) : Dust( loc, vel, d ) {
    img = tex;
    opacity = depth * 0.6;
    
    colorWithOpacity = ColorA( 1.0, 1.0, 1.0, opacity );
}

bool Bubble :: alive(){
    //off screen or on floor or above surface
    return !(local.y < -200 || local.x < -200 || local.x > cinder::app::getWindowWidth() + 200 || global.y > 505 || global.y < - 7000);
}

void Bubble :: draw(){
    gl::color(colorWithOpacity);
    gl::ScopedModelMatrix modelScope;
    gl::translate( local );
    gl::draw( *img, drawRect );
    
}
