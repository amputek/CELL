#include "Urchin.h"

Urchin :: Urchin(Vec2f loc, gl::Texture* tex) : Swimmer(loc){
    radius = int(rand(10,30));
    speed = 0.0001 * radius;
    int feelerLength = radius * rand(0.2,0.5);
    for(int i = 0; i < int(rand(15,40)); i++){
        feelers.push_back(new Feeler(loc, feelerLength, 1));
    }
    img = tex;
}

void Urchin :: collide(Vec2f loc){
    contacts = 0;
    if( dist(loc,global) < 100){
        for(int i = 0; i < feelers.size(); i++){
            if( dist(loc, feelers.at(i)->global) < 75){
           //  feelers.at(i)->addForce( (loc - feelers.at(i)->global) * 0.1 );
            feelers.at(i)->collide( loc );
             contacts++;
            }
        }
    }
}

void Urchin :: updateFeelers(){
        
    for(int i = 0; i < feelers.size(); i++){
        float pos = 2*M_PI * i / feelers.size();
        feelers.at(i)->global = global + Vec2f(sin(pos) * radius, cos(pos) * radius);
        feelers.at(i)->update();
        feelers.at(i)->addForce( (feelers.at(i)->global - global) * 0.3 );
        feelers.at(i)->addForce( Vec2f(rand(-4,4), rand(-4,4) ) );
    }
    
    paths.clear();
    
    for(int i = 0; i < feelers.size(); i++){
        paths.push_back( feelers.at(i)->getPath() );
    }
}


void Urchin :: update(){
    
    updateFeelers();
    
    if(inSpace == false){
        if(speed < 0.005){  speed += 0.001; }
    } else {
        if(speed > 0.001){  speed -= 0.001; }
    }
    
    contacts--;
    if(contacts <= 0){      contacts = 0;   }
    if(contacts >= 100){    contacts = 100; }
    
    Swimmer::update();
}



void Urchin :: draw(){
    
    glLineWidth(0.1);
    gl::color(ColorA(1,0.5,1,0.5));
    
    for(int i = 0; i < paths.size(); i++){
        gl::draw( paths.at(i) );
    }
    
    for(int i = 0; i < feelers.size(); i++){
        gl::drawSolidCircle(feelers.at(i)->getEnd(), 3);
    }
    
    gl::color(Color(1,1,1));
    gl::draw( *img, Rectf(local.x - radius*2.5, local.y - radius*2.5, local.x + radius*2.5, local.y + radius*2.5) );

}