#include "Starfish.hpp"

Starfish :: Starfish(vec2 loc) : Swimmer(loc){
    radius = int(rand(20,30));
    speed = 0.001;
    //random feeler length and random number of feelers
    int feelerLength = int(rand(3,5));
    for(int i = 0; i < int(rand(4,7)); i++){
        feelers.push_back(new Feeler(loc, feelerLength, 1));
    }
    spinCounter = 0;
    contacts = 0;
}

//feelers move away from Player
void Starfish:: collide(vec2 loc){
    if( dist(loc,global) < 100){
        for(int i = 0; i < feelers.size(); i++){
            if( dist(loc,feelers.at(i)->global) < 30){
                feelers.at(i)->addForce( (feelers.at(i)->global - loc) * 0.15f );
                contacts++;
            }
        }
    }
}

// true if enough contact has happened
bool Starfish :: activated(){
    if(contacts >= 100){
        contacts = 0;
        return true;
    }
    return false;
}

void Starfish :: update(){
    
    //spin amount 
    spinCounter+=contacts * 0.003;

    //reduce amount of contact
    if(contacts > 0){
        contacts--;
    }

    //update feelers
    for(int i = 0; i < feelers.size(); i++){
        
        //spin legs, depending on amount of contact. Distribute legs equally around the center
        float angle = 2*M_PI * i / feelers.size();
        feelers.at(i)->global = global + vec2(sin(angle + spinCounter) * 10, cos(angle + spinCounter) * 10);
        
        feelers.at(i)->update();
        
        //legs move away from the center of the starfish (push outwards)
        feelers.at(i)->addForce( (feelers.at(i)->global - global) * 0.7f );
        
        //random force added, which stops things being static - looks like the flow of underwater currents
        feelers.at(i)->addForce( vec2(rand(-4,4), rand(-4,4) ) );  
    }
    
    Swimmer::update();
}

void Starfish :: draw(){
    
    if(onScreen() == true){
        gl::color(ColorA8u(150,100 - contacts,10,2));
        
        for(int i = radius; i < 100; i+=5){
            gl::drawSolidCircle(local, i);
        }
        
        gl::color(ColorA8u(150,100 - contacts,10,60));
        
        for(int i = 1; i < radius; i+=2){
            gl::drawSolidCircle(local, i);
        }
        
        for(int i = 0; i < feelers.size(); i++){
            vector<vec2> points = feelers.at(i)->getPoints();
            float s = 15;
            for(int n = 1; n < points.size(); n+=2){
                gl::drawSolidCircle(points.at(n), s);
                gl::drawSolidCircle(points.at(n), 2);
                gl::drawSolidCircle(points.at(n), 2);
                s -= 1.0;
            }
        }
    }
    
}