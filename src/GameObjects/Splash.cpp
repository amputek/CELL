#include "Splash.hpp"


//alternative constructor has depth parameter too
Splash::Splash(vec2 aloc, float size, int l, float d, gl::TextureRef * tex):Finite(aloc,d){
    life = l;
    startSize = size;
    img = tex;
}

//decrease opacity and increase radius at every update stage
void Splash::update(){
    Finite::update();
    opacity = life-time;
    radius = startSize+time;
}

void Splash::draw(){
    //glLineWidth(2);
    gl::color(ColorA8u(255,255,255, opacity));
    //gl::drawStrokedCircle(local,radius);

    //gl::color(colorWithOpacity);
    gl::ScopedModelMatrix modelScope;
    gl::translate( local );
    gl::draw( *img, Rectf(-radius, -radius, radius, radius) );
    
    
}
