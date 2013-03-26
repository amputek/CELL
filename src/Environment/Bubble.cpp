#include "Bubble.h"

Bubble :: Bubble(Vec2f loc, Vec2f vel, float d, gl::Texture* tex) : Dust( loc, vel, d ) {
    img = tex;
    opacity = depth*0.2;
}

bool Bubble :: alive(){
    //off screen or on floor or above surface
    return !(local.y < -200 || local.x < -200 || local.x > 1000 || global.y > 505 || global.y < - 7000);
}

void Bubble :: draw(){
    gl::color(ColorA(1.0,1.0,1.0,opacity));
    gl::draw( *img, Rectf( local.x - radius, local.y - radius, local.x + radius, local.y + radius ) );
}