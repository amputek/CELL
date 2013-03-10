#include "Splash.h"


//alternative constructor has depth parameter too
Splash::Splash(Vec2f aloc, float size, int l, float d):Finite(aloc,d){
    life = l;
    startSize = size;
}

//decrease opacity and increase radius at every update stage
void Splash::update(){
    Finite::update();
    opacity = life-time;
    radius = startSize+time;
}

void Splash::draw(){
    glLineWidth(2);
    gl::color(ColorA8u(255,255,255, opacity));
    gl::drawStrokedCircle(local,radius);
}