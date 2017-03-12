#include "Plankton.hpp"


Plankton :: Plankton( vec2 loc, int t) : GameObject(loc, randFloat(0.9,1.1), randFloat(6,13) ){
    mPlanktonType = t;
    mRotation = randFloat(0, M_PI * 2);
}

void Plankton :: draw( CellRenderer & renderer ){
    renderer.drawPlankton(mPosition, mDepth, mRadius, mPlanktonType, mRotation );
}
