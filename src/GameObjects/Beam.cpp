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
    counter+= rand(0.6f,6.0f) * deltaTime;
    if(life < 1.0){
        op = (0.2+sin(counter)*0.05f) * depth * life;
        life+=0.05;
    } else {
        op = (0.2+sin(counter)*0.05f) * depth;
    }
}


void Beam :: draw(){
    entityDrawCount++;
    gl::color(ColorA(1.0,1.0,1.0,op));
    gl::ScopedModelMatrix modelScope;
    float y = 0;
    gl::translate(vec2(local.x,y));
    gl::draw( *img, scaledRect );
}
