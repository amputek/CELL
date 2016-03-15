#ifndef Friendly_hpp
#define Friendly_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Tail.hpp"

class Friendly : public Swimmer{
public:
    Friendly(vec2 loc, gl::TextureRef* tex);
    void draw();
    void update();
    void birth();
    
    //getters
    bool born(){ return mborn; };
    
private:
    int levelCount;
    Tail* tail;
    gl::TextureRef* img;
    bool mborn;
    bool birthing;
    int birthCount = 0;
};


#endif