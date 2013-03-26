#include "Spore.h"


Spore :: Spore(Vec2f loc, float d, int t, gl::Texture* tex) : Dust(loc, Vec2f(0,0), d){
    health = 4;
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
        gl::color( ColorA(1.0,1.0,1.0,0.8*depth) );
        gl::draw( *img, Rectf( local.x - radius, local.y - radius, local.x + radius, local.y + radius ) );
    }
}

void Spore :: collide(Vec2f loc){
    sporeContact = false;
    
    //only collides with entity if on a similar depth-plane
    if(abs(depth - 1.0) < 0.25){
        if( dist(global, loc) < (radius*0.75 + 10 )){
            velocity += (global - loc) * 0.2;    //incrememnt velocity
            sporeContact = true;                 //variable for OSC
            health-=1;
            radius+=1;                           //grow in size on each hit
        }
    }
}
