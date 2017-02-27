#include "SeaFloor.hpp"

SeaFloor :: SeaFloor(){
    
    vec2 loc = vec2(0,0);
    
    for(float i = 1.0f; i >= 0.05f; i*=0.86f){
        rows.push_back( new GameObject(loc, i));
        paths.push_back( *new vector<vec2>() );
    }
    reverse( rows.begin(), rows.end() );
    
    perlin = Perlin(2); //create new perlin noise generator

}


void SeaFloor :: update(){
    

    for(int y = 0; y < rows.size(); y++){
        
        GameObject * r = rows.at(y);
        
        r->update();
        
        paths.at(y).clear();
        
        
        for(int x = 0; x <= cinder::app::getWindowWidth(); x+=10){
            
            float pX = x + offset.x;
            float pY = y * 45.0f;

            
            float perlinResult = perlin.fBm( vec3(pX, pY, 0.0f) / perlinDetail );
            
            float waveHeight = 70.0f * r->depth;
            
            float height = r->local.y + (perlinResult * waveHeight);
            
            
            vec2 point = vec2(x,height);
            
            paths.at(y).push_back( point );
            
        }
    }
    
    
    
}

void SeaFloor:: draw(){
    
    float globalOpacityMod = 1.0f;
    
    globalOpacityMod = (offset.y + 1000) * 0.002f;
    if( globalOpacityMod >= 1.0f ) globalOpacityMod = 1.0f;

    for(int n = 0; n < rows.size(); n++){
        Path2d path;
        path.moveTo( paths.at(n).at(0) );
        for(int i = 1; i < paths.at(n).size(); i++ )
            path.lineTo( paths.at(n).at(i) );
        path.lineTo( vec2( cinder::app::getWindowWidth(), cinder::app::getWindowHeight()) );
        path.lineTo( vec2( 0.0, cinder::app::getWindowHeight() ) );
        path.close();
        gl::color(ColorA8u(192,177,139, rows.at(n)->depth * 30.0f * globalOpacityMod ) );
        gl::drawSolid(path);
        gl::draw(path);
    }
}
