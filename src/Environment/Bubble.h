#ifndef __Cellv0__Bubble__
#define __Cellv0__Bubble__

#include <iostream>
#include "Dust.h"

class Bubble : public Dust{
public:
    Bubble( Vec2f loc, Vec2f vel, float d, gl::Texture* tex);
    bool alive();
    void draw();
    
private:
    gl::Texture* img;
    float opacity;
    
};


#endif /* defined(__Cellv0__Bubble__) */
