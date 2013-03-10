//
//  Splash.cpp
//  Nodes2Multi
//
//  Created by Rob on 13/10/2012.
//
//

#include "cinder/Color.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Splash.h"



Splash::Splash(Vec2f aloc, float size){
    life = 15;
    loc = aloc;
    startSize = size;
}


Splash::Splash(Vec2f aloc, float size, float endSize){
    life = endSize;
    loc = aloc;
    startSize = size;
}


bool Splash::alive(){
    if(time > life){
        return false;
    }
    return true;
}

void Splash::draw(){
    
    time+=1;
    
    if(time<life){
        gl::color(ColorA8u(255,255,255, (life-time)));
        glLineWidth(1);
        gl::drawStrokedCircle(loc,time+startSize * 0.8);
        gl::drawStrokedCircle(loc,time+startSize);
    }
    
    

}