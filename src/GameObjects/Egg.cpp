#include "Egg.hpp"
#include "cinder/Triangulate.h"

Egg :: Egg(vec2 loc, gl::TextureRef* tex) : GameObject(loc, 1){
    radius = 160;
    ratio = radius * 2.4;
    

    float damping = 1.2f;
    float stiffness = 5.0f;
    float mass = 10.0f;
    
    
    for(int i = 0; i < numSprings; i++){
        float pos =  2 * M_PI * i / numSprings;
        //create new spring with damp, stiffness, mass parameters
        
        springs.push_back( new Spring( loc + vec2(sin(pos) * radius, cos(pos) * radius ) , stiffness, mass, damping  ) );
    }
    
    img = tex;
    ins = false;
    
    //update a few times upon arrival
    for(int i = 0; i < numSprings; i++){
        update();
    }
}


//collide egg with a location (generally the player)
void Egg :: collide(const vec2 & loc, float radius){
    for(int i = 0; i < numSprings; i++){
        springs.at(i)->collide(loc, radius);
    }
}

//true if a location (hero) is inside radius
void Egg :: setInside(vec2 loc){
    ins = dist(loc, global) < radius;
}


void Egg :: update(){
    
    GameObject::update();
    
    counter += deltaTime * 3.0f;
    


    for(int i = 0; i < numSprings; i++){
        if( randFloat() < 0.2f)
        {
            float pos =  2 * M_PI * i / numSprings;
            springs.at(i)->moveTowards( global + vec2(sin(pos) * radius, cos(pos) * radius ) );
        }
    }

    
    


    //update each spring with it's neighbouring springs
    int j = 0;
    for(int i = randInt( numSprings-1 ); j < numSprings; i++, j++){
        int t = i % numSprings;
        int t2 = (i+1) % numSprings;
        springs.at( t )->moveTowards( springs.at( t2 )->global );
    }

    j = 0;
    for(int i = numSprings + randInt( numSprings-1); j < numSprings; i--, j++){
        int t = i % numSprings;
        int t2 = (i+1) % numSprings;
        springs.at( t2 )->moveTowards( springs.at( t )->global );
    }

    
    drawPositions.clear();
    for(int i = 0; i < numSprings; i++)
    {
        int after = i + 1;
        if( after == numSprings ) after = 0;
        drawPositions.push_back( springs.at(i)->local );
        drawPositions.push_back( (springs.at(i)->local + springs.at(after)->local) * 0.5f);
    }
    
    //Average twice
    for(int n = 0; n < 2; n++)
    {
        for(int i = 0; i < drawPositions.size(); i++)
        {
            int before = i - 1;
            if( before < 0 ) before = drawPositions.size()-1;
            int after = i + 1;
            if( after == drawPositions.size() ) after = 0;
            vec2 p = (drawPositions.at(before) + drawPositions.at(after)) * 0.5f;
            drawPositions.at(i) = (drawPositions.at(i) + p) * 0.5f;
        }
    }
    

}


void Egg :: draw(){

    if( !onScreen() ) return;
    entityDrawCount++;
    
    
    gl::color(ColorA(1,1,1,0.5 + (sin(counter)*0.20f)) );
    gl::draw( *img, Rectf(local.x - ratio, local.y - ratio, local.x + ratio, local.y + ratio) );

    mShape.clear();
    mShape.moveTo( drawPositions.at(0) );
    for(int i = 1; i < drawPositions.size(); i++)
    {
        mShape.lineTo( drawPositions.at(i) );
    }
    mShape.close();
    
    gl::color(ColorA (0.7,0.25,0.15,0.5)) ;
    gl::drawSolid( mShape );
    gl::draw( mShape );

}
