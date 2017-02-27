#include "Plankton.hpp"


Plankton :: Plankton( vec2 loc, gl::TextureRef* tex, int t) : GameObject(loc, rand(0.9,1.1)){
    radius = int(rand(6,13));
    planktonType = t;
    img = tex;
    rotation = randFloat(0, M_PI * 2);
}

void Plankton :: draw(){
    if(!onScreen()) return;
    
    entityDrawCount++;
    
    gl::ScopedModelMatrix modelScope;
    gl::translate( local );
    gl::rotate( rotation );
    gl::draw( *img, Rectf(-radius,-radius,radius,radius) );
}

bool Plankton :: alive(){
    return onScreen();
}

bool Plankton :: checkEaten( const vec2 & heroLocation ){
    return dist(heroLocation, global) < 10;
}
