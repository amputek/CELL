//
//  Urchin.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Urchin__
#define __Cellv0__Urchin__

#include <iostream>
#include "Swimmer.h"
#include "Feeler.h"


class Urchin : public Swimmer{
public:
    Urchin(Vec2f loc, gl::Texture* tex);
    void update();
    void draw();
    void collide(Vec2f loc);
    int contactAmount(){ return contacts; };
    
private:
    void updateFeelers();
    vector<Feeler*> feelers;
    vector<Path2d> paths;
    gl::Texture* img;
    int contacts;
};


#endif /* defined(__Cellv0__Urchin__) */
