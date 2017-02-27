#include "Urchin.hpp"

Urchin :: Urchin(vec2 loc, gl::TextureRef* tex) : Swimmer(loc, true){
    
    GameObject::radius = int(rand(10,30));
    
    Braitenberg::speed = 0.001f * radius;
    
    int feelerLength = radius * rand(0.2,0.5);
    float tentacleStartWidth = 4.0f;
    float tentacleEndWidth = 0.2f;
    
    for(int i = 0; i < int(rand(15,40)); i++){
        feelers.push_back(new Feeler(loc, feelerLength, tentacleStartWidth, tentacleEndWidth));
    }
    
    img = tex;
}


void Urchin :: collide(const vec2 & loc, float colliderSize ){
    //count number of feelers that are making contact
    //contactCount = 0;
    //only bother checking if hero is near enough
    if( dist(loc,global) < 200){
        for(int i = 0; i < feelers.size(); i++){
            feelers.at(i)->collide( loc, colliderSize );
            if(feelers.at(i)->inContactWithCollider()){
                contactCount++;
            }
        }
    }
    
    //if( contactCount > 0 ) cout << contactCount << "\n";
}


void Urchin :: updateFeelers(){
    
    for(int i = 0; i < feelers.size(); i++){
        //distribute feelers equally around the Urchin
        float pos = 2*M_PI * i / feelers.size();
        feelers.at(i)->global = global + vec2(sin(pos) * radius, cos(pos) * radius);
        feelers.at(i)->update();
        feelers.at(i)->addForce( (feelers.at(i)->global - global) * 1.0f );
        //random force added to simulate underwater currents
        feelers.at(i)->addForce( vrand(30.0f));

    }
    
}


void Urchin :: update(){
    
    if( randFloat() < 0.001f )
    {
        setDestination( global + vrand(400.0f) );
    }
    
    //collide( global, radius );
    
    updateFeelers();

    
    contactCount--;
    if(contactCount <= 0){      contactCount = 0;   }
    if(contactCount >= 10){     contactCount = 10; }
    
    Swimmer::update();
}


void Urchin :: draw(){

    if( !onScreen() ) return;
    entityDrawCount++;    

    gl::color(ColorA(1,0.5,1,0.5));
    
    //draw the feelers
    for(int i = 0; i < feelers.size(); i++){
        Shape2d mShape;
        mShape.moveTo( feelers.at(i)->drawPositions.at(0) );
        for(int n = 0; n < feelers.at(i)->drawPositions.size(); n++)
            mShape.lineTo( feelers.at(i)->drawPositions.at(n) );
        mShape.close();
        gl::drawSolid( mShape );
    }
    
    gl::color(Color(1,1,1));
    gl::draw( *img, Rectf(local.x - radius*2.5, local.y - radius*2.5, local.x + radius*2.5, local.y + radius*2.5) );
}
