//
//  Friendly.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Friendly__
#define __Cellv0__Friendly__

#include <iostream>
#include "Swimmer.h"
#include "Tail.h"

class Friendly : public Swimmer{
public:
    Friendly(Vec2f loc, gl::Texture* tex);
    void draw();
    void update();
    void birth();
    
    //getters
    bool born(){return mborn;};
    
private:
    int levelCount;
    Tail* tail;
    gl::Texture* img;
    bool mborn;
    bool birthing;
    int birthCount = 0;
};


#endif /* defined(__Cellv0__Friendly__) */
