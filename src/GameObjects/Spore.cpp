#include "Spore.hpp"


Spore :: Spore(vec2 loc, float d, int t, gl::TextureRef* tex) : Dust(loc, vec2(0,0), d){
    health = 4;
    radius = rand(20,40)*depth;
    sporeContact = false;
    img = tex;
    sporeType = t;
}

void Spore :: update(){
    if(health <= 1){
        global += vec2(rand(-2,2), rand(-2,2));
    }
    Dust::update();
}

void Spore :: draw(){
    if(onScreen() == true){
        gl::color( ColorA(1.0,1.0,1.0,0.8*depth) );
        gl::draw( *img, Rectf( local.x - radius, local.y - radius, local.x + radius, local.y + radius ) );
    }
}

void Spore :: collide(vec2 loc){
    sporeContact = false;
    
    //only collides with entity if on a similar depth-plane
    if(abs(depth - 1.0) < 0.25){
        if( dist(global, loc) < (radius * 0.75 + 10 )){
            velocity += (global - loc) * 0.2f;    //incrememnt velocity
            sporeContact = true;                 //variable for OSC
            health -= 1;
            radius += 1;                         //grow in size on each hit
        }
    }
}
