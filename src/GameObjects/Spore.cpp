#include "Spore.hpp"


Spore :: Spore(vec2 loc, float depth, int type) : Dust(loc, depth, randFloat(20,40) * depth, 4.0f ){
    mHealth = randInt(4,8);
    mSporeType = type;
}

void Spore :: update(){

    mContactFlag = false;
    
    if(mHealth <= 1){
        mPosition += vec2(randFloat(-2,2), randFloat(-2,2));
    }
    Dust::update();
}

void Spore :: draw( CellRenderer * renderer ){
    renderer->drawSpore( mPosition, mRadius, mDepth, mSporeType );
}

void Spore :: collide(const vec2 & loc){

    if( dist(mPosition, loc) < ( getSize() * 0.75 + 10 )){
        addVelocity( (mPosition - loc) * 6.0f );
        mContactFlag = true;
        mHealth -= 1;
    }

}
