#include "Plankton.hpp"


Plankton :: Plankton( vec2 loc, gl::TextureRef* tex, int t) : GameObject(loc, rand(0.9,1.1)){
    radius = int(rand(6,20));
    planktonType = t;
    img = tex;
}

void Plankton :: draw(){
    if(onScreen() == true){
        gl::color( Color(1,1,1) );
        gl::draw( *img, Rectf(local.x - radius, local.y - radius, local.x + radius, local.y + radius));
    }
}

bool Plankton :: alive(){
    return !(local.x < -400 || local.x > 1200 || local.y < -400 || local.y > 1000);
}

bool Plankton :: checkEaten( vec2 heroLocation ){
    return dist(heroLocation, global) < 10;
}