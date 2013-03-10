//
//  Link.cpp
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#include "Link.h"

Link::Link(){
    state = "incomplete";
}

void Link::update(Vec2f mousePos){
    
    edge1 = node1->loc;
    float angle = atan2(edge1.x-edge2.x, edge1.y-edge2.y);
    float nx = edge1.x - sin(angle)*node1->radius;
    float ny = edge1.y - cos(angle)*node1->radius;
    edge1 = Vec2f(nx,ny);
    
    if(state == "incomplete"){
        edge2 = mousePos;
    } else {
        edge2 = node2->loc;
        float n2x = edge2.x + sin(angle)*node2->radius;
        float n2y = edge2.y + cos(angle)*node2->radius;
        edge2 = Vec2f(n2x,n2y);
    }
    
    if(state != "incomplete"){
        
        
        
                float a = (edge2.y - edge1.y) / (edge2.x - edge1.x);
                float b = edge1.y - a*edge1.x;
                
                if(abs(mousePos.y - (a*mousePos.x+b)) < 5){
                    
                    state = "over";
                } else {
                    state = "complete";
                    
                }
        
        // node2->orbit(node1->loc);
    }
}

void Link::draw(){
    
    float angle = atan2(edge1.x-edge2.x, edge1.y-edge2.y);
    
    gl::color(ColorA8u(255,255,255,50));
    for(float i = 1.0; i < 3.0; i+=0.05){
        
        float nx = node1->loc.x - sin(angle)*(node1->radius * i);
        float ny = node1->loc.y - cos(angle)*(node1->radius * i);
        Vec2f edge = Vec2f(nx,ny);
        gl::drawSolidCircle(edge,3-i);
        
        nx = node1->loc.x - sin(angle+(i-1.0))*(node1->radius);
        ny = node1->loc.y - cos(angle+(i-1.0))*(node1->radius);
        edge = Vec2f(nx,ny);
        gl::drawSolidCircle(edge,3-i);
        
        nx = node1->loc.x - sin(angle-(i-1.0))*(node1->radius);
        ny = node1->loc.y - cos(angle-(i-1.0))*(node1->radius);
        edge = Vec2f(nx,ny);
        gl::drawSolidCircle(edge,3-i);
        
        
        if(state != "incomplete"){
            nx = node2->loc.x + sin(angle)*(node2->radius * i);
            ny = node2->loc.y + cos(angle)*(node2->radius * i);
            edge = Vec2f(nx,ny);
            gl::drawSolidCircle(edge,3-i);
            
            nx = node2->loc.x + sin(angle+(i-1.0))*(node2->radius);
            ny = node2->loc.y + cos(angle+(i-1.0))*(node2->radius);
            edge = Vec2f(nx,ny);
            gl::drawSolidCircle(edge,3-i);
            
            nx = node2->loc.x + sin(angle-(i-1.0))*(node2->radius);
            ny = node2->loc.y + cos(angle-(i-1.0))*(node2->radius);
            edge = Vec2f(nx,ny);
            gl::drawSolidCircle(edge,3-i);
        }
    }
    
    
    
    gl::color(Color(1,1,1));
    glLineWidth(0.1);
    gl::drawLine(edge1, edge2);
    
    highlight();
    
}

void Link::highlight(){
    if(state == "over"){
        if (opacity < 200) {
            opacity+=20;
        } else {
            opacity =200;
        }
    } else {
        if (opacity > 10) {
            opacity-=20;
        } else {
            opacity = 0;
        }
    }
    
    if(opacity > 1){
        
        gl::color(ColorA8u(255,255,255,opacity));
        for(int i = 0; i < 1; i++){
            float point = Rand::randFloat(-3,3);
            
            float ax = ((edge1.x+edge2.x)/2) + (sin(point)*((edge2.x-edge1.x)/2));
            float ay = ((edge1.y+edge2.y)/2) + (sin(point)*((edge2.y-edge1.y)/2));
            Vec2f edge = Vec2f(ax,ay);
            
            sparks.push_back( Spark(edge,8,255,100,100));
        }
        for(int i = 0; i < 6; i++){
            
            gl::color(ColorA8u(255,100,100,opacity*0.2));
            glLineWidth(i);
            gl::drawLine(edge1, edge2);
        }
        
        for( list<Spark>::iterator p = sparks.begin(); p != sparks.end(); ++p ){
            p->draw();
            if(p->alive() == false){
                sparks.erase(p);
            }
        }

        
        
    }
    
    
}
