#include "Beam.hpp"

Beam :: Beam(vec2 start): GameObject(start, randFloat(0.4,1.6), 1){
    counter = 0;
    op = 0.0;
    life = 0;
    
}

//fluctuating opacity
//new 'born' beams rise to full opacity over a period of 20 frames
void Beam :: update(){

    counter+= randFloat(0.6f,6.0f) * deltaTime;
    if(life < 1.0){
        op = (0.2+sin(counter)*0.05f) * mDepth * life;
        life += 0.02f;
    } else {
        op = (0.2+sin(counter)*0.05f) * mDepth;
    }
}


void Beam :: draw( CellRenderer & renderer ){
    renderer.drawBeam( mPosition, mDepth, op );
}
