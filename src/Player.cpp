#include "Player.h"


Player :: Player(Vec2f loc, vector<gl::Texture*> texs) : Braitenberg(loc, true){
    speed = 0.02;
    radius = 5;
    level = 1;
    longTail = new Tail( 3, false, 1, false );
    playerLevelling = false;
    falling = false;
    
    imgs = texs;
    img = imgs.at(0);
    
    for(int i = 0; i < 5; i ++){
        planktonEaten.push_back(0);
    }
}


//move to a specified location
void Player :: moveTo(Vec2f dest){
    
    if(falling == true){
        Braitenberg::moveTo( Vec2f(dest.x, 400) );
    } else {
        Braitenberg::moveTo(dest);
    }
}



void Player :: update(){
    
    falling = global.y < -5000;

    if(playerLevelling == true){
        levelCount++;
        if(levelCount >= 40){
            playerLevelling = false;
        }
    }
    

   // leftTail->addPoint(Vec2f(global.x - sin(direction-M_PI/4) * radius, global.y - cos(direction-M_PI/4) * radius), direction-M_PI/4);
   // rightTail->addPoint(Vec2f(global.x - sin(direction+M_PI/4) * radius, global.y - cos(direction+M_PI/4) * radius), direction+M_PI/4);
    longTail->update(Vec2f(global.x - sin(direction) * radius, global.y - cos(direction) * radius), direction);
   // leftTail->update();
   // rightTail->update();
    
    Braitenberg::update();
}


void Player:: draw(){
    
    gl::color(Color(1,1,1));
    
    gl::pushModelView();
    gl::translate(local );
    gl::rotate( -direction * 57.2957795 + 180);
    gl::draw( *img , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    
    if(playerLevelling == true){
        gl::color(ColorA8u(150,200,255,40-levelCount));
        for(int i = 0; i <= radius + levelCount; i+=int(rand(2,6))){
            gl::drawSolidCircle(local, i);
        }
    }
    
   // leftTail->draw();
   // rightTail->draw();
    longTail->draw();
}


void Player :: levelUp(){
    level++;
    radius++;
    playerLevelling = true;
    levelCount = 0;
    longTail->incLength(1.2);
    longTail->incWidth(0.25);
    if( level == 10 ){
        longTail->setWide();
    }
    if(level == 20 ){
        longTail->setFins();
    }

    
   // rightTail->length += 0.6;
   // leftTail->length += 0.6;
}