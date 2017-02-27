#ifndef Egg_hpp
#define Egg_hpp

#include <iostream>
#include "Spring.hpp"
#include "cinder/TriMesh.h"


class Egg : public GameObject{
public:
    Egg(vec2 loc, gl::TextureRef* tex);
    ~Egg(){
        for( vector<Spring*>::iterator p = springs.begin(); p != springs.end(); ++p ){
            delete *p;
        }
    }
    
    
    
    
    void update();
    void draw();
    void collide(const vec2 & loc, float radius);
    void setInside( vec2 loc);
    
    //getters
    bool inside(){ return ins; };

private:
    vector<Spring*> springs;
    vector<vec2> drawPositions;
    int ratio;          //constant value - ratio between png and shape size
    Shape2d mShape;     //The shape - connected points

    gl::TextureRef* img;
    bool ins;           //allows EntityManager to check if player is inside
    float counter = 0;
    
    const int numSprings = 80;
};

#endif
