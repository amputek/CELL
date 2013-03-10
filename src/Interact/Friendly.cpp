#include "Friendly.h"


Friendly :: Friendly(Vec2f loc, gl::Texture* tex) : Swimmer(loc){
    speed = 0.015;
    radius = 4;
    tail = new Tail( radius*2, false, radius*0.1, false );
    img = tex;
}


void Friendly :: update(){
    levelCount++;
    if(radius < 30){
        if(levelCount % 500 == 0){
            radius+=1;
            tail->incLength(1.0);
        }
    }
    
    tail->update(Vec2f(global.x - sin(direction) * radius, global.y - cos(direction) * radius), direction);
    Swimmer::update();
}

void Friendly :: draw(){
    
    gl::color(Color(1,1,1));
    gl::pushModelView();
    gl::translate(local );
    gl::rotate( -direction * 57.2957795 + 180);
    gl::draw( *img , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    tail->draw();
    
}