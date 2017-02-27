#ifndef SeaSurface_hpp
#define SeaSurface_hpp

#include <iostream>
#include "GameObject.hpp"
#include "cinder/Perlin.h"

using namespace std;

class SeaSurface{
    
public:
    SeaSurface();
    void draw();
    void update();
    
private:
    float counter;
    vector<GameObject*> rows;
    Perlin perlin;
    vector< vector<vec2> > paths;
    const float perlinDetail = 300.0;
};

#endif
