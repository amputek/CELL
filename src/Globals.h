//
//  Globals.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Globals__
#define __Cellv0__Globals__

#include <iostream>
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
using namespace ci;

namespace globals {
    extern Vec2f offset;
    extern float rand(float a, float b);
    extern float rand(float a);
    extern Vec2f globalise(Vec2f loc, float d);
}


#endif /* defined(__Cellv0__Globals__) */
