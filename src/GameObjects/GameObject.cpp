#include "GameObject.hpp"

GameObject :: GameObject(vec2 loc, float d){
    global = loc;
    depth = d;
    update();
}

void GameObject :: update(){
    
    //relationship between local (drawn) co-ordinates and global co-ordinates
    local = global - offset;
    local *= depth;
    
    // makes the point of parralax at the center of the screen
    local += (cinder::app::getWindowSize() / 2);
}

bool GameObject :: onScreen(){
    return !(local.x < -200 || local.x > cinder::app::getWindowWidth() + 200 || local.y < -200 || local.y > cinder::app::getWindowHeight() + 200);
}

