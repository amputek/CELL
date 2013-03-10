#include "Feeler.h"

Feeler :: Feeler(Vec2f loc, int joints, float d) : GameObject(loc, d){
    for(int i = 0; i < joints; i++){
        springs.push_back( new Spring(loc, d, 0.9, 0.6, 25.0 ) );
    }
    update();
    feelerContact = false;
};

void Feeler :: collide(Vec2f heroLoc){
    feelerContact = false;
    for(int i = 0; i < springs.size(); i++){
        springs.at(i)->collide(heroLoc);
        if(springs.at(i)->contact() == true){
            feelerContact = true;
        }
    }
}


void Feeler :: addForce(Vec2f force){
    for(int i = 0; i < springs.size(); i++){
        springs.at(i)->addForce(force);
    }    
}


void Feeler :: update(){
    
    //a Feeler's 'global' position is it's BASE point, the first spring gets updated agains this position
    if(springs.size() > 0){
        springs.at(0)->update(global);
    }
    
    //all other springs gets updated against the prior spring
    for(int i = 1; i < springs.size(); i++){
        springs.at(i)->update(springs.at(i-1)->global);
    }
    
    
    updatePoints();
    
    path.clear();
    
    if(points.size() > 0){
        path.moveTo(points.at(0));
        for(int i = 1; i < accuracy; i++){
            path.lineTo(points.at(i));
        }
    }
    
    GameObject::update();
}


void Feeler :: updatePoints(){
    
    //create a new path, fron the (local) base position
    Path2d tempPath;
    tempPath.moveTo(local);;
    
    //draw to each successive spring position
    for(int i = 0 ; i < springs.size(); i++){
        tempPath.lineTo(springs.at(i)->local);
    }
    
    
    //openGL allows lines to be accesed at a position between 0.0 and 1.0
    //the 'points' are created depending on an accuracy constant
    points.clear();
    for(int i = 0; i < accuracy; i++){
        points.push_back(tempPath.getPosition(i / accuracy));
    }
    
    //the points are averaged against neighbouring points, so the line becomes smoother
    for(int n = 0; n < 2; n++){
        for(int i = 1; i < accuracy-1; i++){
            points.at(i) = (points.at(i-1) + points.at(i) + points.at(i+1)) / 3;
        }
    }
    
}

void Feeler :: draw(){
    gl::color( Color( 255,255,0) );
    gl::draw( path ) ;
}