#ifndef Splash_hpp
#define Splash_hpp

#include <iostream>
#include "Finite.hpp"

using namespace ci;

class Splash : public Finite{
public:
    Splash(vec2 aloc, float size, int l, float d);
    void draw();
    void update();
    
private:
    int opacity;
    float startSize;
};


#endif
