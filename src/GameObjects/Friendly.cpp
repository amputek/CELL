#include "Friendly.hpp"


Friendly :: Friendly(vec2 loc, gl::TextureRef* tex) : Swimmer(loc){
    speed = 0.015;
    radius = 4;
    tail = new Tail( 2, false, radius*0.1, false );
    img = tex;
    levelCount = 0;
    
    mborn = false;
    birthing = false;
    birthCount = 0;
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
        levelCount++;
        if(radius < 30){
            if(levelCount % 500 == 0){
                radius+=1;
                tail->incLength(1.0);
            }
        }
    }
    
    tail->update(vec2(global.x - sin(direction) * radius, global.y - cos(direction) * radius), direction);

    if(birthing == true){
        birthCount++;
        if(birthCount >= 40){
            birthing = false;
        }
    }
}


void Friendly :: draw(){
    
    gl::color(Color(1,1,1));
    gl::pushModelView();
    gl::translate(local );
    gl::rotate( -direction * 57.2957795 + 180);
    gl::draw( *img , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    //friendly starts without tail
    if(mborn == true){
        tail->draw();
    }
    
    //when the friendly has been triggered by the player, go through a state of 'birthing' - like levelling up for the player
    if(birthing == true){
        gl::color(ColorA8u(150,200,255,40-birthCount));
        for(int i = 0; i <= radius + birthCount; i+=int(rand(2,6))){
            gl::drawSolidCircle(local, i);
        }
    }
    
    
}