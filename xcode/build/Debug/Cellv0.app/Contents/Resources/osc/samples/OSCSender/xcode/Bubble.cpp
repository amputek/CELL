//
//  Bubble.cpp
//  OSCSender
//
//  Created by Rob on 23/10/2012.
//
//

#include "Bubble.h"

Spark::Spark(Vec2f aloc, float size, float red, float green, float blue){
    life = 15;
    loc = aloc;
    startSize = size;
    r = red;
    g = green;
    b = blue;
}


bool Spark::alive(){
    if(time > life){
        return false;
    }
    return true;
}

void Spark::draw(){

    time+=1;
    
    if(time<life){
        gl::color(ColorA8u(r,g,b, ((life-time)/2)));
        //glLineWidth(1);
        for(int i = startSize*0.5; i < startSize; i++){
            
            gl::drawSolidCircle(Vec2f(loc.x, loc.y),i);



        }
    }
    
    
    
}