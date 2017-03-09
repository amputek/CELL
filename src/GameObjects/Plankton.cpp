#include "Plankton.hpp"


Plankton :: Plankton( vec2 loc, int t) : GameObject(loc, randFloat(0.9,1.1), randFloat(6,13) ){
    planktonType = t;
    rotation = randFloat(0, M_PI * 2);
}

void Plankton :: draw( CellRenderer & renderer ){
    renderer.drawPlankton(mPosition, mDepth, mRadius, planktonType, rotation );
}

bool Plankton :: checkEaten( const vec2 & heroLocation ){
    return dist(heroLocation, mPosition) < 10;
}
