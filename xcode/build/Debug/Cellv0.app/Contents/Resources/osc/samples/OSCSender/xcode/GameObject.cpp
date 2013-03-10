//
//  GameObject.cpp
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#include "GameObject.h"


GameObject::GameObject(Vec2f location, int rad, string t){
    loc = location;
    radius = rad;
    opacity = 255;
    
    TextLayout text;
    text.setFont( Font( "Arial", 10 ) );
    text.setColor( Color( 1,1,1 ));
    text.addLine( t );
    textTexture = gl::Texture( text.render( true, false ) );

}

void GameObject::update(Vec2f mouseLoc){
    if(state == "dragging"){
        loc = mouseLoc;
    } else {
        Vec2f dir = loc - mouseLoc;
        if(dir.length() < radius){
            state = "mouseOver";
        } else {
            state = "inactive";
        }
    }
    
   //  cout << "game object state" << loc << "\n";
}

void GameObject::click(Vec2f mouseLoc){
    if(state == "mouseOver"){
        state = "dragging";
    }
}

void GameObject::unclick(){
    state = "inactive";
}

void GameObject::draw(){
    
    highlight();
    
};

bool GameObject::inCircle(){
    Vec2f dir = Vec2f(400,300) - loc;
    //cout << dir.length() << "\n";
    if(dir.length() <= 280){
        return true;
    } else {
        return false;
        
    }
    
}

float GameObject::getDistance(){
    Vec2f dir = Vec2f(400,300) - loc;
    return dir.length();
}

void GameObject::highlight(){
    if(state == "mouseOver"){
        if (opacity < 200) {
            opacity+=10;
        } else {
            opacity =200;
        }
    } else {
        if (opacity > 10) {
            opacity-=10;
        } else {
            opacity = 0;
        }
    }
    
    if(opacity > 1){
        
        glLineWidth( 0.1f );
        gl::color( ColorA8u(255,255,255,opacity*0.3));
        
       // for (int i = 0; i < radius; i+=4) {
       //     gl::drawStrokedCircle(loc, (radius)-i);
      //  }
        
        gl::color( ColorA8u(255, 255, 255, opacity));
        gl::drawStrokedCircle(loc, radius);
        
        float r = 2.3;
        
        float xl = loc.x+sin(r)*radius;
        float yl = loc.y+cos(r)*radius;
        
        float x2 = loc.x+sin(r)*(radius+15);
        float y2 = loc.y+cos(r)*(radius+15);
        
        gl::drawLine(Vec2f(xl, yl), Vec2f(x2, y2));
        gl::drawLine(Vec2f(x2, y2), Vec2f(x2+(textTexture.getWidth()), y2));
        
     //   gl::drawString(tooltip, Vec2f(x2+4, y2-12));
        
        
        gl::draw( textTexture, Vec2f(x2, y2-10) );
        
    }
    
    
}