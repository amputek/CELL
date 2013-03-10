//
//  Plankton.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Plankton__
#define __Cellv0__Plankton__

#include <iostream>
#include "GameObject.h"

class Plankton : public GameObject{
public:
    Plankton(Vec2f loc, gl::Texture* tex, int t);
    void draw();
    bool checkEaten(Vec2f loc);
    bool alive();
    int type(){ return planktonType; };
private:
    int planktonType;
    gl::Texture* img;
};

#endif /* defined(__Cellv0__Plankton__) */
