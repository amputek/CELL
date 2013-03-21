#include "Beam.h"

Beam :: Beam(Vec2f start, gl::Texture* tex): GameObject(start, rand(0.4,1.6)){
    counter = 0;
    op = 0.0;
    life = 0;
    img = tex;
}

//fluctuating opacity
//new 'born' beams rise to full opacity over a period of 20 frames
void Beam :: update(){
    GameObject :: update();
    counter+= rand(0.01,0.1);
    if(life < 1.0){
        op = (0.2+sin(counter)*0.05) * depth * life;
        life+=0.05;
    } else {
        op = (0.2+sin(counter)*0.05) * depth;
    }
}


void Beam :: draw(){
    gl::color(ColorA(1.0,1.0,1.0,op));
    gl::draw( *img, Vec2f(local.x, -50) );
}