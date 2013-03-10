//
//  MenuNode.cpp
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#include "MenuNode.h"

MenuNode::MenuNode(Vec2f loc, int radius, string t):GameObject(loc, radius, t){
    originalLoc = loc;
    state = "inactive";
    type = t;
};

void MenuNode::draw(){
    GameObject::draw();
    
    gl::color(Color (1,1,1));
    gl::drawSolidCircle(loc, radius*0.7);
}

void MenuNode::update(Vec2f mouseLoc){
    GameObject::update(mouseLoc);
}

void MenuNode::unclick(){
    GameObject::unclick();
    loc = originalLoc;
}