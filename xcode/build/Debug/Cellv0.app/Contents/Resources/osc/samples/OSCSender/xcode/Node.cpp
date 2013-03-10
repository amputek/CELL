//
//  Node.cpp
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#include "Node.h"


inline float myRand(float range);

inline float myRand(float range) {
    
    return Rand::randFloat(0.0f,range);
    
}

Node::Node(Vec2f location, string t):GameObject(location, 15, t){
    center = Vec2f(400, 300);
    state = "inactive";
    type = t;
}

void Node::update(Vec2f mouseLoc){

    if(state == "dragging"){
        
        Vec2f dir = Vec2f(mouseLoc.x, mouseLoc.y) - center;
        if(dir.length() > 280){
            cout << "out of circle" << "\n";
            collide(mouseLoc);
            velocity = Vec2f(0,0);
        } else {
            velocity = mouseLoc - loc;
            loc = mouseLoc;
        }
    } else if(state == "resizing"){
        resize(mouseLoc);
    } else {
        Vec2f dir = loc - mouseLoc;
        if(dir.length() < radius){
            state = "mouseOver";
        } else {
            state = "inactive";
        }
        
       // orbit(mouseLoc);
        
        if(inCircle() == false){
            velocity = -velocity;
            collide(loc);
        }
        
       // velocity *= 0.8;
       // loc += velocity;
        
      //  if(velocity.length() < 0.01){
      //      velocity = Vec2f(0,0);
      //  }
    }
    
    
   // loc += (mouseLoc-loc) * 0.01;
    
    
    
}

void Node::resize(Vec2f mouseLoc){
    if(state == "resizing"){
        radius = 10 + Vec2f(loc - mouseLoc).length();
        if(radius < 10){
            radius = 10;
        }
        if(radius > 50){
            radius = 50;
        }
    } else {
        Vec2f dir = loc - mouseLoc;
        if(dir.length() < radius){
            state = "resizing";
        }
    }
}

bool Node::trigger(float rippleLoc){
    Vec2f dist = loc - center;
    if(abs(dist.length() - rippleLoc) < 0.5){
        return true;
    }
    return false;
}

void Node::collide(Vec2f aLoc){
    float a = atan2(400 - aLoc.x, 300 - aLoc.y);
    float nx = 400 - sin(a) * (280);
    float ny = 300 - cos(a) * (280);
    loc = Vec2f(nx, ny);
}

void Node::draw(){
    
    //glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    
    GameObject::draw();

    glLineWidth(0.1);
    
    gl::color(ColorA8u(0,0,0,50));
    gl::drawSolidCircle(loc, radius*0.7);
    
    if(type == "sine"){
    
        gl::color(Color(1,1,1));
        
        int density = floor(280 - getDistance()) / 50.0;
        
        //gl::color(ColorA8u(255,100,100,5));
        for(int i = 0; i < density; i++){

            float si = Rand::randFloat(radius*0.05,radius*0.7);
           
            Vec2f aLoc = Vec2f(loc.x + radius*Rand::randGaussian()*0.1, loc.y + radius*Rand::randGaussian()*0.1);             
            sparks.push_back( Spark(aLoc,si,255,150,100) );
            

  
        } 
        
        for( list<Spark>::iterator p = sparks.begin(); p != sparks.end(); ++p ){
            p->draw();
            if(p->alive() == false){
                sparks.erase(p);
            }
        }
        
    } else if(type == "pad"){
        gl::color(Color(0,0,0));
        gl::drawSolidCircle(loc, radius*0.7);
        gl::color(ColorA8u(100,255,100,5));
        float j = radius * 0.35;
        for(float i = 1; i < radius*0.7; i+=2, j--){
            
            sparks.push_back( Spark(Vec2f(loc.x + i, loc.y),j,100,255,100));
            sparks.push_back( Spark(Vec2f(loc.x, loc.y + i),j,100,255,100));
            sparks.push_back( Spark(Vec2f(loc.x - i, loc.y),j,100,255,100));
            sparks.push_back( Spark(Vec2f(loc.x, loc.y - i),j,100,255,100));
        }
        for( list <Spark>::iterator p = sparks.begin(); p != sparks.end(); ++p ){
            p->draw();
            if(p->alive() == false){
                sparks.erase(p);
            }
        }
        
    } else if(type == "delay"){
        gl::color(Color(1,1,1));
        gl::drawSolidCircle(Vec2f(loc.x - radius * 0.4, loc.y), radius*0.3);
        gl::drawSolidCircle(Vec2f(loc.x + radius * 0.4, loc.y), radius*0.3);
        gl::drawLine(Vec2f(loc.x - radius * 0.4, loc.y), Vec2f(loc.x + radius * 0.4, loc.y));
    } else if(type == "filte"){
        gl::color(Color(1,1,1));
       // gl::drawSolidCircle(Vec2f(loc.x + radius * 0.4, loc.y), radius*0.3);
        glLineWidth(3);
        gl::drawLine(Vec2f(loc.x - radius * 0.5, loc.y - radius*0.5), Vec2f(loc.x + radius * 0.5, loc.y + radius*0.5));
        gl::drawLine(Vec2f(loc.x + radius * 0.5, loc.y - radius*0.5), Vec2f(loc.x - radius * 0.5, loc.y + radius*0.5));
    } else if(type == "filter"){
        
        gl::color(Color(1,1,1));
        
       // int density = floor(280 - getDistance()) / 20.0;
        glLineWidth(1);
       // glBegin(GL_LINE_STRIP);
        
        float j = 0;
        for(int i = 0; i < radius*0.7; i+=2, j=j+(pi / radius*2.8)){
            float x = (sin(j)*radius*0.7);
            float y = loc.y + (cos(j)*radius*0.7);
            
            gl::drawLine(Vec2f(loc.x+x,y), Vec2f(loc.x-x, y));
            gl::drawStrokedCircle(loc, radius*0.7);
        
        }
       // glEnd();
        
        
    }

    
};

void Node::addVelocity(Vec2f add){
    velocity += add;
}


void Node::orbit(Vec2f aLoc){
    
    float angle = (atan2(loc.y-aLoc.y, loc.x-aLoc.x) + ( pi/2 ));
    
    loc.x += cos(angle)*(abs(velocity.x)+abs(velocity.y));
    loc.y += sin(angle)*(abs(velocity.x)+abs(velocity.y));
    
    
    float distX = aLoc.x - loc.x;
    float distY = aLoc.y - loc.y;
    
  //  Vec2f dist = Vec2f
    float distance = sqrt((distX*distX)+(distY*distY));
 //   float distance = sqrt(sq(distX) + sq(distY));
    if (distance < 5) {
        distance = 5;
    }
    
    float m = 2;
    float m2 = 8;
    
    float grav = (m * m2) / (distance*distance) * 50;
    
    float xGrav = grav * (distX/distance);
    float yGrav = grav * (distY/distance);
    
    Vec2f accel = Vec2f(xGrav / m, yGrav / m);

    //addVelocity(accel);
    velocity.x += accel.x ;//* aMod);
    velocity.y += accel.y ;//* aMod);
    
    
    loc.x += velocity.x;
    loc.y += velocity.y;
}
