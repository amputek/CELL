#include "Player.hpp"


Player :: Player(const vec2 & loc, gl::TextureRef * texs) : Braitenberg(loc, true){
    Braitenberg::speed = 0.5f;
    GameObject::radius = 5.0f;
    level = 1;
    longTail = new Tail( 10, true, 2, false);
    leftTail = new Tail( 2, false, 3, false);
    rightTail = new Tail( 2, false, 3, false);
    playerLevelling = false;
    falling = false;
    
    imgs[0] = &texs[0];
    imgs[1] = &texs[1];
    imgs[2] = &texs[2];
    img = imgs[0];
    
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
        Braitenberg::moveTo( vec2(dest.x, -6500) );  //falls back towards water
    } else {
        Braitenberg::moveTo(dest);                  //standard movement
    }
    
    
}

void Player :: slow(bool t){
    if(t == true){
        speed = 0.15f;
    } else {
        speed = 0.5f;
    }
}


void Player :: update(float deltaTime){
    
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
    
    entityDrawCount++;
    
    gl::color(Color(1,1,1));
    gl::ScopedBlendAlpha alpha;
    gl::pushModelView();
    gl::translate(local );
    gl::rotate( -direction + M_PI );
    gl::draw( *img , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
    gl::popModelView();
    
    
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
    if(level == 20){ img = imgs[0];       }
    if(level == 20){ sideTailsOn = true;  }
    if(level == 40){ img = imgs[1];       }
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
