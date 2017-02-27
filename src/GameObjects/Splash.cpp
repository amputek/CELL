#include "Splash.hpp"


//alternative constructor has depth parameter too
Splash::Splash(vec2 aloc, float size, int l, float d, gl::TextureRef * tex):Finite(aloc,1,d){
    maxLife = l;
    startSize = size;
    img = tex;
}

//decrease opacity and increase radius at every update stage
void Splash::update(){
    Finite::update();
    float opacityRadiusMod = 1.0f;
    
    opacityRadiusMod = (radius-14.0f) * 0.002f;
    if(opacityRadiusMod >= 1.0f) opacityRadiusMod = 1.0f;
    if(opacityRadiusMod <= 0.4f) opacityRadiusMod = 0.4f;
    
    opacity = maxLife - currentLife;
    opacity *= opacityRadiusMod;
    
    radius = startSize + currentLife;
}

void Splash::draw(){
    
    if( !onScreen() ) return;
    entityDrawCount++;
    
    gl::color(ColorA8u(255,255,255, opacity));
    gl::ScopedModelMatrix modelScope;
    gl::translate( local );
    gl::draw( *img, Rectf(-radius, -radius, radius, radius) );
}
