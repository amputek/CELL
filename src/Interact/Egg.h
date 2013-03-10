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
    bool inside(){ return ins; };
    
private:
    vector<Spring*> springs;
    int ratio;
    Shape2d mShape;
    TriMesh2d mesh;
    gl::Texture* img;
    bool ins;
    
    
};

#endif /* defined(__Cellv0__Egg__) */
