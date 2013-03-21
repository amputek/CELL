#include "SeaSurface.h"

SeaSurface :: SeaSurface(){
    
    Vec2f loc = Vec2f(0,-7000);
    for(float i = 0.1; i < 2.0; i+=0.025){
        rows.push_back( new GameObject(loc, i));
    }
    
    perlin = Perlin(2);
    counter = 0;
}


void SeaSurface :: update(){
    
    counter += 0.1;
   
    paths.clear();
    
    for(int n = 0; n < rows.size(); n++){
        
        GameObject* r = rows.at(n);
        r->update(); 
        
        Path2d floorPath;
        
        float perlinHeight = 50.0 * r->depth;
        
        floorPath.moveTo(Vec2f(0, r->local.y + perlin.fBm((offset.x) / perlinDetail) * perlinHeight));
        
        for(int i = 0; i <= cinder::app::getWindowWidth(); i+=10){
            float perl = perlin.fBm(( i + offset.x ) / perlinDetail );
            
            //draw point
            Vec2f point = Vec2f(i, r->local.y + perl * perlinHeight);
            
            //add wave motion
            point.y += sin(counter+(i*0.02*(2.0-r->depth)))*(10*r->depth);
            floorPath.lineTo(point);
        }
        
        paths.push_back( floorPath );
    }
    
}

void SeaSurface:: draw(){
    for(int n = 0; n < paths.size(); n++){
        glLineWidth(n*0.25);        //line width depends on depth (z axis)
        gl::color(ColorA8u(255,255,255,1*n));
        gl::draw( paths.at(n) );
    }
}