#ifndef Beam_hpp
#define Beam_hpp

#include <iostream>
#include "GameObject.hpp"
#include "Renderer.hpp"

class Beam : public GameObject{
public:
    Beam(vec2 start);
    void draw( CellRenderer & renderer );
    void update();

private:
    float counter;
    float op;
    float life;
};


#endif
