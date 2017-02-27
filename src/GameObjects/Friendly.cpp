#include "Friendly.hpp"


Friendly :: Friendly(vec2 loc, gl::TextureRef* tex) : Swimmer(loc){
    speed = 0.5f;
    radius = 6.0f;
    tail = new Tail( 2, false, radius*0.1, false );
    img = tex;
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
        if(radius < 20.0f){
            if(levelCounter > 800.0f)
            {
                radius++;
                levelCounter = 0.0f;
                tail->incLength(1.0);
            }
        }
    }
    
    tail->update(vec2(global.x - sin(direction) * radius, global.y - cos(direction) * radius), direction);

    if(birthing == true){
        birthCount += deltaTime * 60.0f;
        if(birthCount >= 40.0f){
            birthing = false;
        }
    }
}


void Friendly :: draw(){
    
    if( !onScreen() ) return;
    entityDrawCount++;
        gl::pushModelView();
    gl::translate(local );
    gl::rotate( -direction + M_PI );
    gl::draw( *img , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    //friendly starts without tail
    if(mborn){
        tail->draw();
    }
    
    
}
