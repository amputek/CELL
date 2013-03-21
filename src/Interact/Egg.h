//
//  Egg.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Egg__
#define __Cellv0__Egg__

#include <iostream>
#include "Spring.h"
#include "cinder/TriMesh.h"


class Egg : public GameObject{
public:
    Egg(Vec2f loc, gl::Texture* tex);
    void update();
    void draw();
    void collide(Vec2f loc);
    void setInside( Vec2f loc);
    
    //getters
    bool inside(){ return ins; };

private:
    vector<Spring*> springs;
    int ratio;          //constant value - ratio between png and shape size
    Shape2d mShape;     //The shape - connected points
    TriMesh2d mesh;     //The solid shape craeted by the Shape2D
    gl::Texture* img;
    bool ins;           //allows EntityManager to check if player is inside
};

#endif /* defined(__Cellv0__Egg__) */
