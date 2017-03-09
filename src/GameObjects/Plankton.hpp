#ifndef Plankton_hpp
#define Plankton_hpp

#include <iostream>
#include "GameObject.hpp"
#include "Renderer.hpp"

using namespace ci;

class Plankton : public GameObject{
public:
    Plankton( vec2 loc, int t);
    ~Plankton(){ }
    void draw( CellRenderer & renderer );
    bool checkEaten(const vec2 & loc);
    int type(){ return planktonType; };
private:
    int planktonType;
    float rotation = 0.0f;
};

#endif
