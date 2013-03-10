#include "Plankton.h"


Plankton :: Plankton(Vec2f loc, gl::Texture* tex, int t) : GameObject(loc, rand(0.9,1.1)){
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
//    if(local.x < -400 || local.x > 1200 || local.y < -400 || local.y > 1000){
//        return false;
//    }
//    return true;
}

bool Plankton :: checkEaten(Vec2f heroLocation){
    return (heroLocation - global).lengthSquared() < 2000;
}