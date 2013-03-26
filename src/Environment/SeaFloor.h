//
//  SeaFloor.h
//  Cellv0
//
//  Created by Rob on 26/03/2013.
//
//

#ifndef __Cellv0__SeaFloor__
#define __Cellv0__SeaFloor__

#include <iostream>
#include "GameObject.h"

class SeaFloor{
    
public:
    SeaFloor();
    void draw();
    void update();
    
private:
    std::vector<GameObject*> rows;
    std::vector<Path2d> paths;
    
};


#endif /* defined(__Cellv0__SeaFloor__) */
