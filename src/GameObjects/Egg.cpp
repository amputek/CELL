#include "Egg.hpp"
#include "cinder/Triangulate.h"

Egg :: Egg(vec2 loc, gl::TextureRef* tex) : GameObject(loc, 1){
    radius = int(rand(80, 200)); //40
    ratio = radius * 2.4;
    for(int i = 1; i < 101; i++){
        float pos = (2*M_PI * i / (springs.size()) );
        //create new spring with damp, stiffness, mass parameters
        springs.push_back( new Spring( loc + vec2(sin(pos) * radius, cos(pos) * radius ) , 1.0, 1.07, 0.06, 1.1 ) );
    }
    
    img = tex;
    ins = false;
    
    //update a few times upon arrival
    for(int i = 0; i < 100; i++){
        update();
    }
}


//collide egg with a location (generally the player)
void Egg :: collide(vec2 loc){
    for(int i = 0; i < springs.size(); i++){
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
    
    //initilaise first point of egg to a specific point in space(updating causes a bug)
    float apos = (2*M_PI * (1) / (springs.size()) );
    springs.at(0)->global = global + vec2(sin(apos) * radius, cos(apos) * radius );
    
    //all other points 'update' towards desired location on circle
    for(int i = 0; i < springs.size(); i++){
        float pos = (2*M_PI * (i+1) / (springs.size()) );
        springs.at(i)->update( global + vec2(sin(pos) * radius, cos(pos) * radius ) );
    }
    
    //update each spring with it's neighbouring springs
    int j = 0;
    for(int i = int(rand(0,springs.size()-1)); j < springs.size(); i++, j++){
        springs.at( (i % (springs.size()-1)) )->update( springs.at( (i % (springs.size()-1)) +1 )->global );
    }
    
    j = 0;
    for(int i = int(rand(0,springs.size()-1)); j < springs.size(); i--, j++){
        springs.at( (i % (springs.size()-1)) + 1 )->update( springs.at( (i % (springs.size()-1)) )->global );
    }
    
    
    //create shape from spring locations
    mShape.clear();
    
    //draw shape
    mShape.moveTo( springs.at(0)->local );
    for(int i = 1; i < springs.size(); i++){
        mShape.lineTo( springs.at(i)->local );
    }
    mShape.close();
    
    //create mesh from shape
    mesh = Triangulator( mShape ).calcMesh();
}


void Egg :: draw(){
    
    gl::color(ColorA(1,1,1,0.75 + (sin(counter)*0.10)) );
    
    gl::draw( *img, Rectf(local.x - ratio, local.y - ratio, local.x + ratio, local.y + ratio) );
    glLineWidth(4);
    gl::color(ColorA (0.7,0.25,0.15,0.2)) ;
    gl::draw(mShape);
    gl::draw( mesh );
}