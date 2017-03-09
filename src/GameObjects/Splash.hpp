#ifndef Splash_hpp
#define Splash_hpp

#include <iostream>
#include "Finite.hpp"
#include "Renderer.hpp"

using namespace ci;

class Splash : public Finite{
public:
    Splash(vec2 position, float size, int lifetime, float depth ) : Finite( position, lifetime, size, depth )
    {
        startSize = size;
    }
    
    void draw( CellRenderer & cellRenderer ){
        cellRenderer.drawSplash( mPosition, currentLife, maxLife, startSize );
    }

private:
    float startSize;
};


#endif
