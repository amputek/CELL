#ifndef SeaFloor_hpp
#define SeaFloor_hpp

#include <iostream>
#include "GameObject.hpp"

class SeaFloor{
    
public:
    SeaFloor();
    void draw();
    void update();
    
private:
    std::vector<GameObject*> rows;
    std::vector<Path2d> paths;
    
};


#endif
