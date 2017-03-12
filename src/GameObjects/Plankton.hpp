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
    const int getType() const { return mPlanktonType; };
private:
    int mPlanktonType;
    float mRotation = 0.0f;
};

#endif
