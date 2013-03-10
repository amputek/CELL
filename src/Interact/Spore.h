//
//  Spore.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Spore__
#define __Cellv0__Spore__

#include <iostream>
#include "Dust.h"

using namespace std;

class Spore : public Dust{
public:
    Spore(Vec2f loc, float d, int t, gl::Texture* tex);
    void update();
    void draw();
    void collide(Vec2f loc);
    int type(){ return sporeType; };
    bool contact(){ return sporeContact; };
    bool alive(){ return health > 0; };
    
private:
    gl::Texture* img;
    int health;
    int sporeType;
    bool sporeContact;
};



#endif /* defined(__Cellv0__Spore__) */
