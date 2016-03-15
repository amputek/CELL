#include "Player.hpp"


Player :: Player(vec2 loc, vector<gl::TextureRef> * texs) : Braitenberg(loc, true){
    speed = 0.02;
    radius = 5;
    level = 1;
    longTail = new Tail( 5, false, 3, false);
    leftTail = new Tail( 2, false, 3, false);
    rightTail = new Tail( 2, false, 3, false);
    playerLevelling = false;
    falling = false;
    
    imgs = texs;
    img = &imgs->at(0);
    
    for(int i = 0; i < 7; i ++){
        planktonEaten.push_back(0);
    }
}


//move to a specified location
void Player :: moveTo(vec2 mousePos){
    
    //make sure mouse location is within window boundaries
    if(mousePos.x < 0                 ){  mousePos.x = 10;                     }
    if(mousePos.x > getWindowWidth()  ){  mousePos.x = getWindowWidth() - 10;  }
    if(mousePos.y < 0                 ){  mousePos.y = 10;                     }
    if(mousePos.y > getWindowHeight() ){  mousePos.y = getWindowHeight() - 10; }
    
    vec2 dest = globalise(mousePos, 1);
    
    if(falling == true){
        Braitenberg::moveTo( vec2(dest.x, 400) );  //falls back towards water
    } else {
        Braitenberg::moveTo(dest);                  //standard movement
    }
}

void Player :: slow(bool t){
    if(t == true){
        speed = 0.005;
    } else {
        speed = 0.02;
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
    
    //update tails
    longTail->update(vec2(global.x - sin(direction) * radius, global.y - cos(direction) * radius), direction);
    leftTail->update(vec2(global.x - sin(direction - 1.0) * radius, global.y - cos(direction - 1.0) * radius), direction);
    rightTail->update(vec2(global.x - sin(direction + 1.0) * radius, global.y - cos(direction + 1.0) * radius), direction);
    
    Braitenberg::update();
}


void Player:: draw(){
    
    gl::color(Color(1,1,1));
    
    gl::pushModelView();
    gl::translate(local );
    gl::rotate( -direction * 57.2957795 + 180);
    gl::draw( *img , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    //draw 'glow' around player when it's levelling up
    if(playerLevelling == true){
        gl::color(ColorA8u(150,200,255,40-levelCount));
        for(int i = 0; i <= radius + levelCount; i+=int(rand(2,6))){
            gl::drawSolidCircle(local, i);
        }
    }
    
    //draw tails
    longTail->draw();
    
    if(sideTailsOn == true){
        leftTail->draw();
        rightTail->draw();
    }
}

//returns number of eaten plankton
int Player :: getEaten(){
    int eaten = 0;
    for(int i = 0; i < planktonEaten.size(); i++){
        eaten += planktonEaten.at(i);
    }
    return eaten;
}

void Player :: levelUp(){
    level++;
    
    if(level < 40 && level % 3 == 0){
        radius++;
    }
    if(level == 20){ img = &imgs->at(1);       }
    if(level == 20){ sideTailsOn = true;  }
    if(level == 40){ img = &imgs->at(2);       }
    if(level == 12){ longTail->setWide(); }
    if(level == 35){ longTail->setFins(); }
    
    if(level > 20){
        leftTail->incLength(0.5);
        rightTail->incLength(0.5);
    }
    
    playerLevelling = true;
    levelCount = 0;
    longTail->incLength(0.5);
    longTail->incWidth(0.02);


}