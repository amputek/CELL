//
//  Starfish.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Starfish__
#define __Cellv0__Starfish__

#include <iostream>
#include "Feeler.h"
#include "Swimmer.h"

class Starfish : public Swimmer{
public:
    Starfish(Vec2f loc);
    void update();
    void draw();
    void collide(Vec2f loc);
private:
    vector<Feeler*> feelers;
    float counter;
    int contacts;
};



#endif /* defined(__Cellv0__Starfish__) */
