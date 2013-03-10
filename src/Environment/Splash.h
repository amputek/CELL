//
//  Splash.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Splash__
#define __Cellv0__Splash__

#include <iostream>
#include "Finite.h"

class Splash : public Finite{
public:
    Splash(Vec2f aloc, float size, int l, float d);
    void draw();
    void update();
    
private:
    int opacity;
    float startSize;
};


#endif /* defined(__Cellv0__Splash__) */
