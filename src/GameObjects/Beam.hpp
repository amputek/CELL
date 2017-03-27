#ifndef Beam_hpp
#define Beam_hpp

#include <iostream>
#include "GameObject.hpp"


class Beam : public GameObject, public IDynamic, public IDrawable{
public:

    Beam(vec2 start): GameObject(start, randFloat(0.4,1.6), 1) { }
    
    //fluctuating opacity
    //new 'born' beams rise to full opacity over a period of 20 frames
    void update()
    {
        mCounter += randFloat(0.6f,6.0f) * deltaTime;
        if( mLife < 1.0 ) mLife += 0.02f;
        mOpacity = (0.2 + sin(mCounter) * 0.05f) * mDepth * mLife;
    }
    
    void draw( CellRenderer & renderer )
    {
        renderer.drawBeam( mPosition, mDepth, mOpacity );
    }
    
private:
    float mCounter = 0.0f;
    float mOpacity = 0.0f;
    float mLife = 0.0f;
};


#endif
