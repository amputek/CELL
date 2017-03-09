#include "Spore.hpp"


Spore :: Spore(vec2 loc, float depth, int type) : Dust(loc, depth, randFloat(20,40) * depth, 4.0f ){
    health = randInt(4,8);
    sporeContact = false;
    sporeType = type;
}

void Spore :: update(){
    if(health <= 1){
        mPosition += vec2(randFloat(-2,2), randFloat(-2,2));
    }
    Dust::update();
}

void Spore :: draw( CellRenderer & renderer ){
    renderer.drawSpore( mPosition, mRadius, mDepth, sporeType );
}

void Spore :: collide(const vec2 & loc){
    sporeContact = false;
    
    //only collides with entity if on a similar depth-plane

    if( dist(mPosition, loc) < ( getSize() * 0.75 + 10 )){
        addVelocity( (mPosition - loc) * 6.0f );
        sporeContact = true;                 //variable for OSC
        health -= 1;
    }

}
