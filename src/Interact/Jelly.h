//
//  Jelly.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Jelly__
#define __Cellv0__Jelly__

#include <iostream>
#include "Swimmer.h"
#include "Feeler.h"

class Jelly : public Swimmer{
    
public:
    Jelly(Vec2f loc, int type, gl::Texture* tex);
    void update();
    void draw();
    void collide(Vec2f loc);
    vector<bool> contacts(){ return jellyContacts; };
    
private:
    vector<Feeler*> feelers;
    vector<Path2d> paths;
    vector<bool> jellyContacts;
    
    float counter;
    gl::Texture* img;
    
    float rMod; //modifies the radius of the feeler positions (depends on the png being used)
};

#endif /* defined(__Cellv0__Jelly__) */
