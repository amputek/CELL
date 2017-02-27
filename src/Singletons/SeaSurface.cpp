#include "SeaSurface.hpp"

SeaSurface :: SeaSurface(){
    
    vec2 loc = vec2(0,-7000);
    
    for(float i = 2.0f; i >= 0.05f; i*=0.82f){
        rows.push_back( new GameObject(loc, i));
        paths.push_back( *new vector<vec2>() );
    }
    reverse( rows.begin(), rows.end() );
    
    perlin = Perlin(2); //create new perlin noise generator
    counter = 0;        //initialise sinewave counter
}


void SeaSurface :: update(){
    
    counter += deltaTime * 150.0f;     //counter used in sine function for wave motion
   
    for(int y = 0; y < rows.size(); y++){
        
        GameObject * r = rows.at(y);
        
        r->update();
        
        paths.at(y).clear();
        
        
        for(int x = 0; x <= cinder::app::getWindowWidth(); x+=10){

            float pX = x + offset.x;
            float pY = y * 45.0f;
            float pZ = counter;
            
            float perlinResult = perlin.fBm( vec3(pX, pY, pZ) / perlinDetail );

            float waveHeight = 140.0f * r->depth;

            float height = r->local.y + (perlinResult * waveHeight);
            
            
            vec2 point = vec2(x,height);
            
            paths.at(y).push_back( point );
            
        }
    }
    
    

}

void SeaSurface:: draw(){

    float globalOpacityMod = 1.0f;
    
    globalOpacityMod = -(offset.y + 6000) * 0.002f;
    if( globalOpacityMod >= 1.0f ) globalOpacityMod = 1.0f;
    
    for(int n = 0; n < rows.size(); n++){
        Path2d path;
        path.moveTo( paths.at(n).at(0) );
        for(int i = 1; i < paths.at(n).size(); i++ )
            path.lineTo( paths.at(n).at(i) );
        path.lineTo( vec2( cinder::app::getWindowWidth(), 0.0 ) );
        path.lineTo( vec2( 0.0, 0.0 ) );
        path.close();
        gl::color(ColorA8u(237,160, 135, rows.at(n)->depth * 20.0f * globalOpacityMod ) );
        gl::drawSolid(path);
        gl::draw(path);
    }
}
