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
    bool alive(){ return !(local.x < -1000 || local.x > 1800); };
    
private:
    float counter;
    float op;
    float life;
    gl::Texture* img;
};


#endif /* defined(__Cellv0__Beam__) */
