//
//  Grass.cpp
//  Cell09
//
//  Created by Rob Dawson on 17/02/2017.
//
//

#include "Grass.hpp"



void Grass::draw()
{
    
  //  gl::ScopedBlendAlpha alpha;
    

    Shape2d mShape;
    mShape.moveTo( drawPositions.at(0) );
    for(int n = 0; n < drawPositions.size(); n++)
        mShape.lineTo( drawPositions.at(n) );
    mShape.close();
    gl::color( ColorA8u( 94,133,94, 50.0f ) );
    gl::drawSolid( mShape );
    gl::color( ColorA8u( 175,178,127, 150.0f ) );
    gl::draw( mShape );
}


void Grass::update()
{
    counter += deltaTime * randFloat(2.0f, 6.0f);
    addForce(vec2( randFloat(-10.0f,10.0f), randFloat(-55.0,-35.0f)));
    Feeler::update();
}
