#include "Player.h"


Player :: Player(Vec2f loc, vector<gl::Texture*> texs) : Braitenberg(loc, true){
    speed = 0.02;
    radius = 5;
    level = 1;
    longTail = new Tail( 5, false, 3, false);
    playerLevelling = false;
    falling = false;
    
    imgs = texs;
    img = imgs.at(0);
    
    for(int i = 0; i < 5; i ++){
        planktonEaten.push_back(0);
    }
}


//move to a specified location
void Player :: moveTo(Vec2f mousePos){
    
    //make sure mouse location is within window boundaries
    if(mousePos.x < 0                 ){  mousePos.x = 10;                     }
    if(mousePos.x > getWindowWidth()  ){  mousePos.x = getWindowWidth() - 10;  }
    if(mousePos.y < 0                 ){  mousePos.y = 10;                     }
    if(mousePos.y > getWindowHeight() ){  mousePos.y = getWindowHeight() - 10; }
    
    Vec2f dest = globalise(mousePos, 1);
    
    if(falling == true){
        Braitenberg::moveTo( Vec2f(dest.x, 400) );  //falls back towards water
    } else {
        Braitenberg::moveTo(dest);                  //standard movement
    }
}



void Player :: update(){
    
    falling = global.y < -7000;
    
    //check if player is in the process of 'levelling up'
    if(playerLevelling == true){
        levelCount++;
        if(levelCount >= 40){       //player levels up for 40 ticks
            playerLevelling = false;
        }
    }
    
    longTail->update(Vec2f(global.x - sin(direction) * radius, global.y - cos(direction) * radius), direction);
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
    
    longTail->draw();
}

void Player :: incEaten(int type){
    planktonEaten.at(type)++;
    
    if(planktonEaten.at(type) % 8 == 0){
        levelUp();
    }
};

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

}