#ifndef __Cellv0__SeaSurface__
#define __Cellv0__SeaSurface__

#include <iostream>
#include "GameObject.h"
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

#endif /* defined(__Cellv0__SeaSurface__) */
