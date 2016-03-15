#include "Egg.hpp"
#include "cinder/Triangulate.h"

Egg :: Egg(vec2 loc, gl::TextureRef* tex) : GameObject(loc, 1){
    radius = int(rand(80, 200)); //40
    ratio = radius * 2.4;
    for(int i = 0; i < numSprings; i++){
        float pos =  2 * M_PI * i / numSprings;
        //create new spring with damp, stiffness, mass parameters
        springs.push_back( new Spring( loc + vec2(sin(pos) * radius, cos(pos) * radius ) , 1.0, 1.07, 0.1, 2.02 ) );
    }
    
    img = tex;
    ins = false;
    
    //update a few times upon arrival
    for(int i = 0; i < numSprings; i++){
        update();
    }
}


//collide egg with a location (generally the player)
void Egg :: collide(vec2 loc){
    for(int i = 0; i < numSprings; i++){
        springs.at(i)->collide(loc);
    }
}

//true if a location (hero) is inside radius
void Egg :: setInside(vec2 loc){
    ins = dist(loc, global) < radius;
}


void Egg :: update(){
    
    GameObject::update();
    
    counter+=0.05;
    

    for(int i = 0; i < numSprings; i++){
        if( randFloat() < 0.2 ){
            float pos =  2 * M_PI * i / numSprings;
            springs.at(i)->update( global + vec2(sin(pos) * radius, cos(pos) * radius ) );
        }
    }



    //update each spring with it's neighbouring springs
    int j = 0;
    for(int i = randInt( numSprings-1 ); j < numSprings; i++, j++){
        int t = i % numSprings;
        int t2 = (i+1) % numSprings;
        springs.at( t )->update( springs.at( t2 )->global );
    }

    j = 0;
    for(int i = numSprings + randInt( numSprings-1); j < numSprings; i--, j++){
        int t = i % numSprings;
        int t2 = (i+1) % numSprings;
        springs.at( t2 )->update( springs.at( t )->global );
    }


    //create shape from spring locations
    mShape.clear();
    
    //draw shape
    mShape.moveTo( springs.at(0)->local );
    for(int i = 1; i < numSprings; i++){
        mShape.lineTo( springs.at(i)->local );
    }
    mShape.lineTo( springs.at(0)->local );
    mShape.close();
    
    //create mesh from shape
    mesh = Triangulator( mShape ).calcMesh();
}


void Egg :: draw(){
    gl::color(ColorA(1,1,1,0.75 + (sin(counter)*0.20)) );
    gl::draw( *img, Rectf(local.x - ratio, local.y - ratio, local.x + ratio, local.y + ratio) );
    glLineWidth(1);
    gl::color(ColorA (0.7,0.25,0.15,0.2)) ;
    gl::draw(mShape);
    gl::draw( mesh );
}