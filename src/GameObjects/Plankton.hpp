#ifndef Plankton_hpp
#define Plankton_hpp

#include <iostream>
#include "GameObject.hpp"

using namespace ci;

class Plankton : public GameObject{
public:
    Plankton( vec2 loc, gl::TextureRef* tex, int t);
    ~Plankton(){ }
    void draw();
    bool checkEaten(const vec2 & loc);
    bool alive();
    int type(){ return planktonType; };
private:
    int planktonType;
    gl::TextureRef* img;
    float rotation = 0.0f;
};

#endif
