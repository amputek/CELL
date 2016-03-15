#ifndef Egg_hpp
#define Egg_hpp

#include <iostream>
#include "Spring.hpp"
#include "cinder/TriMesh.h"


class Egg : public GameObject{
public:
    Egg(vec2 loc, gl::TextureRef* tex);
    void update();
    void draw();
    void collide(vec2 loc);
    void setInside( vec2 loc);
    
    //getters
    bool inside(){ return ins; };

private:
    vector<Spring*> springs;
    int ratio;          //constant value - ratio between png and shape size
    Shape2d mShape;     //The shape - connected points
    TriMesh mesh;     //The solid shape craeted by the Shape2D
    gl::TextureRef* img;
    bool ins;           //allows EntityManager to check if player is inside
    float counter = 0;
};

#endif
