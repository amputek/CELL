#include "Spark.hpp"

Spark :: Spark(vec2 loc, int t) : Swimmer(loc){
    speed = 0.015;
    radius = 5;
    type = t;
}

void Spark :: update(){

    //radius returns to 5 after pulse (from SuperCollider)
    if(radius > 5){
        radius-=0.05;
    }
    
    Swimmer::update();
    
    
    //Spark is a collection of "Finites" that reduce in size
    
    finites.push_back( new Finite(global, radius) );
    
    for( vector<Finite*>::iterator p = finites.begin(); p != finites.end(); ){
        (*p)->update();
        if((*p)->alive() == false){
            delete *p;
            p = finites.erase(p);
        } else {
            ++p;
        }
    }
    
}

void Spark :: draw(){
    if(type == 0){ gl::color(ColorA8u( 255,       radius*25,     255-radius*25, 70 ) ); }
    if(type == 1){ gl::color(ColorA8u( radius*25, 255,           radius*25,     70 ) ); }
    if(type == 2){ gl::color(ColorA8u( 255,       255-radius*25, radius*25,     70 ) ); }
    
    //draw a few circles at each Finite position
    for(int i = 0; i < finites.size(); i++){
        for(int n = 1; n < finites.at(i)->life-finites.at(i)->time; n++){
            gl::drawSolidCircle( finites.at(i)->local, n);
        }
    }

}
