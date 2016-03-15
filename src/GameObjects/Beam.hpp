#ifndef Beam_hpp
#define Beam_hpp

#include <iostream>
#include "GameObject.hpp"
#include "cinder/gl/Texture.h"

class Beam : public GameObject{
public:
    Beam(vec2 start, gl::TextureRef* tex);
    void draw();
    void update();
    bool alive(){ return !(local.x < -1000 || local.x > 1800); };
    
private:
    float counter;
    float op;
    float life;
    gl::TextureRef* img;
};


#endif
