#include "Beam.hpp"

Beam :: Beam(vec2 start, gl::TextureRef* tex): GameObject(start, rand(0.4,1.6)){
    counter = 0;
    op = 0.0;
    life = 0;
    img = tex;
    
    
    float texHeight = (*tex)->getHeight();
    float windowHeight = cinder::app::getWindowHeight() + 100;
    float r = windowHeight / texHeight;
    
    scaledRect = Rectf(0, 0, (*tex)->getWidth() * r, texHeight * r );
    
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
    
   gl::ScopedModelMatrix modelScope;
   gl::translate(vec2(local.x,-50));
    gl::draw( *img, scaledRect );
    
}
