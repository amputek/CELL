#include "SeaFloor.h"

SeaFloor :: SeaFloor(){
    
    Vec2f loc = Vec2f(0,0);
    
    for(float i = 0.5; i < 2.0; i+=0.015){
        rows.push_back( new GameObject(loc, i));
    }
}


void SeaFloor :: update(){
    
    paths.clear();
    
    for(int n = 0; n < rows.size(); n++){
        
        GameObject* r = rows.at(n);
        r->update();
        
        Path2d floorPath;
        floorPath.moveTo(Vec2f(0, r->local.y) );
        floorPath.lineTo( Vec2f( cinder::app::getWindowWidth(), r->local.y) );
        
        //add to path collection
        paths.push_back( floorPath );
    }
}

void SeaFloor :: draw(){
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glLineWidth(10);
    for(int n = 0; n < paths.size(); n++){
        gl::color(ColorA8u(35,50,8,1.0 * n));
        gl::draw( paths.at(n) );
    }

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    
    
}