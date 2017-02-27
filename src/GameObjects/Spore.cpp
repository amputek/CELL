#include "Spore.hpp"


Spore :: Spore(vec2 loc, float depth, int type, gl::TextureRef* tex) : Dust(loc, vec2(0,0), 4.0f, depth){
    health = 4;
    radius = rand(20,40)*depth;
    sporeContact = false;
    img = tex;
    sporeType = type;
}

void Spore :: update(){
    if(health <= 1){
        global += vec2(rand(-2,2), rand(-2,2));
    }
    Dust::update();
}

void Spore :: draw(){
    if(!onScreen()) return;
    entityDrawCount++;
    
    gl::color( ColorA(1.0,1.0,1.0,1.0 * depth) );
    gl::draw( *img, Rectf( local.x - radius, local.y - radius, local.x + radius, local.y + radius ) );
}

void Spore :: collide(vec2 loc){
    sporeContact = false;
    
    //only collides with entity if on a similar depth-plane
    if(abs(depth - 1.0) < 0.2){
        if( dist(global, loc) < (radius * 0.75 + 10 )){
            velocity += (global - loc) * 6.0f;    //incrememnt velocity
            sporeContact = true;                 //variable for OSC
            health -= 1;
            radius += 1;                         //grow in size on each hit
        }
    }
}
