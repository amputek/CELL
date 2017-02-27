#include "Jelly.hpp"

Jelly :: Jelly(vec2 loc, int type, gl::TextureRef* tex) : Swimmer(loc){
    
    Swimmer::speed = 0.002;
    
    GameObject::radius = int(rand(18,40));
    
    img = tex;
    
    jellyType = type;
    
    onScreenSize = 400;
    

    //3 different types of Jellyfish
    if(type == 0){ feelerStartRadius = radius * 0.25f; }
    if(type == 1){ feelerStartRadius = radius * 0.6f; }
    if(type == 2){ feelerStartRadius = radius * 0.6f; }
    
    //add a few feelers
    for(int i = 0; i < randInt(5,10); i++){
        
        int feelerLength = randInt(10,30);
        float tentacleStartWidth = randFloat(3.0f, 6.0f);
        float tentacleEndWidth = 0.2f;
        
        feelers.push_back( new Feeler(global, feelerLength, tentacleStartWidth, tentacleEndWidth));

    }
}


void Jelly :: collide(const vec2 & loc, float colliderSize){
    for(int i = 0; i < feelers.size(); i++){
        feelers.at(i)->collide(loc, colliderSize);
    }
}

void Jelly :: draw(){
    
    if( !onScreen() ) return;
    entityDrawCount++;
        
    gl::ScopedBlendAdditive additive;
    
    switch( jellyType )
    {
        case 0:
            gl::color(ColorA8u(150 + cos(counter) * 30,200 + sin(counter) * 30,200,155));
            break;
        case 1:
            gl::color(ColorA8u(150,200 + sin(counter) * 50,130,155));
            break;
        case 2:
            gl::color(ColorA8u(150,255,200,155));
            break;
    }
    
        
    //draw the feelers
    for(int i = 0; i < feelers.size(); i++){
        Shape2d mShape;
        mShape.moveTo( feelers.at(i)->drawPositions.at(0) );
        for(int n = 0; n < feelers.at(i)->drawPositions.size(); n++)
            mShape.lineTo( feelers.at(i)->drawPositions.at(n) );
        mShape.close();
        gl::drawSolid( mShape );
    }
    
    //Draw texture body
    gl::ScopedBlendAlpha alpha;
    gl::color(Color(1,1,1));
    float width  = radius*2 + sin(counter)*2;
    float height = radius*2 + cos(counter)*3;
    gl::draw( *img, Rectf(local.x - width, local.y - height, local.x + width, local.y + height )) ;

    
}


void Jelly :: update(){
    
    Swimmer::update();
    
    counter += deltaTime * 7.0f;
    
    for(int i = 0; i < feelers.size(); i++){
        feelers.at(i)->addForce( vec2(rand(-15,15), rand(18,34)) );
        float pos = (0.5*M_PI * i / (feelers.size()-1)) - 0.25*M_PI;
        feelers.at(i)->global = global + vec2(sin(pos) * feelerStartRadius, cos(pos) * feelerStartRadius);// + vec2(0, cos(counter)*3) * 1.0f;
        feelers.at(i)->update();  
    }
}
