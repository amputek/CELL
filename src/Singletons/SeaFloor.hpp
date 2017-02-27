#ifndef SeaFloor_hpp
#define SeaFloor_hpp

#include <iostream>
#include "GameObject.hpp"
#include "cinder/Perlin.h"

using namespace std;

class SeaFloor{
    
public:
    SeaFloor();
    void draw();
    void update();
    
private:
    std::vector<GameObject*> rows;
    std::vector< vector<vec2> > paths;
    const float perlinDetail = 300.0;
    Perlin perlin;
};


#endif
