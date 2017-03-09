#include "Friendly.hpp"


Friendly :: Friendly(vec2 loc) : Swimmer(loc, 6.0f, randFloat(0.4f,0.6f) ){
    tail = new Tail( 2, false, 0.6f, false );
}

//when hero reaches friendly, it gets born
void Friendly :: birth(){
    if(mborn == false){
        mborn = true;
        birthing = true;
    }
}


void Friendly :: update(){
    Swimmer::update();
    
    //level up automatically, every 500 frames
    if(mborn == true){
        levelCounter += deltaTime * 60.0f;
        if( mRadius < 20.0f){
            if(levelCounter > 800.0f)
            {
                mRadius++;
                levelCounter = 0.0f;
                tail->incLength(1.0);
            }
        }
    }
    
    tail->update(vec2(mPosition.x - sin(mDirection) * mRadius, mPosition.y - cos(mDirection) * mRadius), mDirection);

    if(birthing == true){
        birthCount += deltaTime * 60.0f;
        if(birthCount >= 40.0f){
            birthing = false;
        }
    }
}


void Friendly :: draw( CellRenderer & renderer ){
    
    renderer.drawFriendly( mPosition, mDirection, mRadius );
    if( mborn ) tail->draw( renderer );
    
}
