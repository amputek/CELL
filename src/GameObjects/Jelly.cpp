#include "Jelly.hpp"

Jelly :: Jelly(vec2 loc, int type, gl::TextureRef* tex) : Swimmer(loc){
    speed = 0.002;
    radius = int(rand(15,30));
    
    //3 different types of Jellyfish
    if(type == 0){ rMod = 0.4; }
    if(type == 1){ rMod = 0.6; }
    if(type == 2){ rMod = 0.6; }
    
    //add a few feelers
    for(int i = 0; i < int(rand(6,10)); i++){
        feelers.push_back( new Feeler(vec2(global.x, global.y), int(rand(5,8)), 1.0));
        jellyContacts.push_back(false);
    }
    counter = 0;
    img = tex;
}



void Jelly :: collide(vec2 loc){
    for(int i = 0; i < feelers.size(); i++){
        feelers.at(i)->collide(loc);
        jellyContacts.at(i) = feelers.at(i)->contact();
    }
}

void Jelly :: draw(){
    
    if(onScreen() == true){
        
        //draw the main body of the Jellyfish. the counter enables the body to pulse in size
        gl::color(Color(1,1,1));
        float width  = radius*2 + sin(counter)*2;
        float height = radius*2 + cos(counter)*3;
        gl::draw( *img, Rectf(local.x - width, local.y - height, local.x + width, local.y + height )) ;
        
        glLineWidth(1);
        
        //draw the feelers
        for(int i = 0; i < paths.size(); i++){
            gl::color(ColorA8u(150,255,200,40));
            gl::draw( paths.at(i) ) ;
            gl::drawSolidCircle( paths.at(i).getPosition(0), 2);
            
            float si = 5;
            //jellyfish's feelers have glowing elements that pass along them
            for(int t = 30; t > 1; t -= 1){
                gl::color(ColorA8u(200-t,100+t,200,25));
                if(t == counter*10){
                    gl::drawSolidCircle(paths.at(i).getPoint(t), (5-si)*0.75);
                }
                si-=0.1;
            }
        }
    }
    
    
}


void Jelly :: update(){
    
    Swimmer::update();
    counter += 0.1;
    
    //update feelers, load them into Path2D collection
    paths.clear();
    
    for(int i = 0; i < feelers.size(); i++){
        feelers.at(i)->addForce( vec2(rand(-5,5), rand(18,24)) );
        //distribute feeler positions along bottom of jellyfish, on radius*0.6
        float pos = (0.5*M_PI * i / (feelers.size()-1)) - 0.25*M_PI;
        feelers.at(i)->global = global + vec2(sin(pos) * radius*rMod, cos(pos) * radius*0.6);
        feelers.at(i)->update();
        
        paths.push_back( feelers.at(i)->getPath() );
    }
}