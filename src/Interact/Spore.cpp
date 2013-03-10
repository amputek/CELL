#include "Spore.h"


Spore :: Spore(Vec2f loc, float d, int t, gl::Texture* tex) : Dust(loc, Vec2f(0,0), d){
    health = 10;
    radius = 30*depth;
    sporeContact = false;
    img = tex;
    sporeType = t;
}

void Spore :: update(){
    if(health <= 1){
        global += Vec2f(rand(-2,2), rand(-2,2));
    }
    Dust::update();
}

void Spore :: draw(){
    if(onScreen() == true){
        gl::color( ColorA(1.0,1.0,1.0,0.7*depth) );
        gl::draw( *img, Rectf( local.x - radius, local.y - radius, local.x + radius, local.y + radius ) );
    }
}

void Spore :: collide(Vec2f loc){
    sporeContact = false;
    if(abs(depth - 1.0) < 0.25){
        if( (global - loc).length() < (radius*0.75 + 10 )){
            velocity += (global - loc) * 0.2;
            sporeContact = true;
            health-=1;
            radius+=1;
        }
    }
}
