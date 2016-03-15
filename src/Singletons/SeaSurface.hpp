#ifndef SeaSurface_hpp
#define SeaSurface_hpp

#include <iostream>
#include "GameObject.hpp"
#include "cinder/Perlin.h"

class SeaSurface{
    
public:
    SeaSurface();
    void draw();
    void update();
    
private:
    float counter;
    std::vector<GameObject*> rows;
    Perlin perlin;
    std::vector<Path2d> paths;
    const float perlinDetail = 300.0;
};

#endif
