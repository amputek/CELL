#include "Starfish.h"

Starfish :: Starfish(Vec2f loc) : Swimmer(loc){
    radius = int(rand(20,30));
    speed = 0.001;
    int feelerLength = int(rand(3,5));
    for(int i = 0; i < int(rand(4,7)); i++){
        feelers.push_back(new Feeler(loc, feelerLength, 1));
    }
    counter = 0;
    contacts = 0;
}

void Starfish:: collide(Vec2f loc){
    if((loc - global).length() < 100){
        for(int i = 0; i < feelers.size(); i++){
            if((loc-feelers.at(i)->global).lengthSquared() < 8000){
                feelers.at(i)->addForce( (feelers.at(i)->global - loc) * 0.15 );
            }
        }
    }
}

void Starfish :: update(){
    
    counter+=contacts * 0.0003;

    for(int i = 0; i < feelers.size(); i++){
        float angle = 2*M_PI * i / feelers.size();
        feelers.at(i)->global = global + Vec2f(sin(angle + counter) * 10, cos(angle + counter) * 10);
        feelers.at(i)->update();
        feelers.at(i)->addForce( (feelers.at(i)->global - global) * 0.7 );
        feelers.at(i)->addForce( Vec2f(rand(-4,4), rand(-4,4) ) );
    }
    
    Swimmer::update();
}



void Starfish :: draw(){
    
    if(onScreen() == true){
        gl::color(ColorA8u(150,100 - contacts,10,2));
        
        for(int i = radius; i < 100; i+=5){
            gl::drawSolidCircle(local, i);
        }
        
        gl::color(ColorA8u(150,100 - contacts,10,100));
        
        for(int i = 1; i < radius; i+=2){
            gl::drawSolidCircle(local, i);
        }
        
        for(int i = 0; i < feelers.size(); i++){
            vector<Vec2f> points = feelers.at(i)->getPoints();
            float s = radius*0.5;
            for(int n = 1; n < points.size(); n+=3){
                gl::drawSolidCircle(points.at(n), s);
                gl::drawSolidCircle(points.at(n), 2);
                gl::drawSolidCircle(points.at(n), 2);
                s -= (radius*0.02);
            }
        }
    }
    
}