//
//  Beam.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Beam__
#define __Cellv0__Beam__

#include <iostream>
#include "GameObject.h"
#include "cinder/gl/Texture.h"

class Beam : public GameObject{
public:
    Beam(Vec2f start, gl::Texture* tex);
    void draw();
    void update();
    bool alive();
    gl::Texture* img;
    
private:
    float counter;
    float op;
    float width;
    float life;
    
};


#endif /* defined(__Cellv0__Beam__) */
