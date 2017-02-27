//
//  Grass.hpp
//  Cell09
//
//  Created by Rob Dawson on 17/02/2017.
//
//

#ifndef Grass_hpp
#define Grass_hpp

#include <stdio.h>
#include "Feeler.hpp"

using namespace std;
using namespace ci;


class Grass : public Feeler{
public:
    Grass(vec2 loc, int joints, float baseWidth, float tipWidth, float depth, gl::TextureRef * tex ) : Feeler( loc, joints, baseWidth, tipWidth, depth) {
        counter = randFloat(0.0f, M_PI * 2 );
        img = tex;
    };
    void draw();
    void update();
private:
    float counter = 0.0f;
    
    gl::TextureRef * img;
};



#endif /* Grass_hpp */
