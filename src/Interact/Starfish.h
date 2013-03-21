// Reacts to player proximity
// Changes colours and 'curls up'

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
    
    //getters
    bool activated();
    
private:
    vector<Feeler*> feelers;
    float spinCounter;
    int contacts;
};



#endif /* defined(__Cellv0__Starfish__) */
